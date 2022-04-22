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

void Drv_Touch_Init(void )
{
    Hal_Touch_Init();

    Drv_Touch_Get_Coordinate();
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
    Hal_Touch_Read_Data(CMD_RDX);
}

uint16_t Drv_Touch_Get_Physical_Y(void )
{
    Hal_Touch_Read_Data(CMD_RDY);
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


