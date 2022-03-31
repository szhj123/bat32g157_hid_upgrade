/********************************************************
* @file       drv_task.c
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

#include "drv_task.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Task_Isr_Callback(void );
/* Private variables ------------------------------------*/
static task_ctrl_block_t *taskHead = NULL;

void Drv_Task_Init(void )
{
    Hal_Task_Init(Drv_Task_Isr_Callback);
}

uint8_t Drv_Task_Register(task_handler_t handler, uint16_t ticks, uint16_t period, void *arg )
{
    static uint8_t id;
    task_ctrl_block_t *task = (task_ctrl_block_t *)malloc(sizeof(task_ctrl_block_t));

    if(task == NULL)
    {
        return TASK_EMPTY;
    }

    if(id < 0xff)
    {
        id++;
    }

    task->handler = handler;
    task->arg = arg;
    task->ticks = ticks;
    task->period = period;
    task->id = id;
    
    if(task->ticks)
    {
        task->state = TASK_SUSPEND;
    }
    else
    {
        task->state = TASK_READY;
    }

    task->prev = NULL;
    task->next = NULL;

    if(taskHead == NULL)
    {
        taskHead = task;
    }
    else
    {
        task_ctrl_block_t *pTask = taskHead;

        while(pTask->next != NULL)
        {
            pTask = pTask->next;
        }

        pTask->next = task;
        task->prev = pTask;
    }

    return id;
}

task_ctrl_block_t * Drv_Task_Get(uint8_t id )
{
    task_ctrl_block_t *pTask = taskHead;
   
    while(pTask != NULL)
    {
        if(pTask->id == id)
        {
            return pTask;
        }

        pTask = pTask->next;
    }

    return NULL;
}


uint8_t Drv_Task_Delay(uint8_t id, uint16_t ticks )
{
    task_ctrl_block_t *pTask = NULL;
    if(id == TASK_EMPTY)
    {
        return TASK_ERR;
    }
    
    pTask = Drv_Task_Get(id);

    if(pTask != NULL)
    {
        pTask->ticks = ticks;
        
        pTask->state = TASK_SUSPEND;
       
        return TASK_OK;
    }
    else
    {
        return TASK_ERR;
    }
}

uint8_t Drv_Task_Sleep(uint8_t id )
{
    task_ctrl_block_t *pTask  = NULL;
    
    if(id == TASK_EMPTY)
    {
        return TASK_ERR;
    }

    pTask = Drv_Task_Get(id);

    pTask->state = TASK_SLEEP;

    return TASK_OK;
}

uint8_t Drv_Task_Wakeup(uint8_t id )
{
    task_ctrl_block_t *pTask = NULL;

    if(id == TASK_EMPTY)
    {
        return TASK_ERR;
    }

    pTask = Drv_Task_Get(id);

    pTask->state = TASK_READY;

    return TASK_OK;
    
}

task_ctrl_block_t * Drv_Task_Delete(uint8_t id )
{
    task_ctrl_block_t *pTask = NULL;
    task_ctrl_block_t *retTask = NULL;

    if(id == TASK_EMPTY)
    {
        return NULL;
    }

    pTask = Drv_Task_Get(id);

    if(pTask == taskHead)
    {
        if(taskHead->next == NULL)
        {
            taskHead = NULL;
        }
        else
        {
            taskHead = pTask->next;

            taskHead->prev = NULL;
        }

        retTask = taskHead;
    }
    else if(pTask->next == NULL)
    {
        pTask->prev->next = NULL;

        retTask = pTask->prev;
    }
    else
    {
        pTask->prev->next = pTask->next;
        pTask->next->prev = pTask->prev;

        retTask = pTask->prev;
    }

    free(pTask);

    pTask->handler = NULL;
    pTask->arg = NULL;
    pTask->id = TASK_EMPTY;
    pTask->ticks = 0;
    pTask->period = 0;
    pTask->state = TASK_IDLE;

    pTask = NULL;

    return retTask;    
}

void Drv_Task_Schedualer(void )
{
    task_ctrl_block_t *pTask = taskHead;

    while(pTask != NULL)
    {
        if(pTask->id != TASK_EMPTY)
        {
            if(pTask->state == TASK_READY)
            {
                pTask->handler(pTask->arg);
                
                pTask->ticks = pTask->period;
    
                pTask->state = TASK_SUSPEND;
            }
        }
        
        pTask = pTask->next;
    }
}

static void Drv_Task_Isr_Callback(void )
{
    task_ctrl_block_t *pTask = taskHead;

    while(pTask != NULL)
    {
        if(pTask->id != TASK_EMPTY)
        {
            if(pTask->state == TASK_SUSPEND)
            {
                if(pTask->ticks > 0)
                {
                    pTask->ticks--;
                }

                if(pTask->ticks == 0)
                {
                    pTask->state = TASK_READY;
                }
            }
        }

        pTask = pTask->next;
    }
}

