#ifndef _HARDWARE_H
#define _HARDWARE_H

#include <stdbool.h>
#include <stdio.h>
#include "BAT32G157.h"
#include "userdefine.h"
#include "gpio.h"
#include "tim4.h"
#include "sci.h"

void Systick_Init(void );
void Timer_Interval_Init(void );
void Uart_Init(void );


#endif 

