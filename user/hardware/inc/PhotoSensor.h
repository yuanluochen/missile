/**
 * @file photosensor.h
 * @author yuanluochen
 * @brief 光传感器驱动文件，配置ADC对硅光电池输出电压进行数据读取,以获取特殊频率光线强度
 * @version 0.1
 * @date 2022-11-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef PHOTOSENSOR_H
#define PHOTOSENSOR_H

#include "stm32f4xx_hal.h"
#include "adc.h"
#include "arm_math.h"


//ADC采样数据类型
typedef uint32_t AdcData_t;

//光传感器输入 ADC
#define PHOTOSENSOR_ADC hadc1
//ADC通道总数
#define PHOTOSENSOR_ADC_CHANNEL_NUM 3
//Fourier Transform 点数，更改该宏定义请更改AdcDataHandle.h内的同名宏定义
#define FOURIER_TRANSFORM_POINT 256

//存放ADC数值内存空间大小
#define PHOTOSENSOR_ADC_BUF_NUM (PHOTOSENSOR_ADC_CHANNEL_NUM * FOURIER_TRANSFORM_POINT)
//ADC通道,通道值为地址偏差值，该通道数为ADC实际通道数
typedef enum
{
    PhotoSensorADCChannel_2 = 0,//ADC 通道2，即程序ADC通道1
    PhotoSensorADCChannel_3 = 1,//ADC 通道3，即程序ADC通道2
    PhotoSensorADCChannel_4 = 2,//ADC 通道4，即程序ADC通道3
}ADCChannel_t;

//光传感器ADC采样结构体
typedef struct 
{
    //ADC结构体地址
    ADC_HandleTypeDef* PhotoSensorAdc;

    //光传感器ADC采样数组大小
    uint16_t AdcBufSize;
    //光传感器ADC采样数值存放位置
    AdcData_t PhotoSensorAdcValue[PHOTOSENSOR_ADC_BUF_NUM];
}PhotoSensorADC_t;


//光传感器输入ADC初始化
void PhotoSensorAdcOpen(void);

// /**
//  * @brief Get the Photo Sensor Adc Value object
//  * 
//  * @return 返回存放光传感器ADC数值数组的地址，该ADC数值空间大小为3 * uint32_t
//  */
// uint32_t* GetPhotoSensorAdcValue(void);

//关闭光传感ADC数据采集
void PhotoSensorAdcClose(void);

//返回光传感器ADC采样结构体地址，内部存储ADC采样数据
PhotoSensorADC_t* GetPhotoSensorAdcStruct(void);

#endif 
