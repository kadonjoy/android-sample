#include <utils/Log.h>
#include <binder/IServiceManager.h>
#include "../common/IBinderTestService.h"

using namespace android;

int main(int argc, char** argv) {
	sp<IServiceManager> sm = defaultServiceManager();
	sp<IBinder> binder = sm->getService(String16("reoadpassion.service"));
	//sp<IBinderTestService> mBinderTestService = interface_cast<IBinderTestService>(binder);
	sp<IBinderTestService> mBinderTestService = IBinderTestService::asInterface(binder);
	mBinderTestService->start();
	mBinderTestService->setVal(3);
	ALOGE("value is %d\n", mBinderTestService->getVal());
}
