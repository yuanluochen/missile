# 飞镖弹体

## 工程简介

飞镖弹体制导工程，采用激光实现飞镖制导，精准命中目标，软件由yuanluochen负责，工程开发主要思路参考一位B站UP主的激光制导方案。但与该UP主方案稍有不同，原方案设计硬件带通滤波器，对采样激光信号进行处理，但是由于在下能力有限，不理解硬件设计，所以我在软件层面上对传感器采样信号进行处理，提取特定频段的能量，实现在软件层面上的提取信号的操作。

## 硬件设备

开发板stm32f401ccu6，

## 文件简介

工程文件下user目录为在本工程在下编写文件的主要部分。里面包含了信号处理，各种外设驱动文件，以及总体工程主要任务文件。一下为各文件目录的详细介绍

### Hardware

主要存放飞镖外设驱动文件。包括光敏传感器采样，舵机控制，涵道控制文件

#### 光敏传感器采样(Photosensor)

读取光敏传感器采样数值，通过ADC采样数据，DMA转运，通过配置定时器触发ADC采样来配置ADC采样频率。  
光敏传感器采样ADC为ADC1，配置3路ADC输入采样三路光敏传感器数值。  
三路ADC输入为  
IN1 -> PA1  
IN2 -> PA2  
IN3 -> PA3

配置TIM4来触发ADC采样，TIM4触发频率为40KHz，即ADC采样频率为40KHz

#### application

存放飞镖数据处理部分文件

## ADC采样数据处理(AdcDataHandle)

FFT点数 256 点  
FFT分辨率 = 采样频率 / FFT点数 = 40K Hz / 256
