/**
 * @file ServoControl.h
 * @author yuanluochen
 * @brief 舵机控制文件
 * @version 0.1
 * @date 2022-12-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H
#include "tim.h"


//舵机定时器自动重装载寄存器数值
#define SERVO_TIM_ARR 20000

//舵机定时器
#define SERVO_TIM htim1
//舵机PWM输出通道
#define SERVO_1_CHANNEL TIM_CHANNEL_1
#define SERVO_2_CHANNEL TIM_CHANNEL_2
#define SERVO_3_CHANNEL TIM_CHANNEL_3

//舵机角度最大值
#define SERVO_ANGLE_MAX 180.0f
//舵机角度最小值
#define SERVO_ANGLE_MIN 0.0f

//舵机最小角度对应的脉冲宽度单位为ms
#define SERVO_PWM_MIN_PULSE_WIDTH 0.5f
//舵机最大角度对应的脉冲宽度单位为ms
#define SERVO_PWM_MAX_PULSE_WIDTH 20.0f

//舵机初始角度
#define SERVO_BEGIN_ANGLE 90

//舵机控制初始化函数，配置舵机初始状态，即初始角度，配置位置为ServoControl.h宏定义处
void ServoControlInit(void);

/**
 * @brief 舵机角度控制函数
 * 
 * @param ServoTim 控制舵机运动的时钟
 * @param ServoChannel 控制舵机运动时钟通道
 * @param angle 控制舵机角度数值
 */
void ServoControl(TIM_HandleTypeDef* ServoTim, uint32_t ServoChannel, uint16_t ServoAngle);



#endif


