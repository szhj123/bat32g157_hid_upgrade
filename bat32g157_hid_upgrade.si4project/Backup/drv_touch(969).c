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
/* Private variables ------------------------------------*/
uint8_t* TP_REMIND_MSG_TBL="screen adjustment";

uint8_t CMD_RDX=0X90;
uint8_t CMD_RDY=0XD0;

_m_tp_dev tp_dev=
{
	Drv_Touch_Init,
	Drv_Touch_Scan,
	Drv_Touch_Adjust,
	0,
	0, 
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};					



void Drv_Touch_Init(void )
{
    Hal_Touch_Init();

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

#define ERR_RANGE 50 
uint8_t Drv_Touch_Read_XY(uint16_t *x,uint16_t *y) 
{
	uint16_t x1,y1;
 	uint16_t x2,y2;
 	uint8_t flag;    

    x1 = Drv_Touch_Get_Physical_X();
    y1 = Drv_Touch_Get_Physical_Y();

    x2 = Drv_Touch_Get_Physical_X();
    y2 = Drv_Touch_Get_Physical_Y();

    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;

        return 1;
    }
    else 
    {
        return 0;     
    }
}  


void Drv_Touch_Scan(uint8_t tp)
{
    if(Hal_Touch_Get_Pen_State() == TOUCH_PEN_DOWN)
    {
       if(tp)
       {
           Drv_Touch_Read_XY(&tp_dev.x[0], &tp_dev.y[0]);
       }
       else if(Drv_Touch_Read_XY(&tp_dev.x[0], &tp_dev.y[0]))
       {
           tp_dev.x[0] = tp_dev.xfac*tp_dev.x[0] + tp_dev.xoff;
           tp_dev.y[0] = tp_dev.yfac*tp_dev.y[0] + tp_dev.yoff;
       }

       if((tp_dev.sta&TP_PRES_DOWN)==0)
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;
			tp_dev.x[4]=tp_dev.x[0];
			tp_dev.y[4]=tp_dev.y[0];  	   			 
		}			   
    }
    else
    {
        if(tp_dev.sta&TP_PRES_DOWN)
        {
			tp_dev.sta&=~(1<<7);
		}
        else
		{
			tp_dev.x[4]=0;
			tp_dev.y[4]=0;
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
		}	    
    }
    
    return tp_dev.sta&TP_PRES_DOWN;
}

void Drv_Touch_Adjust(void )
{
    uint16_t pos_temp[4][2];
    uint8_t cnt = 0;
    uint32_t tem1, tem2;
    uint16_t d1,d2;
    double fac;
    uint16_t outtime = 0;

    cnt = 0;

    ili9341_Show_String(80, 40, TP_REMIND_MSG_TBL, strlen(TP_REMIND_MSG_TBL), 0xff00);

    Drv_Touch_Draw_Point(20, 20, RED);
    
    tp_dev.sta = 0;
    tp_dev.xfac=0;

    while(1)
    {
        tp_dev.scan(1);

        if((tp_dev.sta&0xc0)==TP_CATH_PRES)
		{	
			outtime=0;		
			tp_dev.sta&=~(1<<6);
						   			   
			pos_temp[cnt][0]=tp_dev.x[0];
			pos_temp[cnt][1]=tp_dev.y[0];
			cnt++;	  
			switch(cnt)
			{			   
				case 1:						 
					Drv_Touch_Draw_Point(20,20,WHITE);				 
					Drv_Touch_Draw_Point(ILI9341_LCD_PIXEL_WIDTH-20,20,RED);	
					break;
				case 2:
 					Drv_Touch_Draw_Point(ILI9341_LCD_PIXEL_WIDTH-20,20,WHITE);	
					Drv_Touch_Draw_Point(20,ILI9341_LCD_PIXEL_HEIGHT-20,RED);	
					break;
				case 3:
 					Drv_Touch_Draw_Point(20,ILI9341_LCD_PIXEL_HEIGHT-20,WHITE);			
 					Drv_Touch_Draw_Point(ILI9341_LCD_PIXEL_WIDTH-20,ILI9341_LCD_PIXEL_HEIGHT-20,RED);	
					break;
				case 4:	 
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)
					{
						cnt=0;
 				    	Drv_Touch_Draw_Point(ILI9341_LCD_PIXEL_WIDTH-20,ILI9341_LCD_PIXEL_HEIGHT-20,WHITE);	
   	 					Drv_Touch_Draw_Point(20,20,RED);								
 						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)
					{
						cnt=0;
 				    	Drv_Touch_Draw_Point(ILI9341_LCD_PIXEL_WIDTH-20,ILI9341_LCD_PIXEL_HEIGHT-20,WHITE);	//Çå³ýµã4
   	 					Drv_Touch_Draw_Point(20,20,RED);								//»­µã1
						continue;
					}
								   
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)
					{
						cnt=0;
 				    	Drv_Touch_Draw_Point(ILI9341_LCD_PIXEL_WIDTH-20,ILI9341_LCD_PIXEL_HEIGHT-20,WHITE);
   	 					Drv_Touch_Draw_Point(20,20,RED);							
						continue;
					}
                    
					tp_dev.xfac=(float)(ILI9341_LCD_PIXEL_WIDTH-40)/(pos_temp[1][0]-pos_temp[0][0]);//µÃµ½xfac		 
					tp_dev.xoff=(ILI9341_LCD_PIXEL_WIDTH-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//µÃµ½xoff
						  
					tp_dev.yfac=(float)(ILI9341_LCD_PIXEL_HEIGHT-40)/(pos_temp[2][1]-pos_temp[0][1]);//µÃµ½yfac
					tp_dev.yoff=(ILI9341_LCD_PIXEL_HEIGHT-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//µÃµ½yoff  
					if(abs(tp_dev.xfac)>2||abs(tp_dev.yfac)>2)//´¥ÆÁºÍÔ¤ÉèµÄÏà·´ÁË.
					{
						cnt=0;
 				    	Drv_Touch_Draw_Point(ILI9341_LCD_PIXEL_WIDTH-20,ILI9341_LCD_PIXEL_HEIGHT-20,WHITE);	//Çå³ýµã4
   	 					Drv_Touch_Draw_Point(20,20,RED);								//»­µã1
						tp_dev.touchtype=!tp_dev.touchtype;//ÐÞ¸Ä´¥ÆÁÀàÐÍ.
						if(tp_dev.touchtype)//X,Y·½ÏòÓëÆÁÄ»Ïà·´
						{
							CMD_RDX=0X90;
							CMD_RDY=0XD0;	 
						}else				   //X,Y·½ÏòÓëÆÁÄ»ÏàÍ¬
						{
							CMD_RDX=0XD0;
							CMD_RDY=0X90;	 
						}			    
						continue;
					}		
					ili9341_ClearScreen(WHITE);
                    
                    ili9341_Show_String(35, 110, "Touch Screen Adjust OK!", strlen("Touch Screen Adjust OK!"), 0xff00);
					return;		 
			}
		}
    }
}

