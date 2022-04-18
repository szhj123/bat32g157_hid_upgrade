/********************************************************
* @file       drv_lcd.c
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
#include "cm_backtrace.h"

#include "drv_lcd.h"
/* Private typedef --------------------------------------*/
typedef enum
{
    LCD_WR_STATE_UNDEFINE = 0,
    LCD_WR_STATE_IDLE,
    LCD_WR_STATE_BUSY
}lcd_wr_state_t;
/* Private define ---------------------------------------*/

/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
static LCDB_Typedef Lcdb =
{
    .mode = LCDB_MODE_TYPE_8080,    /* 8080 Mode */
    .clock_div = LCDB_CLOCK_DIV_1,	/* 1 divider */
    .cycle = LDB_CYCLE_8T,          /* 6T cycles */
    .wait_cycle = LDB_WAIT_CYCLE_2T	/* 2T wait cycles */
};

static uint16_t lcd_rgb_color;

static lcd_wr_state_t lcd_wr_state;

void Drv_Lcd_Init(void )
{
    LCDB_Init(&Lcdb);

    ili9341_Init();
}

void Drv_Lcd_Wr_Reg(uint8_t reg)
{
	LCD_CS_LOW();
	LCD_DC_LOW();
	
	Write_LBDATAL(reg);

	LCD_CS_HIGH();
}

void Drv_Lcd_Wr_Data(uint8_t regVal )
{
    LCD_CS_LOW();
	LCD_DC_HIGH();
	
	Write_LBDATAL(regVal);

	LCD_CS_HIGH();
}

void Drv_Lcd_Wr_Multi_Data(uint8_t *const datbuf, uint16_t size)
{
	uint16_t i;
    
	LCD_CS_LOW();
	LCD_DC_HIGH();

    #if 0
	for(i = 0; i < size; i++)
	{
		Write_LBDATAL(datbuf[i]);
	}
    #else

    LCDB_Wr_DMA(datbuf, size, Drv_Lcd_Wr_End_Callback);

    Drv_Lcd_Set_Wr_State(LCD_WR_STATE_BUSY);
    
    while(Drv_Lcd_Get_Wr_State() != LCD_WR_STATE_IDLE);

    #endif 
    
	LCD_CS_HIGH();
}

void Drv_Lcd_ClearScreen(uint16_t rgbCode, uint32_t size )
{
    LCD_CS_LOW();
	LCD_DC_HIGH();

    lcd_rgb_color = rgbCode;

    LCDB_Wr_DMA_Fix_SrcAddr(&rgbCode, size, Drv_Lcd_Wr_End_Callback);

    Drv_Lcd_Set_Wr_State(LCD_WR_STATE_BUSY);
    
    while(Drv_Lcd_Get_Wr_State() != LCD_WR_STATE_IDLE);

	LCD_CS_HIGH();
}

void Drv_Lcd_Wr_End_Callback(void )
{
    Drv_Lcd_Set_Wr_State(LCD_WR_STATE_IDLE);
}

void Drv_Lcd_Set_Wr_State(uint8_t state )
{
    lcd_wr_state = state;
}

uint8_t Drv_Lcd_Get_Wr_State(void )
{
    return lcd_wr_state;
}


