/********************************************************
* @file       main.c
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

#include "drv_task.h"
#include "drv_timer.h"
#include "drv_ili9341.h"
#include "usb_phid_apl.h"
#include "cm_backtrace.h"

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_apps\demo\demo.h"
#include "lv_tests\lv_test_theme\lv_test_theme_1.h"
#include "lv_tests\lv_test_theme\lv_test_theme_2.h"

/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
void usb_cpu_delay_1us (uint16_t time)
{
    uint16_t sf_cnt;
    sf_cnt = time * 20;

    while(sf_cnt)
    {
        sf_cnt --;
    }
}

void usb_cpu_delay_xms(uint16_t n)
{
    uint16_t i;

    for(i=0;i<n;i++)
    {
        usb_cpu_delay_1us(1000);
    }
}

void fault_test_by_div0(void )
{
    uint8_t buf[4] = {0};

    uint32_t *u32Ptr = (uint32_t *)&buf[1];

    *u32Ptr = 0x55;
}


int main(void )
{
    Drv_Task_Init();

    Drv_Timer_Init();

    ili9341_Init();
	
	Usb_Init(); /* USB sample application */

	lv_init();		
    
	lv_port_disp_init();	
	

	lv_obj_t * label; 
	lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL); 
	lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, 0); 
	label = lv_label_create(btn1, NULL); 
	lv_label_set_text(label, "Button"); 
	

    cm_backtrace_init("CmBacktrace", "0.0.1", "0.0.1");
    
	while(1)
	{
	    lv_task_handler();
	}
}

/* USER CODE BEGIN 1 */ 
/* USER CODE END 1 */ 
/* MCU Configuration----------------------------------------------------------*/ 
/* Reset of all peripherals, Initializes the Flash interface and the Systick. */ 
HAL_Init(); 
/* USER CODE BEGIN Init */ 
/* USER CODE END Init */ 
/* Configure the system clock */ 
SystemClock_Config(); 
/* USER CODE BEGIN SysInit */ 
/* USER CODE END SysInit */ 
/* Initialize all configured peripherals */ 
MX_GPIO_Init(); MX_SPI2_Init(); 
MX_USART1_UART_Init(); 
/* USER CODE BEGIN 2 */ 
lv_init(); 
lv_port_disp_init(); 
printf("Welcome to LVGLrn"); 
//建立一个label 
lv_obj_t * label; 
lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL); 
lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, 0); 
label = lv_label_create(btn1, NULL); 
lv_label_set_text(label, "Button"); 
/* USER CODE END 2 */ 
/* Infinite loop */ 
/* USER CODE BEGIN WHILE */ 
while (1) 
{ 
    /* USER CODE END WHILE */ 
    /* USER CODE BEGIN 3 */ 
    //循环调用lv_task处理句柄 
    lv_task_handler(); 
} /* USER CODE END 3 */





