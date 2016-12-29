#ifndef BINDERTESTSERVICE_H
#define BINDERTESTSERVICE_H
#include <utils/Log.h>
#include <binder/BinderService.h>
#include "../common/IBinderTestService.h"

namespace android {
class BinderTestService : public BinderService<BinderTestService>,
	public BnBinderTestService {
	friend class BinderService<BinderTestService>;
public:
	static char const * getServiceName() {return "reoadpassion.service";}
	BinderTestService();
	virtual ~BinderTestService();
	virtual void start();
	virtual void setVal(int32_t val);
	virtual int32_t getVal();
private:
	int32_t value;
};
};
#endif
