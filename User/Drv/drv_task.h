#ifndef _DRV_TASK_H
#define _DRV_TASK_H

#include "hal_task.h"

typedef void (*task_handler_t)(void *);

#define TASK_EMPTY                 (0)

#define TASK_OK                   (0)
#define TASK_ERR                  (!TASK_OK)

typedef enum
{
    TASK_READY = 0,
    TASK_SUSPEND,
    TASK_SLEEP,
    TASK_IDLE
}task_state_t;

typedef struct _task_ctrl_block_t
{
    task_handler_t handler;
    task_state_t   state;
    void *arg;
    uint16_t ticks;
    uint16_t period;
    uint8_t  id;

    struct _task_ctrl_block_t *prev;
    struct _task_ctrl_block_t *next;
}task_ctrl_block_t;

void Drv_Task_Init(void );
uint8_t Drv_Task_Register(task_handler_t handler, uint16_t ticks, uint16_t period, void *arg );
task_ctrl_block_t * Drv_Task_Get(uint8_t id );
uint8_t Drv_Task_Delay(uint8_t id, uint16_t ticks );
uint8_t Drv_Task_Sleep(uint8_t id );
uint8_t Drv_Task_Wakeup(uint8_t id );
task_ctrl_block_t * Drv_Task_Delete(uint8_t id );
void Drv_Task_Schedualer(void );


#endif 

