#ifndef __AS5600_H
#define __AS5600_H

#include "main.h"
#include "i2c.h"

#define AS5600_SLAVE_ADDRESS			0x36
#define AS5600_SHIFTED_SLAVE_ADDRESS	0x6c//(for create spacing for R/W bit)
#define AS5600_I2C_TIMEOUT_DEFAULT		10	// can be choosen from (1 - 30) ms
#define I2C_MEMADD_SIZE_8BIT            0x00000001U //from HAL library

//datasheet : https://ams.com/documents/20143/36005/AS5600_DS000365_5-00.pdf

/* AS5600 configuration registers */
#define AS5600_REGISTER_ZMCO			0x00
#define AS5600_REGISTER_ZPOS_HIGH		0x01
#define AS5600_REGISTER_ZPOS_LOW		0x02
#define AS5600_REGISTER_MPOS_HIGH		0x03
#define AS5600_REGISTER_MPOS_LOW		0x04
#define AS5600_REGISTER_MANG_HIGH		0x05
#define AS5600_REGISTER_MANG_LOW		0x06
#define AS5600_REGISTER_CONF_HIGH		0x07
#define AS5600_REGISTER_CONF_LOW		0x08
/* AS5600 output registers */
#define AS5600_REGISTER_RAW_ANGLE_HIGH	0x0C
#define AS5600_REGISTER_RAW_ANGLE_LOW	0x0D
#define AS5600_REGISTER_ANGLE_HIGH		0x0E
#define AS5600_REGISTER_ANGLE_LOW		0x0F
/* AS5600 status registers */
#define AS5600_REGISTER_STATUS			0x0B
#define AS5600_REGISTER_AGC				0x1A
#define AS5600_REGISTER_MAGNITUDE_HIGH	0x1B
#define AS5600_REGISTER_MAGNITUDE_LOW	0x1C
#define AS5600_REGISTER_BURN			0xFF

//Ignoring power setting register , hysteresis register , pwm , slow filte , fast filter ,watchdog setting

/* AS5600 direction definitions */
#define AS5600_DIR_CW					1
#define AS5600_DIR_CCW					2

/* AS5600 bit mask */
#define AS5600_12_BIT_MASK				(uint16_t)4095 // 4096 resolution
/* AS5600 angle conversions */
#define AS5600_DEG_CONV 8.7890625e-2    /* 360/4096 */
#define AS5600_RAD_CONV 1.5339808e-3    /* 2pi/4096 */

#define I2C_TIMEOUT_BASE 10
#define I2C_TIMEOUT_BYTE 1

#define ZMCO		0x00
#define ZPOS_H		0x01
#define ZPOS_L		0x02
#define MPOS_H		0x03
#define MPOS_L		0x04
#define MANG_H		0x05
#define MANG_L		0x06
#define CONF_L		0x07
#define CONF_H		0x08
#define RAWANG_L	0x0D
#define ANGLE_H		0x0E
#define ANGLE_L		0x0F

#define BURN		0xFF

#define AS5600_RESOLUTION 4096//12bit/lines resolution
#define RAWANG_H 0X0C

uint16_t as5600GetRawAngle(void);

#endif

