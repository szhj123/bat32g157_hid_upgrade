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
#include "hal_task.h"
#include "hal_lcd.h"
#include "hal_mpu.h"
#include "lvgl.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
void IRQ18_Handler(void) __attribute__((alias("tm40_channel0_interrupt")));
void IRQ15_Handler(void) __attribute__((alias("lcdb_interrupt")));
void IRQ03_Handler(void) __attribute__((alias("intp_2_6_interrupt")));

/* Private variables ------------------------------------*/
void tm40_channel0_interrupt(void )
{
    INTC_ClearPendingIRQ(TM00_IRQn);    /* clear INTTM00 interrupt flag */

    Hal_Timer_Isr_Handler();

    lv_tick_inc(1);

    Hal_MPU_Delay_Count();

}

void SysTick_Handler(void)
{
    Hal_Task_Isr_Handler();
}

void lcdb_interrupt(void )
{
    INTC_ClearPendingIRQ(LCDB_IRQn);     /* clear LCDB interrupt flag */

    LCDB_Isr_Handler();
}

void intp_2_6_interrupt(void )
{
    volatile uint8_t ifl;

    ifl = INTC_GetPendingIRQ(INTP2_IRQn);

    if(ifl)
    {
        INTC_ClearPendingIRQ(INTP2_IRQn);
    }
}

