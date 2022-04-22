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
#include "hal_mpu6050.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
#define TIMEOUT_PER_BYTE 2000

/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
static uint8_t tmpBuf[256+8];
static uint8_t tmpBuf2[1];

void Hal_MPU_IIC_Init(void )
{
    IICA0_Init();
}

uint8_t Hal_MPU_IIC_Write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t length )
{
    volatile uint32_t timeout = 0;
    uint16_t i;
    
    tmpBuf[0] = reg;

    for(i=0;i<length;i++)
    {
        tmpBuf[i+1] = buf[i];
    }

    if(MD_OK != IICA0_MasterSend(addr <<1, tmpBuf, length + 1, 200))
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

uint8_t Hal_MPU_IIC_Read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t length )
{
	volatile uint32_t timeout = 0;
    
	tmpBuf2[0] = addr;
    
	if(MD_OK != IICA0_MasterSend(addr << 1, &tmpBuf2[0], 1, 20))
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
	
	if(MD_OK != IICA0_MasterReceive(addr << 1, (uint8_t* )buf, length, 20))
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


