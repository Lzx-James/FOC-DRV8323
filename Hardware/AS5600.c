#include "AS5600.h"

float degree_ang = 0;
uint8_t  deltaT;
int ptime ;

#define AS5600_RAW_ADDR 0X36
#define AS5600_ADDR (AS5600_RAW_ADDR << 1)

float raw_ang = 0 ;

float actual_angle = 0 ;
uint8_t AS5600_buffer[2] = {0};



static int i2cWrite(uint8_t dev_addr, uint8_t *pData, uint32_t count) {
    int status;


    status = HAL_I2C_Master_Transmit(&hi2c1, dev_addr, pData, count, 100);
    return status;
}
static int i2cRead(uint8_t dev_addr, uint8_t *pData, uint32_t count) {
    int status;

    status =  HAL_I2C_Master_Receive(&hi2c1, (dev_addr | 1), pData, count, 100);
    return status;
}



uint16_t as5600GetRawAngle(void) {
    uint16_t raw_angle;

    uint8_t raw_angle_reg = RAWANG_H;

    i2cWrite(AS5600_ADDR, &raw_angle_reg, 1);

    i2cRead(AS5600_ADDR, AS5600_buffer, 2);

    raw_angle = (((uint16_t) AS5600_buffer[0] << 8) | (uint16_t) AS5600_buffer[1]);
//    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    return raw_angle;

}

//uint32_t CurrentMicro(void)
//{
//  uint32_t m0 = HAL_GetTick();
//  uint32_t u0 = SysTick->LOAD - SysTick->VAL;
//  uint32_t m1 = HAL_GetTick();
//  uint32_t u1 = SysTick->LOAD - SysTick->VAL;

//  if (m1 > m0) {
//    return ( m1 * 1000 + (u1 * 1000) / SysTick->LOAD);
//  } else {
//    return ( m0 * 1000 + (u0 * 1000) / SysTick->LOAD);
//  }
//}
