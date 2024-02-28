//
// Created by pc on 2024/2/13.
//
#include "drv8323.h"

void DRV8323_Init() {

//    使能DRV8323的PWM输入，在拉高EN_GATE引脚后需要至少延时5-10ms，此时使能SPI片选引脚才有效
    HAL_GPIO_WritePin(PWM_EN_GPIO_Port, PWM_EN_Pin, GPIO_PIN_SET);
}