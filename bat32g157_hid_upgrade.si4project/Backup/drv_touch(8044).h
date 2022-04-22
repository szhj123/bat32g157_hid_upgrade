#ifndef _DRV_TOUCH_H
#define _DRV_TOUCH_H

#include "hal_touch.h"

/* touch panel interface define */
#define  CMD_RDX  0xD0	 //´¥ÃþIC¶Á×ø±ê»ý´æÆ÷
#define  CMD_RDY  0x90


#define READ_TIMES 8 
#define LOST_VAL 2	 


typedef struct
{
    uint16_t x;
    uint16_t y;
}coordinate_t;

typedef enum
{
    ADC_CHANNEL_X = 0,
    ADC_CHANNEL_Y
}adc_channel_t;

void Drv_Touch_Init(void );
uint16_t Drv_Touch_Get_Physical_X(void );
uint16_t Drv_Touch_Get_Physical_Y(void );
uint32_t Drv_Touch_Measurement(adc_channel_t adc_channel );

#endif 

