#ifndef _HARDWARE_H
#define _HARDWARE_H

#include <stdbool.h>
#include <stdio.h>
#include "BAT32G157.h"
#include "userdefine.h"
#include "gpio.h"
#include "tim4.h"
#include "sci.h"

#define Read_LBCTL_BYF() (LCDB->LBCTL&LCDB_LBCTL_BYF_Msk)
#define Read_LBCTL_TPF() (LCDB->LBCTL&LCDB_LBCTL_TPF_Msk)

#define Write_LBDATA(dat)       while(Read_LBCTL_BYF());\
								LCDB->LBDATA = dat;\
								while(Read_LBCTL_TPF());/* write LBDATA */

#define Write_LBDATAL(dat)      while(Read_LBCTL_BYF());\
								LCDB->LBDATAL = dat;\
								while(Read_LBCTL_TPF());/* write LBDATAL */

#define Write_LBDATAL(dat)      while(Read_LBCTL_BYF());\
								LCDB->LBDATAL = dat;\
								while(Read_LBCTL_TPF());/* write LBDATAL */

void Systick_Init(void );
void Timer_Interval_Init(void );
void Uart_Init(void );


#endif 

