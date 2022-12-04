#include "PhotoSensor.h"



static void PhotoSensorAdcStructInit(PhotoSensorADC_t* PhotoSensorInitStruct);

//光传感器ADC采样变量
PhotoSensorADC_t PhotoSensorAdc;

//光传感器输入ADC初始化
void PhotoSensorAdcOpen(void)
{
    //初始化ADC结构体
    PhotoSensorAdcStructInit(&PhotoSensorAdc);
    //开启ADC DMA
    HAL_ADC_Start_DMA(PhotoSensorAdc.PhotoSensorAdc, PhotoSensorAdc.PhotoSensorAdcValue, PhotoSensorAdc.AdcBufSize);
}

static void PhotoSensorAdcStructInit(PhotoSensorADC_t* PhotoSensorInitStruct)
{
    //配置光传感器ADC结构体
    PhotoSensorInitStruct->PhotoSensorAdc = &PHOTOSENSOR_ADC;
    //配置ADC采样数组大小
    PhotoSensorInitStruct->AdcBufSize = PHOTOSENSOR_ADC_BUF_NUM;
    //初始化ADC采样数组
    for(int i = 0; i < PhotoSensorInitStruct->AdcBufSize; i++)
    {
        PhotoSensorInitStruct->PhotoSensorAdcValue[i] = 0;
    }
}

// /**
//  * @brief Get the Photo Sensor Adc Value object
//  * 
//  * @return 返回存放光传感器ADC数值数组的地址
//  */
// uint32_t* GetPhotoSensorAdcValue(void)
// {
//     return PhotoSensorAdcValue;
// }

//关闭光传感ADC数据采集
void PhotoSensorAdcClose(void)
{
    HAL_ADC_Stop_DMA(PhotoSensorAdc.PhotoSensorAdc);
}


PhotoSensorADC_t* GetPhotoSensorAdcStruct(void)
{
    return &PhotoSensorAdc;
}
