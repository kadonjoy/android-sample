#ifndef IBINDERTESTSERVERICE_H
#define IBINDERTESTSERVERICE_H

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

namespace android {

class IBinderTestService : public IInterface {
public:
	enum {
		START = IBinder::FIRST_CALL_TRANSACTION,
		SETVAL,
		GETVAL,
	};
	DECLARE_META_INTERFACE(BinderTestService);
	virtual void start () = 0;
	virtual void setVal(int32_t val) = 0;
	virtual int32_t getVal() = 0;
};

class BnBinderTestService : public BnInterface<IBinderTestService> {
public:
	virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel * reply, uint32_t flags = 0);
};
};
#endif
