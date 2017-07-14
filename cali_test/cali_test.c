/*
 **************************************************************************************
 *       Filename:  cali_test.c
 *    Description:  cali_test source file
 *
 *        Version:  1.0
 *        Created:  2017-07-10 18:35:20
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <cutils/properties.h>
#include <unistd.h>
#include <seninf_reg.h>
#include <mtkcam/drv_common/isp_reg.h>
#include "cali_test.h"
#include "kd_imgsensor.h"

/*const char dump_name[] = "/sys/class/sensordrv/kd_camera_hw/calibration_dump";*/
/*const char save_name[] = "/sys/class/sensordrv/kd_camera_hw/calibration_save";*/
const char filename[] = "/sys/class/sensordrv/kd_camera_hw/calibration_dump";
#define SENSOR_DEV_NAME     "/dev/kd_camera_hw"
#define ISP_DEV_NAME         "/dev/camera-isp"
#define CAM_ISP_RANGE    0xA000
#define ISP_BASE_HW      0x15000000
#define SENINF_BASE_RANGE   0x4000
#define SENINF_BASE_HW   0x15008000
#define CAM_GPIO_RANGE   0x1000
#define GPIO_BASE_HW     0x10211000

int mfd, m_fdSensor;
unsigned int * mpIspHwRegAddr, * mpSeninfHwRegAddr, * mpGpioHwRegAddr, * mpCAMIODrvRegAddr;
int initialize(void)
{
    // Open isp driver
    mfd = open(ISP_DEV_NAME, O_RDWR);
    if (mfd < 0) {
        printf("error open kernel driver, %d, %s\n", errno, strerror(errno));
        return -1;
    }
    //Open sensor driver
    m_fdSensor = open(SENSOR_DEV_NAME, O_RDWR);
    if (m_fdSensor < 0) {
        printf("[init]: error opening  %s \n",  strerror(errno));
        return -13;
    }

    mpIspHwRegAddr = (unsigned int *) mmap(0, CAM_ISP_RANGE, (PROT_READ | PROT_WRITE), MAP_SHARED, mfd, ISP_BASE_HW);
    if (mpIspHwRegAddr == MAP_FAILED) {
        printf("mmap err(1), %d, %s \n", errno, strerror(errno));
        return -4;
    }

    mpSeninfHwRegAddr = (unsigned int *) mmap(0, SENINF_BASE_RANGE, (PROT_READ|PROT_WRITE), MAP_SHARED, mfd, SENINF_BASE_HW);
    if (mpSeninfHwRegAddr == MAP_FAILED) {
        printf("mmap err(1), %d, %s \n", errno, strerror(errno));
        return -5;
    }
    mpGpioHwRegAddr = (unsigned int *) mmap(0, CAM_GPIO_RANGE, (PROT_READ|PROT_WRITE), MAP_SHARED, mfd, GPIO_BASE_HW);
    if (mpGpioHwRegAddr == MAP_FAILED) {
        printf("mmap err(6), %d, %s \n", errno, strerror(errno));
        return -10;
    }

    mpCAMIODrvRegAddr = mpGpioHwRegAddr + (0x970 / 4);

    seninf_reg_t *pSeninf = (seninf_reg_t *)mpSeninfHwRegAddr;
    SENINF_BITS(pSeninf, SENINF1_MUX_CTRL, SENINF_MUX_EN) = 1;
    SENINF_BITS(pSeninf, SENINF2_MUX_CTRL, SENINF_MUX_EN) = 1;
    SENINF_BITS(pSeninf, SENINF3_MUX_CTRL, SENINF_MUX_EN) = 1;
    SENINF_BITS(pSeninf, SENINF4_MUX_CTRL, SENINF_MUX_EN) = 1;
    SENINF_BITS(pSeninf, SENINF1_CTRL, SENINF_EN) = 1;
    SENINF_BITS(pSeninf, SENINF2_CTRL, SENINF_EN) = 1;
    SENINF_BITS(pSeninf, SENINF3_CTRL, SENINF_EN) = 1;

    return 1;
}

void uninitialize(void)
{
    if ( 0 != mpIspHwRegAddr ) {
        if(munmap(mpIspHwRegAddr, CAM_ISP_RANGE)!=0){
            printf("mpIspHwRegAddr munmap err, %d, %s \n", errno, strerror(errno));
        }
        mpIspHwRegAddr = NULL;
    }

    if(munmap(mpSeninfHwRegAddr, SENINF_BASE_RANGE)!=0) {
        printf("mpSeninfHwRegAddr munmap err, %d, %s \n", errno, strerror(errno));
    }
    mpSeninfHwRegAddr = NULL;

    if(munmap(mpGpioHwRegAddr, CAM_GPIO_RANGE)!=0) {
        printf("mpGpioHwRegAddr munmap err, %d, %s \n", errno, strerror(errno));
    }
    mpGpioHwRegAddr = NULL;

    close(mfd);
    close(m_fdSensor);
}

int setMclk(unsigned long pcEn, unsigned long mclkSel,
            unsigned long clkCnt, unsigned long clkPol,
            unsigned long clkFallEdge, unsigned long clkRiseEdge,
            unsigned long padPclkInv    )
{
    int ret = 0;
    isp_reg_t *pisp = (isp_reg_t *) mpIspHwRegAddr;
    seninf_reg_t *pSeninf = (seninf_reg_t *)mpSeninfHwRegAddr;
    ACDK_SENSOR_MCLK_STRUCT sensorMclk;
    if(pcEn == 1) {
        sensorMclk.on = 1;
        sensorMclk.freq = 1;
        ret = ioctl(m_fdSensor, KDIMGSENSORIOC_X_SET_MCLK_PLL,&sensorMclk);
        if (ret < 0) {
            printf("ERROR:KDIMGSENSORIOC_X_SET_MCLK_PLL\n");
        }
    }
    clkRiseEdge = 0;
    clkFallEdge = (clkCnt > 1)? (clkCnt+1)>>1 : 1;//avoid setting larger than clkCnt

    SENINF_BITS(pSeninf, SENINF_TG1_PH_CNT, PCEN) = pcEn;
    //Seninf Top pclk clear gating
    SENINF_BITS(pSeninf, SENINF_TOP_CTRL, SENINF1_PCLK_EN) = 1;
    SENINF_BITS(pSeninf, SENINF_TOP_CTRL, SENINF2_PCLK_EN) = 1;

    SENINF_BITS(pSeninf, SENINF_TG1_SEN_CK, CLKRS) = clkRiseEdge;
    SENINF_BITS(pSeninf, SENINF_TG1_SEN_CK, CLKFL) = clkFallEdge;
    SENINF_BITS(pSeninf, SENINF_TG1_SEN_CK, CLKCNT) = clkCnt;

    SENINF_BITS(pSeninf, SENINF_TG1_PH_CNT, CLKFL_POL) = (clkCnt & 0x1) ? 0 : 1;
    SENINF_BITS(pSeninf, SENINF_TG1_PH_CNT, CLKPOL) = clkPol;

    SENINF_BITS(pSeninf, SENINF_TG1_PH_CNT, TGCLK_SEL) = 1;//force PLL due to ISP engine clock dynamic spread
    //SENINF_BITS(pSeninf, SENINF_TG1_PH_CNT, ADCLK_EN) = 1;//FPGA experiment
    SENINF_BITS(pSeninf, SENINF_TG1_PH_CNT, PAD_PCLK_INV) = padPclkInv;
    usleep(200);

    if(mpCAMIODrvRegAddr != NULL) {
        *(mpCAMIODrvRegAddr) &= ~0x60;
        *(mpCAMIODrvRegAddr) |= (3<<5); // [6:4] = 0:2mA, 1:4mA, 2:6mA, 3:8mA
        printf("[setIODrivingCurrent]: 0x%08x\n", (int) (*(mpCAMIODrvRegAddr)));
    }

    return ret;
}

int main(int argc, char** argv)
{
    int input = 1;
    int fd = 0;

    initialize();
    setMclk(1, 1, 1, 0, 1, 0, 0);
    fd = open(filename, O_RDWR);
    /*fd = open(argv[1], O_RDWR);*/
    if (fd < 0) {
        printf("Fail to open %s\n", filename);
    }
    if(-1 == write(fd, (const void *)&input, 1)) {
        printf("Fail to wirte\n");
    }
    /*if (argc == 1) {*/
        /*if (property_set("sys.dualcamera.cali.dump", "1")) {*/
            /*printf("Fail to set sys.dualcamera.cali.dump!\n");*/
        /*}*/
    /*} else {*/
        if (property_set("sys.dualcamera.cali.save", "1")) {
            printf("Fail to set sys.dualcamera.cali.save!\n");
        }
    /*}*/

    sleep(10);
    uninitialize();
    close(fd);

    return 1;
}




/********************************** END **********************************************/

