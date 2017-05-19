#define LOG_TAG "BInderIONService"

#include <utils/Log.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <binder/Parcel.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include "IBinderIONService.h"
#define SERVICENAME "com.roadpassion.binderION"

//using namespace android;
namespace android {

class BpBinderIONService : public BpInterface<IBinderIONService> {
public:
	BpBinderIONService(const sp<IBinder>& impl) : BpInterface<IBinderIONService>(impl) {

	}

	virtual void start() {
		Parcel data, reply;
		data.writeInterfaceToken(IBinderIONService::getInterfaceDescriptor());
		data.writeString16(String16("hello binder start!"));
		remote()->transact(START, data, &reply);
	}
	
	virtual void setVal(int32_t val) {
		Parcel data, reply;
		data.writeInterfaceToken(IBinderIONService::getInterfaceDescriptor());
		data.writeInt32(val);
		remote()->transact(SETVAL, data, &reply);
	}
	
	virtual int32_t getVal() {
		Parcel data, reply;
		data.writeInterfaceToken(IBinderIONService::getInterfaceDescriptor());
		remote()->transact(GETVAL, data, &reply);
		return reply.readInt32();
	}

	virtual int32_t getIonBufFd(Parcel & reply) {
		Parcel data;
		data.writeInterfaceToken(IBinderIONService::getInterfaceDescriptor());
		remote()->transact(FETCH_ION_BUF, data, &reply);
        // 由于Parcel析构函数中会调用closeFileDescriptors关掉对象里的所有fd,
        // 那么fd在kernel里对应的struct file指针就会被置空，导致后续需要使用fd去映射
        // ion的进程会出现找不到有效file的情况。
        // 
        // 这里有两个解决方案:
        // 1. reply为局部变量，函数返回之前需要dup()一下驱动返回的fd，dup()成功返回一个新的new_fd
        // 与fd一样指向相同的file结构，这样即使reply 在该函数返回之后释放了fd, 该进程仍然用
        // new_fd找到正确的file结构
        // 2. reply为该进程维护的变量，在使用完fd之后才会析构，这样就不会出现fd被提前释放的情况

		//return dup(reply.readFileDescriptor());
		return reply.readFileDescriptor();
	}
};

IMPLEMENT_META_INTERFACE(BinderIONService, SERVICENAME);

status_t BnBinderIONService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags) {
	switch (code) {
		case START:
			CHECK_INTERFACE(IBinderIONService, data, reply);
			start();
			return NO_ERROR;
			break;
		case SETVAL:
			CHECK_INTERFACE(IBinderIONService, data, reply);
			setVal(data.readInt32());
			return NO_ERROR;
			break;
		case GETVAL:
			CHECK_INTERFACE(IBinderIONService, data, reply);
			reply->writeInt32(getVal());
			return NO_ERROR;
			break;
        case FETCH_ION_BUF:
            CHECK_INTERFACE(IBinderIONService, data, reply);
            // reply->writeDupFileDescriptor(getIonBufFd(*reply));
            // ion fd需要用binder传输到目的进程，因为binder驱动里会做特殊操作，
            // binder驱动会在target进程找到未使用的fd, 然后指向当前进程的ion fd 对应的file结构，
            // 这样两个进程使用不同的fd指向相同的file, 达到共享ion buffer的目的.
            // ⚠ 目前ion buffer只用用binder实现，管道等进程通信方式不能使用.
            reply->writeFileDescriptor(getIonBufFd(*reply));
            return NO_ERROR;
            break;
		default:
			return BBinder::onTransact(code, data, reply, flags);
	}
}
};

