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

    PORT_Init(PORTA, PIN3, PULLUP_INPUT);

    INTP_Init((1<<3), INTP_FALLING);

    INTP_Start(1<<3);
}

void Hal_Touch_Cs_Clr(void )
{
    PORT_ClrBit(PORTA, PIN11);
}

void Hal_Touch_Cs_Set(void )
{
    PORT_SetBit(PORTA, PIN11);
}

void Hal_Touch_Dleay_1US (uint16_t time)
{
    uint16_t sf_cnt;
    sf_cnt = time * 20;

    while(sf_cnt)
    {
        sf_cnt --;
    }
}

void Hal_Spi_Write(uint8_t data )
{    
    SCI0->SIO01 = data;

    SCI0->SS0 |= _0002_SCI_CH1_START_TRG_ON;
    
    while (SCI0->SSR01 & _0040_SCI_UNDER_EXECUTE)
    {
        ;
    }
}

uint8_t Hal_Spi_Read(void )
{    
    SCI0->SIO01 = 0xff;

    SCI0->SS0 |= _0002_SCI_CH1_START_TRG_ON;
    
    while (SCI0->SSR01 & _0040_SCI_UNDER_EXECUTE)
    {
        ;
    }

    return SCI0->SIO01;
}


uint16_t Hal_Touch_Read_Data(uint8_t cmd )
{
    uint8_t rev_buf[2] = {0};
    uint16_t adc_value;

    Hal_Touch_Cs_Clr();
    
	Hal_Spi_Write(cmd);

    rev_buf[0] = Hal_Spi_Read();

    rev_buf[1] = Hal_Spi_Read();
    
    Hal_Touch_Cs_Set();

    adc_value = ((uint16_t )rev_buf[0]) << 8 | rev_buf[1];
    
    adc_value >>= 4;
    
	return adc_value;
}

