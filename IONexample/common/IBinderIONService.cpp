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

	virtual int32_t getIonBufFd() {
		Parcel data, reply;
		data.writeInterfaceToken(IBinderIONService::getInterfaceDescriptor());
		remote()->transact(FETCH_ION_BUF, data, &reply);
		return dup(reply.readFileDescriptor());
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
            reply->writeDupFileDescriptor(getIonBufFd());
            //reply->writeFileDescriptor(getIonBufFd());
            return NO_ERROR;
            break;
		default:
			return BBinder::onTransact(code, data, reply, flags);
	}
}
};

