/*
 **************************************************************************************
 *       Filename:  cali_test.h
 *    Description:   header file
 *
 *        Version:  1.0
 *        Created:  2017-07-13 15:19:52
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#ifndef CALI_TEST_H_INCLUDED
#define CALI_TEST_H_INCLUDED
typedef unsigned char MUINT8;
typedef enum {                                                                                                                   
    MCLK_48MHZ_GROUP = 0x1,
    MCLK_52MHZ_GROUP = 0x2,
} ACKD_SENSOR_MCLK_ENUM;
typedef struct {                                                                                                                 
    MUINT8 on;                                                                                                                   
    ACKD_SENSOR_MCLK_ENUM freq;                                                                                                  
} ACDK_SENSOR_MCLK_STRUCT; 







#endif /*CALI_TEST_H_INCLUDED*/

/********************************** END **********************************************/

