/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    wdt.h
* @brief   This file implements device driver for WDT module.
* @version 1.0.0
* @date    2020/10/24
***********************************************************************************************************************/
#ifndef WDT_H
#define WDT_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global variables
***********************************************************************************************************************/
extern volatile uint32_t g_WdtIntTaken;   	/* WDT interrupt flag */

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void WDT_Restart(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
