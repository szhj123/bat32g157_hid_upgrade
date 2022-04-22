#ifndef _HAL_TOUCH_H
#define _HAL_TOUCH_H

#include "hardware.h"

typedef enum
{
    TOUCH_PEN_UP = 0,
    TOUCH_PEN_DOWN
}touch_pen_state_t;


void Hal_Touch_Init(void );
void Hal_Touch_Cs_Clr(void );
void Hal_Touch_Cs_Set(void );
uint16_t Hal_Touch_Read_Data(uint8_t cmd );
touch_pen_state_t Hal_Touch_Get_Pen_State(void );


#endif 

