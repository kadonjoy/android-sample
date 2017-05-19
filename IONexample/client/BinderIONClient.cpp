#include <utils/Log.h>
#include <binder/IServiceManager.h>
#include <linux/msm_ion.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "../common/IBinderIONService.h"

#define ION_DEV_NAME "/dev/ion"

using namespace android;

int main_ion_fd = -1;
void client_use_ion_buf(int ion_buf_fd);

int main(int __attribute__ ((unused)) argc, char** __attribute__ ((unused)) argv) {
    int ion_buf_fd = -1;

    /* opening ion dev */
    main_ion_fd = open(ION_DEV_NAME, O_RDONLY);
    if (main_ion_fd < 0) {
        ALOGE("Ion dev open failed: %s\n", strerror(errno));
        close(main_ion_fd);
        exit(1);
    }

	sp<IServiceManager> sm = defaultServiceManager();
	sp<IBinder> binder = sm->getService(String16("reoadpassion.service"));
	//sp<IBinderIONService> mBinderIONService = interface_cast<IBinderIONService>(binder);
	sp<IBinderIONService> mBinderIONService = IBinderIONService::asInterface(binder);
	mBinderIONService->start();
	//mBinderIONService->setVal(3);
    ion_buf_fd = mBinderIONService->getIonBufFd();
	//ALOGE("value is %d\n", mBinderIONService->getVal());
	//ALOGE("ion buf fd is %d pid: %d\n", ion_buf_fd, getpid());
	printf("ion buf fd is %d pid: %d\n", ion_buf_fd, getpid());
    client_use_ion_buf(ion_buf_fd);
    return 0;
}

void client_use_ion_buf(int ion_buf_fd)
{
    int rc = 0;
    void * vaddr = NULL;


    printf("\n[parent] ion_buf_fd: %d main_ion_fd: %d\n",
            ion_buf_fd, main_ion_fd);

    /* import ion buffer */
    struct ion_fd_data ion_info_fd;
    memset(&ion_info_fd, 0, sizeof(ion_info_fd));
    ion_info_fd.fd = ion_buf_fd;
    rc = ioctl(main_ion_fd, ION_IOC_IMPORT, &ion_info_fd);
    if (rc) {
        ALOGE("ION import failed: %s\n", strerror(errno));
        exit(1);
    }

    /* mmap ion buffer to virtual address */
    vaddr = mmap(NULL, 0x1000,
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            ion_buf_fd, 0);
    printf("\n[parent] ion mem content: %s\n", (char *)vaddr);

    if (ion_buf_fd > 0) {
        struct ion_handle_data handle_data;
        memset(&handle_data, 0, sizeof(handle_data));
        handle_data.handle = ion_info_fd.handle;
        ioctl(main_ion_fd, ION_IOC_FREE, &handle_data);
        close(ion_buf_fd);
    }
    if (main_ion_fd > 0)
        close(main_ion_fd); 
}
