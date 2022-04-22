/********************************************************
* @file       drv_ili9341.c
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

#include "drv_ili9341.h"
/* Private typedef --------------------------------------*/
typedef enum
{
    ILI9341_WR_STATE_UNDEFINE = 0,
    ILI9341_WR_STATE_IDLE,
    ILI9341_WR_STATE_BUSY
}ili9341_wr_state_t;

/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
static uint16_t ArrayRGB[320] = {0};
uint16_t back_color = 0xff00;
uint8_t lcdbint_cnt;
ili9341_wr_state_t ili9341_wr_state;


void ili9341_Init(void )
{
    LCDB_Init();
    
    LCD_CS_LOW();	
	LCD_RST_LOW();
	ili9341_Loop_Delay(120*48);
	LCD_RST_HIGH();
	ili9341_Loop_Delay(100*48);	

    ili9341_WriteReg(LCD_POWER1);    //Power control 
	ili9341_WriteData(0x1B);         //VRH[5:0] 
	ili9341_WriteReg(LCD_POWER2);    //Power control 
	ili9341_WriteData(0x01);         //SAP[2:0];BT[3:0] 
	ili9341_WriteReg(LCD_VCOM1);     //VCM control 
	ili9341_WriteData(0x30); 	     //3F
	ili9341_WriteData(0x30); 	     //3C
	ili9341_WriteReg(LCD_VCOM2);     //VCM control2 
	ili9341_WriteData(0XB7); 
	ili9341_WriteReg(LCD_MAC);       // Memory Access Control 
	ili9341_WriteData(0x6C);         //(0xC9); 
	ili9341_WriteReg(LCD_PIXEL_FORMAT);   
	ili9341_WriteData(0x55); 
	ili9341_WriteReg(LCD_FRMCTR1);   
	ili9341_WriteData(0x00);   
	ili9341_WriteData(0x1A); 
	ili9341_WriteReg(LCD_DFC);       // Display Function Control 
	ili9341_WriteData(0x0A); 
	ili9341_WriteData(0xA2);

	ili9341_WriteReg(0xF2);          // 3Gamma Function Disable 
	ili9341_WriteData(0x00); 
	ili9341_WriteReg(LCD_GAMMA);     //Gamma curve selected 
	ili9341_WriteData(0x01); 
	ili9341_WriteReg(LCD_PGAMMA);    //Set Gamma 
	ili9341_WriteData(0x0F); 
	ili9341_WriteData(0x2A); 
	ili9341_WriteData(0x28); 
	ili9341_WriteData(0x08); 
	ili9341_WriteData(0x0E); 
	ili9341_WriteData(0x08); 
	ili9341_WriteData(0x54); 
	ili9341_WriteData(0XA9); 
	ili9341_WriteData(0x43); 
	ili9341_WriteData(0x0A); 
	ili9341_WriteData(0x0F); 
	ili9341_WriteData(0x00); 
	ili9341_WriteData(0x00); 
	ili9341_WriteData(0x00); 
	ili9341_WriteData(0x00); 		 
	ili9341_WriteReg(LCD_NGAMMA);    //Set Gamma 
	ili9341_WriteData(0x00); 
	ili9341_WriteData(0x15); 
	ili9341_WriteData(0x17); 
	ili9341_WriteData(0x07); 
	ili9341_WriteData(0x11); 
	ili9341_WriteData(0x06); 
	ili9341_WriteData(0x2B); 
	ili9341_WriteData(0x56); 
	ili9341_WriteData(0x3C); 
	ili9341_WriteData(0x05); 
	ili9341_WriteData(0x10); 
	ili9341_WriteData(0x0F); 
	ili9341_WriteData(0x3F); 
	ili9341_WriteData(0x3F); 
	ili9341_WriteData(0x0F); 
	ili9341_WriteReg(LCD_PAGE_ADDR); 
	ili9341_WriteData(0x00);
	ili9341_WriteData(0x00);
	ili9341_WriteData(0x01);
	ili9341_WriteData(0x3f);
	ili9341_WriteReg(LCD_COLUMN_ADDR); 
	ili9341_WriteData(0x00);
	ili9341_WriteData(0x00);
	ili9341_WriteData(0x00);
	ili9341_WriteData(0xef);	 
	ili9341_WriteReg(LCD_SLEEP_OUT); //Exit Sleep
	ili9341_Loop_Delay(5000*48);//delay_ms(5);
	ili9341_WriteReg(LCD_DISPLAY_ON); //display on
	 /* GRAM start writing */
    ili9341_WriteReg(LCD_GRAM);
	
	BACKLIGHT_ON();	
	
	ili9341_ClearScreen(back_color);
}

/**
  * @brief  Writes  to the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @retval None
  */
void ili9341_WriteReg(uint8_t LCD_Reg)
{
    LCD_CS_LOW();
	LCD_DC_LOW();
	
	Write_LBDATAL(LCD_Reg);

	LCD_CS_HIGH();
}

void ili9341_WriteData(uint8_t RegValue)
{
    LCD_CS_LOW();
	LCD_DC_HIGH();
	
	Write_LBDATAL(RegValue);

	LCD_CS_HIGH();
}

void ili9341_ClearScreen(uint16_t RGBCode)
{
    #if 0
	ili9341_ClearWindow(0, 0, ILI9341_LCD_PIXEL_WIDTH, ILI9341_LCD_PIXEL_HEIGHT, RGBCode);
    #else
    uint32_t size = ILI9341_LCD_PIXEL_WIDTH * ILI9341_LCD_PIXEL_HEIGHT; 
    
	ili9341_SetDisplayWindow(0, 0, ILI9341_LCD_PIXEL_WIDTH, ILI9341_LCD_PIXEL_HEIGHT);

    LCD_CS_LOW();
	LCD_DC_HIGH();

    back_color = RGBCode;

    LCDB_Wr_DMA_Fix_SrcAddr(&back_color, size, ili9341_Wr_End_Callback);

    ili9341_Set_Wr_State(ILI9341_WR_STATE_BUSY);
    
    while(ili9341_Get_Wr_State() != ILI9341_WR_STATE_IDLE);

	LCD_CS_HIGH();
    
    #endif 
}

void ili9341_ClearWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t RGBCode)
{
	uint32_t count, i;
	uint16_t buff[128];
	for(i = 0; i < 128; i++)
	{
		buff[i] = RGBCode;
	}
    
	ili9341_SetDisplayWindow(Xpos, Ypos, Width, Height);
    
	count = Width * Height;

	while(count > 128)
	{
		ili9341_WriteMultiData((uint8_t*)buff , 128*2);
		count -= 128;
	}
    
	ili9341_WriteMultiData((uint8_t*)buff , count*2);
}


void ili9341_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Column addr set, 4 args, no delay: XSTART = Xpos, XEND = (Xpos + Width - 1) */
	ili9341_WriteReg(LCD_COLUMN_ADDR);
	ili9341_WriteData(Xpos >> 8);
	ili9341_WriteData(Xpos & 0xff);
	ili9341_WriteData((Xpos + Width -1) >> 8);
	ili9341_WriteData((Xpos + Width -1) & 0xff);
  /* Row addr set, 4 args, no delay: YSTART = Ypos, YEND = (Ypos + Height - 1) */
	ili9341_WriteReg(LCD_PAGE_ADDR); 
	ili9341_WriteData(Ypos >> 8);
	ili9341_WriteData(Ypos & 0xff);
	ili9341_WriteData((Ypos + Height -1) >> 8);
	ili9341_WriteData((Ypos + Height -1) & 0xff);
    
	ili9341_WriteReg(LCD_GRAM);
}


void ili9341_WriteMultiData(uint8_t *const datbuf, uint16_t size)
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

    LCDB_Wr_DMA(datbuf, size, ili9341_Wr_End_Callback);

    ili9341_Set_Wr_State(ILI9341_WR_STATE_BUSY);
    
    while(ili9341_Get_Wr_State() != ILI9341_WR_STATE_IDLE);

    #endif 
    
	LCD_CS_HIGH();
}

void ili9341_Loop_Delay(uint32_t time )
{
    uint32_t sf_cnt;
    sf_cnt = time * 20;

    while(sf_cnt)
    {
        sf_cnt --;
    }
}


void ili9341_Wr_End_Callback(void )
{
    ili9341_Set_Wr_State(ILI9341_WR_STATE_IDLE);
}

void ili9341_Set_Wr_State(uint8_t state )
{
    ili9341_wr_state = state;
}

uint8_t ili9341_Get_Wr_State(void )
{
    return ili9341_wr_state;
}


