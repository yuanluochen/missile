/**
 * @file AdcDataHandle.h
 * @author yuanluochn
 * @brief 提取ADC数据，对adc数据进行FFT，获取频域谱,并进行数据提取操作
 * @version 0.1
 * @date 2022-11-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef ADCDATAHANDLE_H
#define ADCDATAHANDLE_H

#include "PhotoSensor.h"
#include "arm_const_structs.h"
#include "arm_math.h"

//Fourier变换数据类型
typedef float32_t FourierData_t;


//Fourier transform 点数，即进行一次傅里叶变换输入数值个数,(注意更改该宏定义请同时更改photosensor.h内的同名宏定义)
#define FOURIER_TRANSFORM_POINT 256
//存放Fourier数组内存空间大小
#define FOURIER_INPUT_BUF_NUM (2 * FOURIER_TRANSFORM_POINT)
//FFT输出数组大小
#define FOURIER_OUPUT_BUF_NUM FOURIER_TRANSFORM_POINT


//采样最小数组元素
#define ADC_SAMPLE_MIN_NUM 110
//采样最大数组元素
#define ADC_SAMPLE_MAX_NUM 122

//FFT类型
typedef enum
{
    ForwardTransform = 0,//正变换
    InverseTransform = 1, //逆变换
}FFTtype;
//FFT是否进行位翻转
typedef enum
{
    DisableBitReverse = 0,
    EnabeBitReverse = 1,
}FFTBitReverse;

//光传感器FFT结构体
typedef struct 
{
    //FFT函数结构体
    arm_cfft_radix4_instance_f32 PhotoSensorFFT;

    //FFT输入数组大小
    uint16_t PhotoSensorFFTInputSize;
    //FFT输出数组大小
    uint16_t PhotoSensorFFTOutputSize;
    //光传感器FFT输入数组
    FourierData_t PhotoSensorFFTInputValue[FOURIER_INPUT_BUF_NUM];
    //光传感器FFT输出数组
    FourierData_t PhotoSensorFFTOutputValue[FOURIER_OUPUT_BUF_NUM];
    //光传感器特定频段数组
    FourierData_t PhotoSensorValue;
}PhotoSensorFFT_t;

//ADC过滤后数值结构体
typedef struct R
{
    //ADC原数组结构体
    PhotoSensorADC_t* AdcStruct;

    //光传感器FFT变量
    PhotoSensorFFT_t PhotoSensorFFT_1;
    PhotoSensorFFT_t PhotoSensorFFT_2;
    PhotoSensorFFT_t PhotoSensorFFT_3;

    //提取元素
    uint16_t AdcSampleNumMax;
    uint16_t AdcSampleNumMin;
}AdcFilterValue_t;


//获取取光传感器ADC结构体
AdcFilterValue_t* GetAdcFilterValue(void);

#endif
