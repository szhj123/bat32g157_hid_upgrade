#ifndef _DRV_TOUCH_H
#define _DRV_TOUCH_H

#include "hal_touch.h"

#define TP_PRES_DOWN 0x80  		//´¥ÆÁ±»°´ÏÂ	  
#define TP_CATH_PRES 0x40  		//ÓÐ°´¼ü°´ÏÂÁË 
#define CT_MAX_TOUCH                    5

/* touch panel interface define */


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

typedef struct
{
    coordinate_t posAdjBuf[4];
    coordinate_t pos;
    uint8_t posIndex;
    uint16_t posDelayCnt;
    float xfac;
    float yfac;
    short xoff;
    short yoff;
    
    uint8_t posPressed;
    uint8_t tcType;
}touch_pata_t;


void Drv_Touch_Init(void );
coordinate_t Drv_Touch_Get_Coordinate(void );
uint16_t Drv_Touch_Get_Physical_X(void );
uint16_t Drv_Touch_Get_Physical_Y(void );
uint32_t Drv_Touch_Measurement(adc_channel_t adc_channel );
uint8_t Drv_Touch_Read_XY(uint16_t *x,uint16_t *y) ;
void Drv_Touch_Adjust(void );
void Drv_Touch_Scan(void );
uint16_t Drv_Touch_Get_CurPos_X(void );
uint16_t Drv_Touch_Get_CurPos_Y(void );
uint8_t Drv_Touch_Get_State(void );

#endif 

