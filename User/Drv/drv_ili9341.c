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
#include "font.h"
#include "drv_ili9341.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
#define LCD_DEFAULT_FONT         Font8x16

#define BUF_MAX_SIZE                  128


/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
uint16_t back_color = 0x001f;
uint8_t lcdbint_cnt;
ili9341_wr_state_t ili9341_wr_state;
sFONT current_font;


void ili9341_Init(void )
{
    LCDB_Init();
    
	current_font = LCD_DEFAULT_FONT;
    
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
	uint16_t buff[BUF_MAX_SIZE];
	for(i = 0; i < BUF_MAX_SIZE; i++)
	{
		buff[i] = RGBCode;
	}
    
	ili9341_SetDisplayWindow(Xpos, Ypos, Width, Height);
    
	count = Width * Height;

	while(count > BUF_MAX_SIZE)
	{
		ili9341_WriteMultiData(buff , BUF_MAX_SIZE);
		count -= BUF_MAX_SIZE;
	}
    
	ili9341_WriteMultiData(buff , count);
}

void ili9341_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t RGBCode)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
    
	if(delta_x > 0)
    {
        incx = 1;
	}
	else if(delta_x == 0)
    {
        incx = 0;
	}
	else 
    {
        incx = -1;
        delta_x = -delta_x;
    } 
    
	if(delta_y > 0)
    {
        incy = 1; 
	}
	else if(delta_y == 0)
	{
        incy=0;
	}
	else
    {
        incy = -1;
        delta_y = -delta_y;
    } 
    
	if(delta_x > delta_y)
	{
        distance = delta_x; 
	}
	else 
	{
        distance = delta_y; 
	}
    
	for(t=0;t<=distance+1;t++ )
	{  
		ili9341_WritePixel(uRow, uCol, RGBCode);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    

void ili9341_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color )
{
    int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             
	while(a<=b)
	{
		ili9341_WritePixel(x0+a,y0-b,color);             //5
 		ili9341_WritePixel(x0+b,y0-a,color);             //0           
		ili9341_WritePixel(x0+b,y0+a,color);             //4               
		ili9341_WritePixel(x0+a,y0+b,color);             //6 
		ili9341_WritePixel(x0-a,y0+b,color);             //1       
 		ili9341_WritePixel(x0-b,y0+a,color);             
		ili9341_WritePixel(x0-a,y0-b,color);             //2             
  		ili9341_WritePixel(x0-b,y0-a,color);             //7     	         
		a++;
		//Ê¹ÓÃBresenhamËã·¨»­Ô²     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}


void ili9341_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
  if((Xpos >= ILI9341_LCD_PIXEL_WIDTH) || (Ypos >= ILI9341_LCD_PIXEL_HEIGHT)) 
  {
    return;
  }
  
  /* Set Cursor */
  ili9341_SetCursor(Xpos, Ypos);
  
  ili9341_WriteData(RGBCode >> 8);
  
  ili9341_WriteData(RGBCode & 0xff);
}  


void ili9341_DrawMonoText(uint16_t Xpos, uint16_t Ypos, uint8_t width, uint8_t height, const uint16_t *TextTable, uint16_t RGBCode)
{
	uint32_t i, j, k = 0;
	uint16_t tempbuf[16];	//max font width
	uint16_t  temp, start_bit;
	
	if((Xpos >= ILI9341_LCD_PIXEL_WIDTH) || (Ypos >= ILI9341_LCD_PIXEL_HEIGHT)) 
    {
      return;
    }
	
	ili9341_SetDisplayWindow(Xpos, Ypos, width, height); 
	start_bit = 16 - width;	//font table 16 bit - font display width
	for(i = 0; i < height; i++)		//one row
	{
		temp = *TextTable;
		for(j = start_bit; j < 16; j++)
		{
			if((temp & (0x8000>>j)) != 0 )
			{
				//tempbuf[k--] = RGBCode;		//font table 16*24 is a mirror
				tempbuf[k++] = RGBCode;
			}
			else
			{
				//tempbuf[k--] = 0xffff;
				tempbuf[k++] = back_color;
			}
		}
		ili9341_WriteMultiData(tempbuf , width);	//each dot has 2 bytes(RGB 565)
		TextTable++;
		//k = width -1;
		k = 0;
	}
}


void ili9341_DrawAscii(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii, uint16_t Color)
{
    Ascii -= 0x20;
    
	ili9341_DrawMonoText(Xpos, Ypos, current_font.Width, current_font.Height, \
				         &current_font.table[Ascii * current_font.Height], Color);	 //font table is 16 bit width,no matter the font width, table fill 0 at begai
}


void ili9341_Show_String(uint16_t Xpos, uint16_t Ypos, uint8_t* String, uint16_t len, uint16_t Color)
{
    while(len-- > 0)
	{
		ili9341_DrawAscii(Xpos, Ypos, *String++, Color);
        
		Xpos += current_font.Width;
	}

    
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

void ili9341_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  ili9341_WriteReg(LCD_COLUMN_ADDR);
  ili9341_WriteData(Xpos >> 8);
  ili9341_WriteData(Xpos & 0xff);
  ili9341_WriteReg(LCD_PAGE_ADDR); 
  ili9341_WriteData(Ypos >> 8);
  ili9341_WriteData(Ypos & 0xff);
  ili9341_WriteReg(LCD_GRAM);
}



void ili9341_WriteMultiData(uint16_t *datbuf, uint16_t size)
{
	uint16_t i;
    
	LCD_CS_LOW();
	LCD_DC_HIGH();

    #if 0
	for(i = 0; i < size; i++)
	{        
		Write_LBDATA(datbuf[i]);
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

void ili9341_Set_Wr_State(ili9341_wr_state_t state )
{
    ili9341_wr_state = state;
}

uint8_t ili9341_Get_Wr_State(void )
{
    return ili9341_wr_state;
}

void ili9341_Flush(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t *RGBCode)
{
	uint32_t count;
    uint16_t i;
    uint16_t buf[BUF_MAX_SIZE];
    
 	ili9341_SetDisplayWindow(Xpos, Ypos, Width, Height);
    
	count = Width * Height;

    while(count > BUF_MAX_SIZE)
    {
        for(i=0;i<BUF_MAX_SIZE;i++)
        {
            buf[i] = ((*RGBCode) >> 8) | ((*RGBCode) << 8);

            RGBCode++;
        }

        count -= BUF_MAX_SIZE;
        
        ili9341_WriteMultiData(buf , BUF_MAX_SIZE);
    }

    for(i=0;i<count;i++)
    {
        buf[i] = ((*RGBCode) >> 8) | ((*RGBCode) << 8);

        RGBCode++;
    }

	ili9341_WriteMultiData(buf , count);

}



