/********************************************************
* @file       drv_timer.c
* @author     szhj13
* @version    V1.0
* @date       2021-08-12
* @brief      the entry of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_timer.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Timer_Isr_Callback(void );

/* Private variables ------------------------------------*/
timer_ctrl_block_t timerList[TIMER_MAX_NUM];

void Drv_Timer_Init(void )
{
    uint8_t i;

    for(i=0;i<TIMER_MAX_NUM;i++)
    {
        timerList[i].handler = NULL;
        timerList[i].arg = NULL;
        timerList[i].ticks = 0;
        timerList[i].period = 0;
        timerList[i].registFlag = 0;
    }
    
    Hal_Timer_Init(Drv_Timer_Isr_Callback);
}

static uint8_t Drv_Timer_Regist(timer_handler_t handler, uint16_t ticks, uint16_t period, void *arg )
{
    uint8_t i;

    for(i=0;i<TIMER_MAX_NUM;i++)
    {
        if(!timerList[i].registFlag)
        {
            timerList[i].handler = handler;
            timerList[i].arg = arg;
            timerList[i].ticks = ticks;
            timerList[i].period = period;

            timerList[i].registFlag = 1;

            return i;
        }
    }

    return TIMER_NULL;
}

uint8_t Drv_Timer_Run_Oneshort(timer_handler_t handler, uint16_t ticks, void *arg )
{
    return Drv_Timer_Regist(handler, ticks, 0, arg );
}

uint8_t Drv_Timer_Run_Period(timer_handler_t handler, uint16_t ticks, uint16_t period, void *arg )
{
    return Drv_Timer_Regist(handler, ticks, period, arg );
}

void Drv_Timer_Delete(uint8_t timerId )
{
    if(timerId >= TIMER_MAX_NUM)
    {
        return ;
    }

    timerList[timerId].handler = NULL;
    timerList[timerId].arg = NULL;
    timerList[timerId].ticks = 0;
    timerList[timerId].period = 0;
    timerList[timerId].registFlag = 0;
}

static void Drv_Timer_Isr_Callback(void )
{
    uint8_t i;

    for(i=0;i<TIMER_MAX_NUM;i++)
    {
        if(timerList[i].registFlag)
        {
            if(timerList[i].ticks > 0)
            {
                timerList[i].ticks--;
            }

            if(timerList[i].ticks == 0)
            {
                if(timerList[i].handler != NULL)
                {
                    timerList[i].handler(timerList[i].arg);
                }

                if(timerList[i].period)
                {
                    timerList[i].ticks = timerList[i].period;
                }
                else
                {
                    Drv_Timer_Delete(i);
                }
            }
        }
    }
}

