/********************************************************
* @file       drv_touch.c
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
#include "stdlib.h"
#include "math.h"
#include <string.h>
#include "BAT32G157.h"

#include "drv_task.h"
#include "drv_timer.h"
#include "drv_ili9341.h"
#include "drv_touch.h"
#include "usb_phid_apl.h"
#include "cm_backtrace.h"



/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Touch_Timer_Count(void *arg );
/* Private variables ------------------------------------*/
uint8_t* TP_REMIND_MSG_TBL="screen adjustment";

uint8_t CMD_RDX=0X90;
uint8_t CMD_RDY=0XD0;

static touch_pata_t touchPara;

void Drv_Touch_Init(void )
{
    Hal_Touch_Init();

    Drv_Timer_Run_Period(Drv_Touch_Timer_Count, 0, 1, NULL);

    Drv_Touch_Adjust();	    
}

coordinate_t Drv_Touch_Get_Coordinate(void )
{
    coordinate_t pos;

    uint32_t dx, dy;

    dx = Drv_Touch_Measurement(ADC_CHANNEL_X);
    dy = Drv_Touch_Measurement(ADC_CHANNEL_Y);

    pos.x = dx;
    pos.y = dy;
    
    return pos;
}

uint16_t Drv_Touch_Get_Physical_X(void )
{
    return Hal_Touch_Read_Data(CMD_RDX);
}

uint16_t Drv_Touch_Get_Physical_Y(void )
{
    return Hal_Touch_Read_Data(CMD_RDY);
}

uint32_t Drv_Touch_Measurement(adc_channel_t adc_channel )
{
    uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint32_t sum=0;
	uint16_t temp;
    
	for(i=0;i<READ_TIMES;i++)
	{				 
        if(adc_channel == ADC_CHANNEL_X)
        {
		    buf[i] = Drv_Touch_Get_Physical_X();	 
        }
        else
        {
            buf[i] = Drv_Touch_Get_Physical_Y();
        }
	}			
    
	for(i=0;i<READ_TIMES-1; i++)
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
    
	sum=0;

    for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
    {
        temp=sum/(READ_TIMES-2*LOST_VAL);
    }
    
    return temp;       
}

void Drv_Touch_Draw_Point(uint16_t x, uint16_t y, uint16_t color )
{
    ili9341_DrawLine(x-12, y, x+13, y, color);
    ili9341_DrawLine(x, y-12, x, y+13, color);
    ili9341_WritePixel(x+1,y+1, color);
    ili9341_WritePixel(x-1,y+1, color);
    ili9341_WritePixel(x+1,y-1, color);
    ili9341_WritePixel(x+1,y-1, color);
    ili9341_DrawCircle(x, y, 6, color);
}


void Drv_Touch_Adj_Paint(void )
{
    switch(touchPara.posIndex)
    {
        case 0:
        {
            Drv_Touch_Draw_Point(ILI9341_LCD_PIXEL_WIDTH-20, ILI9341_LCD_PIXEL_HEIGHT-20, WHITE);
            Drv_Touch_Draw_Point(20, 20, RED);
            break;
        }
        case 1: 
        {                               
            Drv_Touch_Draw_Point(20, 20, WHITE);
            Drv_Touch_Draw_Point(ILI9341_LCD_PIXEL_WIDTH-20, 20, RED);
            break;
        }
        case 2: 
        {
            Drv_Touch_Draw_Point(ILI9341_LCD_PIXEL_WIDTH-20, 20, WHITE);
            Drv_Touch_Draw_Point(20, ILI9341_LCD_PIXEL_HEIGHT-20, RED);
            break;
        }
        case 3: 
        {
            Drv_Touch_Draw_Point(20, ILI9341_LCD_PIXEL_HEIGHT-20, WHITE);
            Drv_Touch_Draw_Point(ILI9341_LCD_PIXEL_WIDTH-20, ILI9341_LCD_PIXEL_HEIGHT-20, RED);
            break;
        }
        default: break;
    }
}

void Drv_Touch_Adj_Set_Type(uint8_t type)
{
    if(type)
    {
        CMD_RDX=0XD0;
        CMD_RDY=0X90;
    }
    else
    {
        CMD_RDX=0X90;
        CMD_RDY=0XD0;
    }
}


uint8_t Drv_Touch_Adj_Get(void )
{
    uint32_t a, b;
    uint16_t c1, c2;
    double fac;

    a = abs(touchPara.posAdjBuf[0].x - touchPara.posAdjBuf[1].x);
    b = abs(touchPara.posAdjBuf[0].y - touchPara.posAdjBuf[1].y);
    a *= a;
    b *= b;
    c1 = sqrt(a+b);

    a = abs(touchPara.posAdjBuf[2].x - touchPara.posAdjBuf[3].x);
    b = abs(touchPara.posAdjBuf[2].y - touchPara.posAdjBuf[3].y);
    a *= a;
    b *= b;
    c2 = sqrt(a+b);

    fac=(float)c1/c2;

    if(fac < 0.95 || fac > 1.05 || c1 == 0 || c2 == 0)
    {
        return 0;
    }

    a = abs(touchPara.posAdjBuf[0].x - touchPara.posAdjBuf[2].x);
    b = abs(touchPara.posAdjBuf[0].y - touchPara.posAdjBuf[2].y);
    a *= a;
    b *= b;
    c1 = sqrt(a+b);

    a = abs(touchPara.posAdjBuf[1].x - touchPara.posAdjBuf[3].x);
    b = abs(touchPara.posAdjBuf[1].y - touchPara.posAdjBuf[3].y);
    a *= a;
    b *= b;
    c2 = sqrt(a+b);

    fac=(float)c1/c2;

    if(fac < 0.95 || fac > 1.05)
    {
        return 0;
    }

    a = abs(touchPara.posAdjBuf[0].x - touchPara.posAdjBuf[3].x);
    b = abs(touchPara.posAdjBuf[0].y - touchPara.posAdjBuf[3].y);
    a *= a;
    b *= b;
    c1 = sqrt(a+b);

    a = abs(touchPara.posAdjBuf[1].x - touchPara.posAdjBuf[2].x);
    b = abs(touchPara.posAdjBuf[1].y - touchPara.posAdjBuf[2].y);
    a *= a;
    b *= b;
    c2 = sqrt(a+b);

    fac=(float)c1/c2;

    if(fac < 0.95 || fac > 1.05)
    {
        return 0;
    }

    touchPara.xfac = (float)(ILI9341_LCD_PIXEL_WIDTH - 40)/(touchPara.posAdjBuf[1].x - touchPara.posAdjBuf[0].x);
    touchPara.xoff = (ILI9341_LCD_PIXEL_WIDTH - touchPara.xfac*(touchPara.posAdjBuf[1].x + touchPara.posAdjBuf[0].x))/2;

    touchPara.yfac = (float)(ILI9341_LCD_PIXEL_HEIGHT -40)/(touchPara.posAdjBuf[2].y - touchPara.posAdjBuf[0].y);
    touchPara.yoff = (ILI9341_LCD_PIXEL_HEIGHT - touchPara.yfac*(touchPara.posAdjBuf[2].y + touchPara.posAdjBuf[0].y))/2;


    if(abs(touchPara.xfac) > 2 || abs(touchPara.yfac) > 2)
    {
        touchPara.tcType ^= 1;
        
        Drv_Touch_Adj_Set_Type(touchPara.tcType);

        return 0;
    }

    ili9341_ClearScreen(WHITE);

    ili9341_Show_String(30, 30, "hello", 5, RED);

    return 1;

}

void Drv_Touch_Scan(void )
{
    if(Hal_Touch_Get_Pen_State() == TOUCH_PEN_DOWN)
    {
        if(!touchPara.posPressed)
        {
            touchPara.pos = Drv_Touch_Get_Coordinate();

            touchPara.pos.x = touchPara.xfac * touchPara.pos.x + touchPara.xoff;
            touchPara.pos.y = touchPara.yfac * touchPara.pos.y + touchPara.yoff;
            touchPara.posPressed = 1;
        }
    }
    else
    {
        if(touchPara.posPressed)
        {
            touchPara.posPressed = 0;
        }
        else
        {
            touchPara.posAdjBuf[0].x = 0xffff;
            touchPara.posAdjBuf[0].y = 0xffff;
        }
    }
}

uint16_t Drv_Touch_Get_CurPos_X(void )
{
    return touchPara.pos.x;
}

uint16_t Drv_Touch_Get_CurPos_Y(void )
{
    return touchPara.pos.y;
}

uint8_t Drv_Touch_Get_State(void )
{
    return Hal_Touch_Get_Pen_State();
}


void Drv_Touch_Adjust(void )
{
    Drv_Touch_Draw_Point(20, 20, RED);

    touchPara.tcType = 0;
    
    Drv_Touch_Adj_Set_Type(touchPara.tcType);
   
    while(1)
    {
        if(Hal_Touch_Get_Pen_State() == TOUCH_PEN_DOWN)
        {
            if(touchPara.posDelayCnt > 250)
            {
                touchPara.posDelayCnt = 0;
                if(!touchPara.posPressed)
                {
                    touchPara.posPressed = 1;
                    touchPara.posAdjBuf[touchPara.posIndex] = Drv_Touch_Get_Coordinate();
                    touchPara.posIndex++;
                    
                    Drv_Touch_Adj_Paint();

                    if(touchPara.posIndex > 3)
                    {
                        touchPara.posIndex = 0;

                        if(Drv_Touch_Adj_Get())
                        {
                            touchPara.posIndex = 0;
                            touchPara.posPressed = 0;
                            
                            return ;
                        }
                    }
                }
            }
        }
        else
        {
            touchPara.posPressed = 0;
            touchPara.posDelayCnt = 0;
        }
    }
}


static void Drv_Touch_Timer_Count(void *arg )
{
    if(touchPara.posDelayCnt < 0xffff)
    {
        touchPara.posDelayCnt++;
    }
}

