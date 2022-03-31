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
#include <string.h>
#include "BAT32G157.h"

#include "usb_basic_mini_if.h"

#include "drv_timer.h"
#include "drv_usb_hid_cfg.h"
#include "drv_usb_descriptor.h"
#include "drv_usb.h"
/* Private typedef --------------------------------------*/

/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Usb_Handler(void *arg );

/* Private variables ------------------------------------*/
static uint32_t usbDelayMs;
static uint8_t usbStatus = NO_WRITING;           /* HID device is the HID data transfer status or not */

static usb_ctrl_t usbCtrl;
static usb_cfg_t  usbCfg;

static uint8_t usbData[64] = {0};
static uint8_t usbZeroData[64] = {0};

const static usb_descriptor_t usbDescriptor = 
{
    .p_device = (uint8_t *)usb_device_descriptor,
    .p_config_f = (uint8_t *)usb_config_descriptor, 
    .pp_string = (uint8_t **)usb_string_descriptor_table,
    .num_string = NUM_STRING_DESCRIPTOR
};


void Drv_Usb_Init(void )
{
    usbCtrl.type = USB_PHID;
    usbCfg.usb_mode = USB_PERI;
    usbCfg.p_usb_reg = (usb_descriptor_t *)&usbDescriptor;

    Systick_Init();

    memset(usbZeroData, 0, sizeof(usbZeroData));
    
    memset(usbData, 0, sizeof(usbData));
    
    USB_Open(&usbCtrl, &usbCfg);

    Drv_Timer_Run_Period(Drv_Usb_Handler, 0, 1, NULL);
}

void usb_cpu_delay_xms(uint32_t n)
{
    usbDelayMs = n;

    while(usbDelayMs);
}

void usb_cpu_delay_1us (uint16_t time)
{
    uint16_t sf_cnt;
    sf_cnt = time * 20;

    while(sf_cnt)
    {
        sf_cnt --;
    }
}

void Drv_Usb_Delay_Count(void )
{
    if(usbDelayMs > 0)
    {
        usbDelayMs--;
    }
}

static void Drv_Usb_Handler(void *arg )
{
    switch(USB_GetEvent(&usbCtrl))
    {
        case USB_STS_CONFIGURED:
        {
            usbStatus = NO_WRITING;

            break;
        }
        case USB_STS_WRITE_COMPLETE:
        {
            if(usbStatus == DATA_WRITING)
            {
                usbStatus = ZERO_WRITING;

                usbCtrl.type = USB_PHID;
                USB_Write(&usbCtrl, (uint8_t *)usbZeroData, sizeof(usbZeroData));
            }
            else
            {
                usbStatus = NO_WRITING;
            }

            break;
        }
        case USB_STS_REQUEST:
        {
            static uint8_t idleVal;
            
            if(USB_REQUEST_TYPE_CLASS == (usbCtrl.setup.type & USB_REQUEST_TYPE_CLASS))
            {
                if(USB_SET_REPORT == (usbCtrl.setup.type & USB_BREQUEST))
                {

                }

                if (USB_SET_IDLE == (usbCtrl.setup.type & USB_BREQUEST))
                {
                    uint8_t *pIdleVal = NULL;
    
                    pIdleVal = (uint8_t *)&usbCtrl.setup.value;
                    idleVal = pIdleVal[0];
                    usbCtrl.type = USB_REQUEST;
                    usbCtrl.status = USB_ACK;
                    USB_Write(&usbCtrl, (uint8_t*)USB_NULL, (uint32_t)USB_NULL);
                }
    
                if (USB_GET_IDLE == (usbCtrl.setup.type & USB_BREQUEST))
                {
                    usbCtrl.type = USB_REQUEST;
                    USB_Write(&usbCtrl, &idleVal, 1);
                }
                else if (USB_SET_PROTOCOL == (usbCtrl.setup.type & USB_BREQUEST))
                {
                    usbCtrl.type = USB_REQUEST;
                    usbCtrl.status = USB_ACK;
                    USB_Write(&usbCtrl, (uint8_t*)USB_NULL, (uint32_t)USB_NULL);
                }
                else if (USB_GET_PROTOCOL == (usbCtrl.setup.type & USB_BREQUEST))
                {
                    /* Do Nothing */
                }
                else
                {
                    /* Do Nothing */
                }
            }
            else
            {
                if (USB_GET_DESCRIPTOR == (usbCtrl.setup.type & USB_BREQUEST))
                {
                    if (USB_GET_REPORT_DESCRIPTOR == usbCtrl.setup.value)
                    {
                        /* Send Report descriptor */
                        usbCtrl.type = USB_REQUEST;
                        USB_Write (&usbCtrl, (uint8_t *)HID_DeviceReportDescriptor, sizeof(HID_DeviceReportDescriptor));
                    }
                    else if (USB_GET_HID_DESCRIPTOR == usbCtrl.setup.value)
                    {
                        /* HID Discriptor address set. */
                        usbCtrl.type = USB_REQUEST;
                        USB_Write (&usbCtrl, (uint8_t *) &usb_config_descriptor[HID_DESCRIPTOR_INDEX],
                                   HID_DESCRIPTOR_SIZE);
                    }
                    else
                    {
                        /* Do Nothing */
                    }
                }

                #if defined(USE_LPW)
                    /* Do Nothing */
                #endif /* defined(USE_LPW) */
            }

            break;
        }
        case USB_STS_REQUEST_COMPLETE : /* Complete Class Request */
        {
            if (USB_REQUEST_TYPE_CLASS == (usbCtrl.setup.type & USB_REQUEST_TYPE_CLASS))
            {
                if (USB_SET_IDLE == (usbCtrl.setup.type & USB_BREQUEST))
                {
                    /* Do Nothing */
                }
                else if (USB_SET_PROTOCOL == (usbCtrl.setup.type & USB_BREQUEST))
                {
                    /* Do Nothing */
                }
                else
                {
                    /* Do Nothing */
                }
            }

            break;
        }
        case USB_STS_SUSPEND:
        {
            #if defined(USE_LPW)
                /* Do Nothing */
            #endif /* defined(USE_LPW) */
            break;
        }
        case USB_STS_DETACH:
        {
            #if defined(USE_LPW)
                /* Do Nothing */
            #endif /* defined(USE_LPW) */
            break;
        }
        default: break;
    }
}



