#ifndef _HAL_TOUCH_H
#define _HAL_TOUCH_H

#include "hardware.h"

void Hal_Touch_Init(void );
void Hal_Touch_Cs_Clr(void );
void Hal_Touch_Cs_Set(void );
uint16_t Hal_Touch_Read_Data(uint8_t cmd );


#endif 

