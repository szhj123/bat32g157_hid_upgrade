/********************************************************
* @file       drv_usb.c
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

#include "usb_basic_mini_if.h"

#include "drv_timer.h"
#include "drv_usb_descriptor.h"
#include "drv_usb.h"
/* Private typedef --------------------------------------*/

/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

const static usb_descriptor_t usbDescriptor = 
{
    .p_device = (uint8_t *)usb_device_descriptor,
    .p_config_f = (uint8_t *)usb_config_descriptor, 
    .pp_string = (uint8_t **)usb_string_descriptor_table,
    .num_string = NUM_STRING_DESCRIPTOR
};


void Drv_Usb_Init(void )
{
    usb_ctrl_t usbCtrl;
    usb_cfg_t  usbCfg;

    uint8_t    *p_idle_value;

    usb_info_t usbInfo;

    usbCtrl.type = USB_PHID;
    usbCfg.usb_mode = USB_PERI;
    usbCfg.p_usb_reg = (usb_descriptor_t *)&usbDescriptor;
    
    USB_Open(&usbCtrl, &usbCfg);
}

