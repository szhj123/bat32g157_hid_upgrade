/********************************************************
* @file       hal_touch.c
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

#include "hal_touch.h"

/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Hal_Touch_Init(void )
{
    SPI01_MasterInit(SPI_MODE_0);
}

void Hal_Touch_Cs_Clr(void )
{
    PORT_ClrBit(PORTA, PIN11);
}

void Hal_Touch_Cs_Set(void )
{
    PORT_SetBit(PORTA, PIN11);
}

uint16_t Hal_Touch_Read_Data(uint8_t cmd )
{
    uint8_t rev_buf[2];
    uint16_t adc_value;

    Hal_Touch_Cs_Clr();
    
	SPI01_MasterSend(&cmd, 1);

	SPI01_MasterReceive(&rev_buf[0], 1);		//**when read data, keep MOSI LOW !**
	SPI01_MasterReceive(&rev_buf[1], 1);

    Hal_Touch_Cs_Set();

    adc_value = rev_buf[0];
    adc_value = (adc_value << 8) | rev_buf[1];
    adc_value >>= 4;
    
	return adc_value;
}

