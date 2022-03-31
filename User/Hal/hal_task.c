/********************************************************
* @file       hal_task.c
* @author     szhj13
* @version    V1.0
* @date       2021-08-12
* @brief      the entry of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "BAT32G157.h"

#include "hal_task.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
hal_task_callback_t hal_task_callback = NULL;

void Hal_Task_Init(hal_task_callback_t callback )
{
    hal_task_callback = callback;

    Systick_Init();
}

void Hal_Task_Isr_Handler(void )
{
    if(hal_task_callback != NULL)
    {
        hal_task_callback();
    }
}


