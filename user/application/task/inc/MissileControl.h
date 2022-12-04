/**
 * @file MissileControl.h
 * @author yuanluochen
 * @brief 飞镖控制文件，控制飞镖弹体运行，主要任务为分析飞镖光线传感器数据，控制舵机角度，进而调整飞镖弹体姿态。
 * @version 0.1
 * @date 2022-12-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MISSILE_CONTROL_H
#define MISSILE_CONTROL_H

#include "ServoControl.h"
#include "AdcDataHandle.h"

//飞镖控制整体结构体
typedef struct 
{
    
    float32_t MissilePhotoSensor1Val;
}MissileControl_t;


/**
 * @brief 飞镖弹体控制函数，控制飞镖弹体运行。
 * 
 */
void MissileControl(void);

#endif // !MISSILE_CONTROL_H
