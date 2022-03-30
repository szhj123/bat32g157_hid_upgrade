#ifndef _HAL_TIMER_H
#define _HAL_TIMER_H

#include "hardware.h"

typedef void (*hal_timer_callback_t)(void );

void Hal_Timer_Init(hal_timer_callback_t callback );
void Hal_Timer_Isr_Handler(void );


#endif 

