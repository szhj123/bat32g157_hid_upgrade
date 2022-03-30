#ifndef _DRV_TIMER_H
#define _DRV_TIMER_H

#include "hal_timer.h"

typedef void (*timer_handler_t)(void *);

#define TIMER_MAX_NUM                16
#define TIMER_NULL                   TIMER_MAX_NUM

typedef struct _timer_ctrl_block_t
{
    timer_handler_t handler;
    void            *arg;
    uint16_t        ticks;
    uint16_t        period;
    uint8_t         registFlag;
}timer_ctrl_block_t;

void Drv_Timer_Init(void );
uint8_t Drv_Timer_Run_Oneshort(timer_handler_t handler, uint16_t ticks, void *arg );
uint8_t Drv_Timer_Run_Period(timer_handler_t handler, uint16_t ticks, uint16_t period, void *arg );
void Drv_Timer_Delete(uint8_t timerId );




#endif


