/***********************************************************************************************************************
* Copyright (C) . All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    key.h
* @brief   This file implements device driver for KEY module.
* @version 1.0.0
* @date    2020/10/24
***********************************************************************************************************************/
#ifndef KEY_H
#define KEY_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define KEY_WAITTIME                    (1U)   /* change the waiting time according to the system */ 

#define KR0_PORT                 ((uint16_t)(1<<6))
#define KR1_PORT                 ((uint16_t)(1<<7)) 
#define KR2_PORT                 ((uint16_t)(1<<12))
#define KR3_PORT                 ((uint16_t)(1<<13))
#define KR4_PORT                 ((uint16_t)(1<<14))
#define KR5_PORT                 ((uint16_t)(1<<15))
#define KR6_PORT                 ((uint16_t)(1<<8)) 
#define KR7_PORT                 ((uint16_t)(1<<9)) 
#define KRALL_PORT               ((uint16_t)(0xf3c0)) 

#define KRM0                    1U  
#define KRM1                    2U  
#define KRM2                    3U  
#define KRM3                    4U  	 
#define KRM4                    5U  
#define KRM5                    6U  
#define KRM6                    7U  
#define KRM7                    8U  	
#define KRMALL                  0xffU 

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global variables
***********************************************************************************************************************/
extern volatile uint32_t g_keyIntTaken;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void KEY_Init(uint8_t pinMsk);
void KEY_Start(void);
void KEY_Stop(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
