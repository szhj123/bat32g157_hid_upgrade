/********************************************************
* @file       main.c
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
#include "drv_lcd.h"
#include "usb_phid_apl.h"
#include "cm_backtrace.h"

/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
void usb_cpu_delay_1us (uint16_t time)
{
    uint16_t sf_cnt;
    sf_cnt = time * 20;

    while(sf_cnt)
    {
        sf_cnt --;
    }
}

void usb_cpu_delay_xms(uint16_t n)
{
    uint16_t i;

    for(i=0;i<n;i++)
    {
        usb_cpu_delay_1us(1000);
    }
}

void fault_test_by_div0(void )
{
    uint8_t buf[4] = {0};

    uint32_t *u32Ptr = (uint32_t *)&buf[1];

    *u32Ptr = 0x55;
}


int main(void )
{
    Drv_Task_Init();

    Drv_Timer_Init();

    Drv_Lcd_Init();
	
	Usb_Init(); /* USB sample application */

    cm_backtrace_init("CmBacktrace", "0.0.1", "0.0.1");
    
	while(1)
	{
	}
}




