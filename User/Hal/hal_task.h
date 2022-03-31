#ifndef _HAL_TASK_H
#define _HAL_TASK_H

#include "hardware.h"

typedef void (*hal_task_callback_t)(void );

void Hal_Task_Init(hal_task_callback_t callback );
void Hal_Task_Isr_Handler(void );


#endif 

