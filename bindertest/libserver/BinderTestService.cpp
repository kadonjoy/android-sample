#define LOG_TAG "BinderTestService"
#include "BinderTestService.h"

using namespace android;

BinderTestService::BinderTestService() : value(0) {

}

BinderTestService::~BinderTestService() {

}

void BinderTestService::start() {
	ALOGE("BinderTestService::start");
}

void BinderTestService::setVal(int32_t val) {
	value = val;
	ALOGE("BinderTestService value is %d\n", value);
}

int32_t BinderTestService::getVal() {
	ALOGE("BinderTestService::getVal");
	return value;
}
