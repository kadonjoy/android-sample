#define LOG_TAG "BInderTestService"

#include <utils/Log.h>
#include <stdint.h>
#include <sys/types.h>
#include <binder/Parcel.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include "IBinderTestService.h"
#define SERVICENAME "com.roadpassion.bindertest"

//using namespace android;
namespace android {

class BpBinderTestService : public BpInterface<IBinderTestService> {
public:
	BpBinderTestService(const sp<IBinder>& impl) : BpInterface<IBinderTestService>(impl) {

	}

	virtual void start() {
		Parcel data, reply;
		data.writeInterfaceToken(IBinderTestService::getInterfaceDescriptor());
		data.writeString16(String16("hello binder start!"));
		remote()->transact(START, data, &reply);
	}
	
	virtual void setVal(int32_t val) {
		Parcel data, reply;
		data.writeInterfaceToken(IBinderTestService::getInterfaceDescriptor());
		data.writeInt32(val);
		remote()->transact(SETVAL, data, &reply);
	}
	
	virtual int32_t getVal() {
		Parcel data, reply;
		data.writeInterfaceToken(IBinderTestService::getInterfaceDescriptor());
		remote()->transact(GETVAL, data, &reply);
		return reply.readInt32();
	}
};

IMPLEMENT_META_INTERFACE(BinderTestService, SERVICENAME);

status_t BnBinderTestService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags) {
	switch (code) {
		case START:
			CHECK_INTERFACE(IBinderTestService, data, reply);
			start();
			return NO_ERROR;
			break;
		case SETVAL:
			CHECK_INTERFACE(IBinderTestService, data, reply);
			setVal(data.readInt32());
			return NO_ERROR;
			break;
		case GETVAL:
			CHECK_INTERFACE(IBinderTestService, data, reply);
			reply->writeInt32(getVal());
			return NO_ERROR;
			break;
		default:
			return BBinder::onTransact(code, data, reply, flags);
	}
}
};

