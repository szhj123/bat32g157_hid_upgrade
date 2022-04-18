/********************************************************
* @file       hardware.c
* @author     szhj13
* @version    V1.0
* @date       2021-08-12
* @brief      the entry of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hardware.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
void Systick_Init(void )
{
    SystemCoreClockUpdate();
    
    SysTick_Config(SystemCoreClock/1000);
}

void Timer_Interval_Init(void )
{
    CGC->PER0 |= CGC_PER0_TM40EN_Msk;    /* enables input clock supply */
    TM40->TPS0 = _0000_TM4_CKM3_fCLK_8 | _0000_TM4_CKM2_fCLK_1 | _0000_TM4_CKM1_fCLK_0 | _0000_TM4_CKM0_fCLK_0; 

    SystemCoreClockUpdate();
    
    /* stop specify channel */
    TM40->TT0 |= TM4_CHANNEL_0;

    /* clear interrupt flag */
    INTC_ClearPendingIRQ(TM00_IRQn);
    NVIC_ClearPendingIRQ(TM00_IRQn);
    /* Channel 0 is used as interval timer */
    TM40->TMR00 = _8000_TM4_CLOCK_SELECT_CKM1 | _0000_TM4_CLOCK_MODE_CKS | _0000_TM4_TRIGGER_SOFTWARE |
                  _0000_TM4_MODE_INTERVAL_TIMER | _0000_TM4_START_INT_UNUSED;
    TM40->TDR00 = SystemCoreClock/1000 - 1;
    TM40->TOE0 &= ~_0001_TM4_CH0_OUTPUT_ENABLE;

    TM40->TS0 |= TM4_CHANNEL_0;

    INTC_EnableIRQ(TM00_IRQn);
}

void Uart_Init(void )
{
    SystemCoreClockUpdate();
    
	UART0_Init(SystemCoreClock, 115200);
}

