/********************************************************
* @file       drv_lcd.c
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
#include "BAT32G157.h"
#include "cm_backtrace.h"

#include "drv_lcd.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
static LCDB_Typedef Lcdb =
{
    .mode = LCDB_MODE_TYPE_8080,    /* 8080 Mode */
    .clock_div = LCDB_CLOCK_DIV_1,	/* 1 divider */
    .cycle = LDB_CYCLE_8T,          /* 6T cycles */
    .wait_cycle = LDB_WAIT_CYCLE_2T	/* 2T wait cycles */
};

void Drv_Lcd_Init(void )
{
    LCDB_Init(&Lcdb);
}

