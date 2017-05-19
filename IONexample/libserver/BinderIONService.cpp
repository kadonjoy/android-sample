#define LOG_TAG "BinderIONService"
#include "BinderIONService.h"
#include <linux/msm_ion.h>

using namespace android;

BinderIONService::BinderIONService() : 
    value(0),
    main_ion_fd(-1),
    ion_buf_fd(-1)
{

}

BinderIONService::~BinderIONService() {
    stop();
}

void BinderIONService::start() {
	ALOGE("BinderIONService::start");
    FUNC_NAME(__FUNCTION__)
    initializeION();
}

void BinderIONService::stop() {
    FUNC_NAME(__FUNCTION__)
    destroyION();
}

void BinderIONService::initializeION()
{
    FUNC_NAME(__FUNCTION__)
    int fd, rc;
    struct ion_handle_data handle_data;
    struct ion_allocation_data alloc;
    struct ion_fd_data ion_info_fd;
    void * vaddr = NULL;
    /* opening ion dev */
    main_ion_fd = open(ION_DEV_NAME, O_RDONLY);
    if (main_ion_fd < 0) {
        ALOGE("Ion dev open failed: %s\n", strerror(errno));
        close(main_ion_fd);
    }

    /* allocate ion buffer */
    memset(&alloc, 0, sizeof(alloc));
    alloc.len = 0x1000;
    alloc.len = (alloc.len + 4095U) & (~4095U);
    alloc.align = 4096;
    alloc.heap_id_mask = ION_HEAP(ION_SYSTEM_HEAP_ID);
    alloc.flags = ION_FLAG_CACHED;

    rc = ioctl(main_ion_fd, ION_IOC_ALLOC, &alloc);
    if (rc < 0) {
        ALOGE("ION allocation failed: %s\n", strerror(errno));
        close(main_ion_fd);
    }

    /* share ion buffer */
    memset(&ion_info_fd, 0, sizeof(ion_info_fd));
    ion_info_fd.handle = alloc.handle;
    memset(&handle_data, 0, sizeof(handle_data));
    handle_data.handle = ion_info_fd.handle;
    rc = ioctl(main_ion_fd, ION_IOC_SHARE, &ion_info_fd);
    if (rc < 0) {
        ALOGE("ION share failed %s\n", strerror(errno));
        ioctl(main_ion_fd, ION_IOC_FREE, &handle_data);
    }

    /* mmap ion buffer to virtual address */
    vaddr = mmap(NULL, alloc.len,
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            ion_info_fd.fd, 0);
    memcpy((void *)vaddr, "xiamachit", 10);

    ioctl(main_ion_fd, ION_IOC_FREE, &handle_data);
    ion_buf_fd = ion_info_fd.fd;

    ALOGE("\n[child] ion mem content: %s\n", (char *)vaddr);
    ALOGE("\n[child] new ion buffer handle: 0x%x, old: 0x%x, fd: %d main_ion_fd: %d\n",
            ion_info_fd.handle,
            alloc.handle,
            ion_buf_fd,
            main_ion_fd);
}

void BinderIONService::destroyION()
{
    FUNC_NAME(__FUNCTION__)

    if (ion_buf_fd > 0)
        close(ion_buf_fd);
    if (main_ion_fd > 0) {
        close(main_ion_fd);
    }
}

void BinderIONService::setVal(int32_t val) {
	value = val;
	ALOGE("BinderIONService value is %d\n", value);
}

int32_t BinderIONService::getVal() {
	ALOGE("BinderIONService::getVal");
    FUNC_NAME(__FUNCTION__)
	return value;
}
