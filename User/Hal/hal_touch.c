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

    PORT_Init(PORTA, PIN11, OUTPUT);

    PORT_Init(PORTA, PIN3, PULLUP_INPUT);

    //INTP_Init((1<<2), INTP_FALLING);

    //INTP_Start(1<<2);
    
}

void Hal_Touch_Cs_Clr(void )
{
    PORT_ClrBit(PORTA, PIN11);
}

void Hal_Touch_Cs_Set(void )
{
    PORT_SetBit(PORTA, PIN11);
}

touch_pen_state_t Hal_Touch_Get_Pen_State(void )
{
    if(PORT_GetBit(PORTA, PIN3))
    {
        return TOUCH_PEN_UP;
    }
    else
    {
        return TOUCH_PEN_DOWN;
    }
}

void Hal_Spi_Write(uint8_t data )
{    
    SPI01_Start();

    SCI0->SIO01 = data; 
    
    while (SCI0->SSR01 & _0040_SCI_UNDER_EXECUTE)
    {
        ;
    }
}

uint8_t Hal_Spi_Read(void )
{    
    SPI01_Start();
    
    SCI0->SIO01 = 0x0;
    
    while (SCI0->SSR01 & _0040_SCI_UNDER_EXECUTE)
    {
        ;
    }

    return SCI0->SIO01;
}


uint16_t Hal_Touch_Read_Data(uint8_t command )
{
    uint8_t rev_buf[2];
	uint16_t adc_value;

    Hal_Touch_Cs_Clr();
 
	Hal_Spi_Write(command);

    rev_buf[0] = Hal_Spi_Read();

    rev_buf[1] = Hal_Spi_Read();

    Hal_Touch_Cs_Set();
    
    adc_value = (uint16_t )rev_buf[0] << 8 | rev_buf[1];
    adc_value >>= 4;
    
	return adc_value;
}



