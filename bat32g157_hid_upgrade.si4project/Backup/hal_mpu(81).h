#ifndef _HAL_MPU6050_H
#define _HAL_MPU6050_H

#include "hardware.h"

void Hal_MPU_IIC_Init(void );
uint8_t Hal_MPU_IIC_Write(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t const *data );
uint8_t Hal_MPU_IIC_Read(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data );
void Hal_MPU_Delay_Count(void );
void Hal_MPU_Delay_Ms(uint32_t nms );
void Hal_MPU_Get_Ms(uint32_t *count );

#endif 

