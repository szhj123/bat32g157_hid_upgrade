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
    uint8_t x,y,z;
    
    x = 10;
    y = 0;
    z = x / y;

    printf("z:%d\n", z);
}


int main(void )
{
    #if 0
    Drv_Task_Init();

    Drv_Timer_Init();
	
	Usb_Init(); /* USB sample application */
    #else
    MD_STATUS status;
    SystemCoreClockUpdate();
	status = UART0_Init(SystemCoreClock, 19200);
	if(status == MD_ERROR)
	{
		while(1);
	}
	
	printf("Hello, UART\n");
    #endif 

    cm_backtrace_init("CmBacktrace", "0.0.1", "0.0.1");

    fault_test_by_div0();
    
	while(1)
	{
	}
}




