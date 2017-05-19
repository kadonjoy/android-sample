#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include "../libserver/BinderIONService.h"

using namespace android;
int main(int __attribute__((unused)) argc, char ** __attribute__((unused)) argv) {
	sp<ProcessState> proc(ProcessState::self());
	sp<IServiceManager> sm = defaultServiceManager();
	BinderIONService::instantiate();
	ProcessState::self()->startThreadPool();
	IPCThreadState::self()->joinThreadPool();
}
