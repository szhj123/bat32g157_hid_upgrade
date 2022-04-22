/********************************************************
* @file       drv_mpu6050.c
* @author     szhj13
* @version    V1.0
* @date       2021-08-12
* @brief      the entry of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include "BAT32G157.h"

#include "drv_task.h"
#include "drv_timer.h"
#include "drv_mpu6050.h"

/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_MPU_Delay_Count(void *arg );
/* Private variables ------------------------------------*/
volatile uint16_t mpu_delay_cnt;

void Drv_MPU_Init(void )
{
    static uint8_t res;
    
    Hal_MPU_IIC_Init();

    Drv_Timer_Run_Period(Drv_MPU_Delay_Count, 0, 1, NULL);

    Drv_MPU_Write_Byte(MPU_ADDR, MPU_PWR_MGMT1_REG, 0x80);//复位MPU6050

    Drv_MPU_Delay_Ms(100);

    Drv_MPU_Write_Byte(MPU_ADDR, MPU_PWR_MGMT1_REG, 0X00);//唤醒MPU9250

    Drv_MPU_Set_Gyro_Fsr(3);//陀螺仪传感器,±2000dps

    Drv_MPU_Set_Accel_Fsr(0);//加速度传感器,±2g

    Drv_MPU_Set_Rate(1000);//设置采样率1KHz

    Drv_MPU_Write_Byte(MPU_ADDR, MPU_INT_EN_REG, 0x00);//关闭数据中断

    Drv_MPU_Write_Byte(MPU_ADDR, MPU_USER_CTRL_REG, 0X00);	 //I2C主模式关闭
    
    Drv_MPU_Write_Byte(MPU_ADDR, MPU_FIFO_EN_REG, 0X00);	 //关闭FIFO

    Drv_MPU_Write_Byte(MPU_ADDR, MPU_INTBP_CFG_REG, 0X82);	 //INT引脚低电平有效，开启bypass模式，可以直接读取磁力计

    Drv_MPU_Write_Byte(MPU_ADDR, MPU_I2CMST_CTRL_REG, 0X00); //

    res = Drv_MPU_Read_Byte(MPU_ADDR, MPU_DEVICE_ID_REG);	 //读取MPU6050的ID
}

//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败
void Drv_MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	 Drv_MPU_Write_Byte(MPU_ADDR, MPU_GYRO_CFG_REG, (fsr << 3) | 3); //设置陀螺仪满量程范围
}

//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败
void Drv_MPU_Set_Accel_Fsr(uint8_t fsr)
{
	 Drv_MPU_Write_Byte(MPU_ADDR, MPU_ACCEL_CFG_REG, fsr << 3); //设置加速度传感器满量程范围
}

//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败
void MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data = 0;
	if (lpf >= 188)
		data = 1;
	else if (lpf >= 98)
		data = 2;
	else if (lpf >= 42)
		data = 3;
	else if (lpf >= 20)
		data = 4;
	else if (lpf >= 10)
		data = 5;
	else
		data = 6;
    
	 Drv_MPU_Write_Byte(MPU_ADDR, MPU_CFG_REG, data); //设置数字低通滤波器
}

//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败
void Drv_MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if (rate > 1000)
		rate = 1000;
	if (rate < 4)
		rate = 4;
	data = 1000 / rate - 1;
    
	 Drv_MPU_Write_Byte(MPU_ADDR, MPU_SAMPLE_RATE_REG, data); //设置数字低通滤波器

    MPU_Set_LPF(rate / 2);									//自动设置LPF为采样率的一半
}


void Drv_MPU_Write_Byte(uint8_t addr, uint8_t reg, uint8_t data)
{
    Hal_MPU_IIC_Write(addr, reg, &data, 1);
}

uint8_t Drv_MPU_Read_Byte(uint8_t addr, uint8_t reg )
{
    uint8_t retVal = 0;
    
    Hal_MPU_IIC_Read(addr, reg, &retVal, 1);

    return retVal;
}

void Drv_MPU_Delay_Ms(uint16_t nms )
{
    mpu_delay_cnt = 0;

    while(mpu_delay_cnt < nms);
}

static void Drv_MPU_Delay_Count(void *arg )
{
    if(mpu_delay_cnt < 0xffff)
    {
        mpu_delay_cnt++;
    }
}
