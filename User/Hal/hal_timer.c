/********************************************************
* @file       hal_timer.c
* @author     szhj13
* @version    V1.0
* @date       2021-08-12
* @brief      the entry of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_timer.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/

/* Private variables ------------------------------------*/
hal_timer_callback_t hal_timer_callback = NULL;

void Hal_Timer_Init(hal_timer_callback_t callback )
{
    hal_timer_callback = callback;

    Timer_Interval_Init();
}

void Hal_Timer_Isr_Handler(void )
{
    if(hal_timer_callback != NULL)
    {
        hal_timer_callback();
    }
}

