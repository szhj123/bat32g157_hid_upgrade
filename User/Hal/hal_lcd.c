/********************************************************
* @file       hal_lcd.c
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

#include "hal_lcd.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
#define DMA_CTRL_DATA_LCDB                0

/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
lcdb_callback_t lcdb_callback = NULL;
static uint8_t lcdb_sent_cnt;
static uint32_t lcdb_dma_size;
static uint16_t lcd_rgb_color;

void LCDB_Init(LCDB_Typedef* pLcdb)
{
   CGC->PER1 |= CGC_PER1_LCDBEN_Msk;	/*enable lcdb clock*/
	
   if(LCDB_MODE_TYPE_8080 == pLcdb->mode)
   {
       LCDB->LBCTL &= ~LCDB_LBCTL_IMD_Msk;
   }
   else
   {
   	   LCDB->LBCTL |= LCDB_LBCTL_IMD_Msk;
  	   if(LCDB_MODE_6800_CNTL_ENABLE_HIGH == pLcdb->mode_6800_E_level)
  	   {
  	       LCDB->LBCTL &= ~LCDB_LBCTL_EL_Msk;
  	   }
  	   else
       {
           LCDB->LBCTL |= LCDB_LBCTL_EL_Msk;
       }
   }
   
   LCDB->LBCTL &= ~LCDB_LBCTL_LBC_Msk;//lcdb clock divider
   LCDB->LBCTL |= (pLcdb->clock_div)<<LCDB_LBCTL_LBC_Pos;
   
   LCDB->LBCYC = pLcdb->cycle;
   
   LCDB->LBWST = pLcdb->wait_cycle;
   
   /* Set LCDB pin */
   PORT->PC &= ~0xF3C0;//PC6-9 PC12-15
   PORT->PC |= 0x0C00;//PC10-11
   
   PORT->PMCC &= ~(0x3FF<<6);//PC6-15 digital function
   PORT->PMC &= ~0x0C00;//PC10-11 output mode
   PORT->PMC |= 0xF3C0;//PC6-9 PC12-15 input mode
   
   /* PA02£ºTFT-RST, PA04:TFT-BLK£¬ PA10:TFT-CS, PA14:T-CLK, PD0: TFT-RS,  */ 
   PORT->PMA &= ~(0x4414);	//PA02, PA04, PA10£¬PA11, output mode
   PORT->PMCA &= ~(0x4414);	//PA02, PA04, PA10£¬PA11, digital function
   PORT->PMD &= 0xFFFE;	//PD00 output mode
   
   INTC_DisableIRQ(LCDB_IRQn);      /* disable LCDB interrupt */
   INTC_ClearPendingIRQ(LCDB_IRQn); /* clear LCDB interrupt flag */
}

void LCDB_Start(void)
{
    INTC_ClearPendingIRQ(LCDB_IRQn); /* clear LCDB interrupt flag */
    INTC_EnableIRQ(LCDB_IRQn);/* enable LCDB interrupt */
}

void LCDB_Stop(void)
{
    INTC_DisableIRQ(LCDB_IRQn);      /* disable LCDB interrupt */
    INTC_ClearPendingIRQ(LCDB_IRQn); /* clear LCDB interrupt flag */
}

void LCDB_Wr_DMA(const uint8_t *buf, uint32_t size, lcdb_callback_t callback )
{
    lcdb_dma_size = size;
    
    DMAVEC->VEC[DMA_VECTOR_LCDB] = DMA_CTRL_DATA_LCDB;
    DMAVEC->CTRL[DMA_CTRL_DATA_LCDB].DMACR = (DMA_SIZE_HALF << CTRL_DMACR_SZ_Pos) | (1 << CTRL_DMACR_RPTINT_Pos) |
                                             (0 << CTRL_DMACR_DAMOD_Pos)  | (1 << CTRL_DMACR_SAMOD_Pos) |
                                             (0 << CTRL_DMACR_RPTSEL_Pos)| (DMA_MODE_NORMAL << CTRL_DMACR_MODE_Pos);
    DMAVEC->CTRL[DMA_CTRL_DATA_LCDB].DMBLS = 1;
    DMAVEC->CTRL[DMA_CTRL_DATA_LCDB].DMACT = size/2-1;
    DMAVEC->CTRL[DMA_CTRL_DATA_LCDB].DMRLD = size/2-1;
    DMAVEC->CTRL[DMA_CTRL_DATA_LCDB].DMSAR = (uint32_t)((uint16_t *)buf);
    DMAVEC->CTRL[DMA_CTRL_DATA_LCDB].DMDAR = (uint32_t)((uint16_t*)&LCDB->LBDATA);

    /* init DMA registers */
    CGC->PER1   |= CGC_PER1_DMAEN_Msk;
    DMA->DMABAR  = DMAVEC_BASE;
    DMA_Enable(DMA_VECTOR_LCDB);
	LCDB_Start();

    lcdb_callback = callback;

    lcdb_sent_cnt = 1;

    LCDB->LBDATA = *(uint16_t *)&buf[0];
}

void LCDB_Wr_DMA_Fix_SrcAddr(const uint16_t *u16Data, uint32_t size, lcdb_callback_t callback )
{
    lcdb_dma_size = size;
    
    DMAVEC->VEC[DMA_VECTOR_LCDB] = DMA_CTRL_DATA_LCDB;
    DMAVEC->CTRL[DMA_CTRL_DATA_LCDB].DMACR = (DMA_SIZE_HALF << CTRL_DMACR_SZ_Pos) | (1 << CTRL_DMACR_RPTINT_Pos) |
                                             (0 << CTRL_DMACR_DAMOD_Pos)  | (0 << CTRL_DMACR_SAMOD_Pos) |
                                             (0 << CTRL_DMACR_RPTSEL_Pos)| (DMA_MODE_NORMAL << CTRL_DMACR_MODE_Pos);
    DMAVEC->CTRL[DMA_CTRL_DATA_LCDB].DMBLS = 1;
    DMAVEC->CTRL[DMA_CTRL_DATA_LCDB].DMACT = size/2-1;
    DMAVEC->CTRL[DMA_CTRL_DATA_LCDB].DMRLD = size/2-1;
    DMAVEC->CTRL[DMA_CTRL_DATA_LCDB].DMSAR = (uint32_t)(u16Data);
    DMAVEC->CTRL[DMA_CTRL_DATA_LCDB].DMDAR = (uint32_t)((uint16_t*)&LCDB->LBDATA);

    /* init DMA registers */
    CGC->PER1   |= CGC_PER1_DMAEN_Msk;
    DMA->DMABAR  = DMAVEC_BASE;
    DMA_Enable(DMA_VECTOR_LCDB);
	LCDB_Start();

    lcdb_callback = callback;
    lcdb_sent_cnt = 0;

    LCDB->LBDATA = *u16Data;
}

void LCDB_Isr_Handler(void )
{
    if(++lcdb_sent_cnt == 1)
    {
        DMAVEC->CTRL[DMA_CTRL_DATA_LCDB].DMACT = lcdb_dma_size/2-1;
		DMA_Enable(DMA_VECTOR_LCDB);
		LCDB->LBDATA = lcd_rgb_color;
    }
    else
    {
        if(lcdb_callback != NULL)
        {
            lcdb_callback();
            
            lcdb_callback  = NULL;
        }
        LCD_CS_HIGH();
    		
    	LCDB_Stop();
    }
}

