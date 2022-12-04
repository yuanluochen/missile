#include "AdcDataHandle.h"


static void PhotoSensorFourierTransform(void);
static void AdcDataToFourierData(AdcData_t* AdcData, uint16_t AdcNum,
                                FourierData_t* FourierData_1, uint16_t FourierNum_1,
                                FourierData_t* FourierData_2, uint16_t FourierNum_2,
                                FourierData_t* FourierData_3, uint16_t FourierNum_3);

// static void DeleteDCSignale(FourierData_t* Fourier, uint16_t FourierBufNum);
static void AdcFilterStructInit(AdcFilterValue_t* AdcFilterStructInit);
static void GetSpecificValue(AdcFilterValue_t* PhotoSensorValue);


AdcFilterValue_t AdcfilterValue;

AdcFilterValue_t* GetAdcFilterValue(void)
{
    return &AdcfilterValue;
}

// //光传感器Fourier transform信号提取 
static void PhotoSensorFourierTransform(void)
{
    //初始化ADC信号过滤结构体
    AdcFilterStructInit(&AdcfilterValue);

    //提取ADC采样结果，将ADC采样结果放于Fourier transform buf内
    AdcDataToFourierData(AdcfilterValue.AdcStruct->PhotoSensorAdcValue, AdcfilterValue.AdcStruct->AdcBufSize,
                        AdcfilterValue.PhotoSensorFFT_1.PhotoSensorFFTInputValue, AdcfilterValue.PhotoSensorFFT_1.PhotoSensorFFTInputSize,
                        AdcfilterValue.PhotoSensorFFT_2.PhotoSensorFFTInputValue, AdcfilterValue.PhotoSensorFFT_2.PhotoSensorFFTInputSize,
                        AdcfilterValue.PhotoSensorFFT_3.PhotoSensorFFTInputValue, AdcfilterValue.PhotoSensorFFT_3.PhotoSensorFFTInputSize);
    //FFT变换结构体初始化
    arm_cfft_radix4_init_f32(&AdcfilterValue.PhotoSensorFFT_1.PhotoSensorFFT, FOURIER_TRANSFORM_POINT, ForwardTransform, EnabeBitReverse);
    arm_cfft_radix4_init_f32(&AdcfilterValue.PhotoSensorFFT_2.PhotoSensorFFT, FOURIER_TRANSFORM_POINT, ForwardTransform, EnabeBitReverse);
    arm_cfft_radix4_init_f32(&AdcfilterValue.PhotoSensorFFT_3.PhotoSensorFFT, FOURIER_TRANSFORM_POINT, ForwardTransform, EnabeBitReverse);
    //FFT计算
    arm_cfft_radix4_f32(&AdcfilterValue.PhotoSensorFFT_1.PhotoSensorFFT, AdcfilterValue.PhotoSensorFFT_1.PhotoSensorFFTInputValue);
    arm_cfft_radix4_f32(&AdcfilterValue.PhotoSensorFFT_2.PhotoSensorFFT, AdcfilterValue.PhotoSensorFFT_2.PhotoSensorFFTInputValue);
    arm_cfft_radix4_f32(&AdcfilterValue.PhotoSensorFFT_3.PhotoSensorFFT, AdcfilterValue.PhotoSensorFFT_3.PhotoSensorFFTInputValue);
    
    //计算信号幅度谱
    arm_cmplx_mag_f32(AdcfilterValue.PhotoSensorFFT_1.PhotoSensorFFTInputValue, AdcfilterValue.PhotoSensorFFT_1.PhotoSensorFFTOutputValue, FOURIER_TRANSFORM_POINT);
    arm_cmplx_mag_f32(AdcfilterValue.PhotoSensorFFT_2.PhotoSensorFFTInputValue, AdcfilterValue.PhotoSensorFFT_2.PhotoSensorFFTOutputValue, FOURIER_TRANSFORM_POINT);
    arm_cmplx_mag_f32(AdcfilterValue.PhotoSensorFFT_3.PhotoSensorFFTInputValue, AdcfilterValue.PhotoSensorFFT_3.PhotoSensorFFTOutputValue, FOURIER_TRANSFORM_POINT); 

    GetSpecificValue(&AdcfilterValue);
} 

//判断ADC转运结束，开启FFT进行滤波处理
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
    //完成一次测量关闭DMA数据转运
    HAL_ADC_Stop_DMA(&PHOTOSENSOR_ADC);
    //处理ADC采样数据，对采样数据进行信号处理，提取特定频段数据
    PhotoSensorFourierTransform();
    //开启ADC DMA转运
    PhotoSensorAdcOpen();    

}

//ADC信号过滤结构体初始化
static void AdcFilterStructInit(AdcFilterValue_t* AdcFilterStructInit)
{
    //获取ADC原数组
    AdcFilterStructInit->AdcStruct = GetPhotoSensorAdcStruct();
    //配置Fourier transform buf size
    AdcFilterStructInit->PhotoSensorFFT_1.PhotoSensorFFTInputSize = FOURIER_INPUT_BUF_NUM;
    AdcFilterStructInit->PhotoSensorFFT_2.PhotoSensorFFTInputSize = FOURIER_INPUT_BUF_NUM;
    AdcFilterStructInit->PhotoSensorFFT_3.PhotoSensorFFTInputSize = FOURIER_INPUT_BUF_NUM;

    AdcFilterStructInit->PhotoSensorFFT_1.PhotoSensorFFTOutputSize = FOURIER_OUPUT_BUF_NUM;
    AdcFilterStructInit->PhotoSensorFFT_2.PhotoSensorFFTOutputSize = FOURIER_OUPUT_BUF_NUM;
    AdcFilterStructInit->PhotoSensorFFT_3.PhotoSensorFFTOutputSize = FOURIER_OUPUT_BUF_NUM;
    //配置FFT提取元素
    AdcFilterStructInit->AdcSampleNumMin = ADC_SAMPLE_MIN_NUM;
    AdcFilterStructInit->AdcSampleNumMax = ADC_SAMPLE_MAX_NUM;
}

/**
 * @brief ADC数值提取，将ADC数值赋于Fouier transform buf内，赋值顺序为FourierData_1 -> FourierData_2 -> FourierData_3
 * 
 * @param AdcData ADC采样buf
 * @param AdcNum ADC采样数值大小
 * @param FourierData_1 Fourier赋值数组一，
 * @param FourierNum_1 Fourier赋值数组一大小
 * @param FourierData_2 Fourier赋值数组二
 * @param FourierNum_2 Fourier赋值数值二大小
 * @param FourierData_3 Fourier赋值数组三
 * @param FourierNum_3 Fourier赋值数组三大小
 */
static void AdcDataToFourierData(AdcData_t* AdcData, uint16_t AdcNum,
                                FourierData_t* FourierData_1, uint16_t FourierNum_1,
                                FourierData_t* FourierData_2, uint16_t FourierNum_2,
                                FourierData_t* FourierData_3, uint16_t FourierNum_3)
{
    int i = 0;//总循环用

    int FourierCount_1 = 0;//Fourier计数数组1
    int FourierCount_2 = 0;//Fourier计数数组2
    int FourierCount_3 = 0;//Fourier计数数组3

    for(i = 0; i < AdcNum; i++)
    {
        //赋值fouier 数组1
        if(i % 3 == 0)
        {
            //判断是否越界
            if(FourierCount_1 >= FourierNum_1)
            {
                return;
            }

            FourierData_1[FourierCount_1++] = AdcData[i];//实部赋ADC数值
            FourierData_1[FourierCount_1++] = 0;//虚部赋零
        }
        //赋值fourier 数组2
        else if(i % 3 == 1)
        {
            //判断是否越界
            if(FourierCount_2 >= FourierNum_2)
            {
                return;
            }
            FourierData_2[FourierCount_2++] = AdcData[i];
            FourierData_2[FourierCount_2++] = 0;
        }
        //赋值fourier 数组3
        else if(i % 3 == 2)
        {
            //判读是否越界
            if(FourierCount_3 >= FourierNum_3)
            {
                return;
            }
            FourierData_3[FourierCount_3++] = AdcData[i];
            FourierData_3[FourierCount_3++] = 0;
        }
    }
}

// /**
//  * @brief 删除ADC采样中的直流信号
//  * 
//  * @param FourierData 存放fourier transform数值的内存空间
//  * @param FourierBufNum Fourier transform 数组元素数量
//  */
// static void DeleteDCSignale(FourierData_t* Fourier, uint16_t FourierBufNum)
// {
//     FourierData_t average = 0;
// 		FourierData_t t = 0;//
// 		FourierData_t temp = 0;
//     uint16_t i = 0;
//     for(i = 0; i < FourierBufNum; i++)
//     {
//         if(i == 0)
//         {
//             average = Fourier[2 * i];
//         }
//         else
//         {
//             t = i + 1;
//             temp = average;
//             average = ((t - 1) / t) * temp + (1 / t) * Fourier[2 * i];
//         }
//     }
//     //去直流分量
//     for(i = 0; i < FourierBufNum; i++)
//     {
//         Fourier[2 * i] -= average;
//     }
// }

/**
 * @brief 提取特定频段的数值
 * 
 * @param PhotoSensorValue ADC数组
 */
static void GetSpecificValue(AdcFilterValue_t* PhotoSensorValue)
{
    int i = 0;
    //数据重置
    PhotoSensorValue->PhotoSensorFFT_1.PhotoSensorValue = 0;
    PhotoSensorValue->PhotoSensorFFT_2.PhotoSensorValue = 0;
    PhotoSensorValue->PhotoSensorFFT_3.PhotoSensorValue = 0;
    //求其均值
    for(i = PhotoSensorValue->AdcSampleNumMin; i <= PhotoSensorValue->AdcSampleNumMax;i++)
    {
        PhotoSensorValue->PhotoSensorFFT_1.PhotoSensorValue += PhotoSensorValue->PhotoSensorFFT_1.PhotoSensorFFTOutputValue[i];
        PhotoSensorValue->PhotoSensorFFT_2.PhotoSensorValue += PhotoSensorValue->PhotoSensorFFT_2.PhotoSensorFFTOutputValue[i];
        PhotoSensorValue->PhotoSensorFFT_3.PhotoSensorValue += PhotoSensorValue->PhotoSensorFFT_3.PhotoSensorFFTOutputValue[i];
    }
    
}
