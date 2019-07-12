/*
 **************************************************************************************
 *       Filename:  main.cpp
 *    Description:   source file
 *
 *        Version:  1.0
 *        Created:  2019-07-12 17:28:33
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */
#ifdef __ANDROID__
#include <android/log.h>
#include <utils/CallStack.h>
#define TAG "CallStack-DEMO"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__)
#endif

#define UNUSED(x) do { break;} while(x);

#ifdef __ANDROID__
void save_bt() {
  android::CallStack cs; 
  cs.update();

  cs.log(TAG);
}
#endif

int main(int argc, const char *argv[]) {

    UNUSED(argc);
    UNUSED(argv);
    LOGE("...STARTING CallStack Call...");
    save_bt(); 

    return 0;
}


/********************************** END **********************************************/

