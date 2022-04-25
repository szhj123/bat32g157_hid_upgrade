/********************************************************
* @file       hal_mpu6050.c
* @author     szhj13
* @version    V1.0
* @date       2021-08-12
* @brief      the entry of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_mpu.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
#define TIMEOUT_PER_BYTE 2000

/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
uint8_t tempbuf[256+8];
uint8_t tempbuf2[1];

volatile uint32_t mpu_systick;


void Hal_MPU_IIC_Init(void )
{
    IICA0_Init();
}

uint8_t Hal_MPU_IIC_Write(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t const *data )
{
    uint8_t i;
    volatile uint32_t timeout = 0;
    
    tempbuf[0] = reg_addr;

    for(i = 0; i < length; i++)
    {
     tempbuf[i+1] = data[i];
    }
    
    if(MD_OK != IICA0_MasterSend(slave_addr << 1, tempbuf, length + 1, 20))
    {
     return 1;
    }
    
    while(g_iica0_tx_end == 0)
	{
		if(timeout++ > length * TIMEOUT_PER_BYTE)
		{
			return 1;
		}
	}
	
	return 0;
}

uint8_t Hal_MPU_IIC_Read(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data )
{
	tempbuf2[0] = reg_addr;
	volatile uint32_t timeout = 0;
    
	if(MD_OK != IICA0_MasterSend(slave_addr <<1, &tempbuf2[0], 1, 20))
	{
		return 1;
	}
    
	while(g_iica0_tx_end == 0)
	{
		if(timeout++ > 2 * TIMEOUT_PER_BYTE)
		{
			return 1;
		}
	}
	
	if(MD_OK != IICA0_MasterReceive(slave_addr <<1, (uint8_t* )data, length, 20))
	{
		return 1;
	}
    while(g_iica0_rx_end == 0)
    {
  	    if(timeout++ > length * TIMEOUT_PER_BYTE)
	    {
		    return 1;
	    }
    }
	
	return 0;
}

void Hal_MPU_Delay_Count(void )
{
    mpu_systick--;
}

void Hal_MPU_Delay_Ms(uint32_t nms )
{
    mpu_systick = nms;

    while(mpu_systick);
}

void Hal_MPU_Get_Ms(uint32_t *count )
{
    count[0] = mpu_systick;
}

