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
#include "usb_phid_apl.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
uint32_t g_ticks;

void usb_cpu_delay_xms(uint32_t n)
{
    g_ticks = n;

    while(g_ticks);
}

void usb_cpu_delay_1us (uint16_t time)
{
    uint16_t sf_cnt;
    sf_cnt = time * 20;

    while(sf_cnt)
    {
        sf_cnt --;
    }
}

int main(void )
{
	uint32_t msCnt; 	// count value of 1ms
	 g_ticks = 1000; 	// 1000ms
    //SystemCoreClockUpdate();
    //msCnt = SystemCoreClock / 1000;
    msCnt = 48000000U / 1000;				// USB use 48MHz
    SysTick_Config(msCnt);
	
    //Drv_Task_Init();

    //Drv_Timer_Init();
	
	 usb_main(); /* USB sample application */
    
	while(1)
	{
	}
}


