#ifndef _DRV_LCD_H
#define _DRV_LCD_H

#include "hal_lcd.h"
#include "drv_ili9341.h"

#define LCD_W 320
#define LCD_H 240
#define TFT_X_OFFSET 0
#define TFT_Y_OFFSET 0


void Drv_Lcd_Init(void );
void Drv_Ili9341_Init(void );
void Drv_Lcd_Wr_Reg(uint8_t reg);
void Drv_Lcd_Wr_Data(uint8_t regVal );
void Drv_Lcd_Wr_Multi_Data(uint8_t *datbuf, uint16_t size);
void Drv_Lcd_ClearScreen(uint16_t rgbCode, uint32_t size );
void Drv_Lcd_Wr_End_Callback(void );
void Drv_Lcd_Set_Wr_State(uint8_t state );
uint8_t Drv_Lcd_Get_Wr_State(void );

#endif 

