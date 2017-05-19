#ifndef BINDERIONSERVICE_H
#define BINDERIONSERVICE_H
#include <utils/Log.h>
#include <binder/BinderService.h>
#include "../common/IBinderIONService.h"
#include <errno.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>

#define FUNC_NAME(name) ALOGE("FUNCTION NAME: %s", name);
#define ION_DEV_NAME "/dev/ion"

namespace android {
class BinderIONService : public BinderService<BinderIONService>,
	public BnBinderIONService {
	friend class BinderService<BinderIONService>;
public:
	static char const * getServiceName() {return "reoadpassion.service";}
	BinderIONService();
	virtual ~BinderIONService();
	virtual void start();
    virtual void stop();
	virtual void setVal(int32_t val);
	virtual int32_t getVal();
    virtual int32_t getIonBufFd() { return ion_buf_fd; }
private:
    void initializeION();
    void destroyION();
	int32_t value;
    int32_t main_ion_fd;
    int32_t ion_buf_fd;
};
};
#endif
