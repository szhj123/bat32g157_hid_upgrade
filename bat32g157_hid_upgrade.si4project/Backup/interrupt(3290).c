/********************************************************
* @file       interrupt.c
* @author     szhj13
* @version    V1.0
* @date       2021-08-12
* @brief      the entry of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hardware.h"
#include "hal_timer.h"
#include "drv_usb.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
void IRQ18_Handler(void) __attribute__((alias("tm40_channel0_interrupt")));

/* Private variables ------------------------------------*/
void tm40_channel0_interrupt(void )
{
    INTC_ClearPendingIRQ(TM00_IRQn);    /* clear INTTM00 interrupt flag */

    Hal_Timer_Isr_Handler();
}

void SysTick_Handler(void)
{
    Drv_Usb_Delay_Count();
}


