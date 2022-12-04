#include "ServoControl.h"

void ServoControlInit(void)
{
    //开启舵机PWM输出通道
    HAL_TIM_PWM_Start(&SERVO_TIM, SERVO_1_CHANNEL);
    HAL_TIM_PWM_Start(&SERVO_TIM, SERVO_2_CHANNEL);
    HAL_TIM_PWM_Start(&SERVO_TIM, SERVO_3_CHANNEL);
    //设置舵机初始化状态.配置初始角度
    ServoControl(&SERVO_TIM, SERVO_1_CHANNEL, SERVO_BEGIN_ANGLE);
    ServoControl(&SERVO_TIM, SERVO_2_CHANNEL, SERVO_BEGIN_ANGLE);
    ServoControl(&SERVO_TIM, SERVO_3_CHANNEL, SERVO_BEGIN_ANGLE);

}


void ServoControl(TIM_HandleTypeDef* ServoTim, uint32_t ServoChannel, uint16_t ServoAngle)
{
    //舵机控制时钟CCR寄存器数值
    uint16_t ServoTimCompare = 0;
    //控制舵机角度，若超过最大值则舵机角度等于最大值，若小于最小值，则舵机角度，等于最小值。
    if(ServoAngle >= SERVO_ANGLE_MAX)
    {
        ServoAngle = SERVO_ANGLE_MAX;
    }
    else if (ServoAngle <= SERVO_ANGLE_MIN)
    {
        ServoAngle = SERVO_ANGLE_MIN;
    }

    //舵机零度时的CCR数值
    uint16_t ServoTIMBeginCompare = (uint16_t)(SERVO_TIM_ARR * SERVO_PWM_MIN_PULSE_WIDTH) / SERVO_PWM_MAX_PULSE_WIDTH;
    //舵机运动CCR数值
    ServoTimCompare = ServoTIMBeginCompare + (uint16_t)((SERVO_TIM_ARR / (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN)) * ServoAngle);
    //配置CCR数值
    __HAL_TIM_SET_COMPARE(ServoTim, ServoChannel, ServoTimCompare);
}
