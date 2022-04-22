#ifndef _HAL_MPU6050_H
#define _HAL_MPU6050_H

#include "hardware.h"

void Hal_MPU_IIC_Init(void );
uint8_t Hal_MPU_IIC_Write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t length );
uint8_t Hal_MPU_IIC_Read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t length );

#endif 

