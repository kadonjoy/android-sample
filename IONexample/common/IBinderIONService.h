#ifndef IBINDERIONSERVERICE_H
#define IBINDERIONSERVERICE_H

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

namespace android {

class IBinderIONService : public IInterface {
public:
	enum {
		START = IBinder::FIRST_CALL_TRANSACTION,
		SETVAL,
		GETVAL,
        FETCH_ION_BUF,
	};
	DECLARE_META_INTERFACE(BinderIONService);
	virtual void start () = 0;
	virtual void setVal(int32_t val) = 0;
	virtual int32_t getVal() = 0;
	virtual int32_t getIonBufFd(Parcel & reply) = 0;
};

class BnBinderIONService : public BnInterface<IBinderIONService> {
public:
	virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel * reply, uint32_t flags = 0);
};
};
#endif
