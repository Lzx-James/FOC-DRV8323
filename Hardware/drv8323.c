//
// Created by pc on 2024/2/13.
//
#include "drv8323.h"

void DRV8323_Init() {

//    ʹ��DRV8323��PWM���룬������EN_GATE���ź���Ҫ������ʱ5-10ms����ʱʹ��SPIƬѡ���Ų���Ч
    HAL_GPIO_WritePin(PWM_EN_GPIO_Port, PWM_EN_Pin, GPIO_PIN_SET);
}