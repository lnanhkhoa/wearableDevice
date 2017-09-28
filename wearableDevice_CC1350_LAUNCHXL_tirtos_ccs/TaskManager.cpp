/*
 * TaskManager.cpp
 *
 *  Created on: Sep 22, 2017
 *      Author: AppsTI
 */



/***** Includes *****/
#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include "TaskManager.h"

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Clock.h>

#include <ti/drivers/GPIO.h>
#include "Board.h"

static uint8_t taskStack[TASK_STACK_SIZE];

Event_Struct concentratorEvent;  /* not static so you can see in ROV */
static Event_Handle concentratorEventHandle;
Clock_Struct clk0Struct;

TaskManager::TaskManager()
{
    GPIO_init();
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_OFF);

}

TaskManager::~TaskManager()
{

}





void hello_world(UArg arg0)
{
    Uint32 second = arg0*1000000;
    Task_sleep(second/Clock_tickPeriod);
    GPIO_toggle(Board_GPIO_LED0);
//    Task_exit();
//    Event_post(concentratorEventHandle, RADIO_EVENT_VALID_PACKET_RECEIVED);
    // Event_pend(concentratorEventHandle, 0, RADIO_EVENT_ALL, BIOS_WAIT_FOREVER);
}

void hello_world1()
{
    UInt key = Task_disable();
    GPIO_toggle(Board_GPIO_LED1);
    Task_restore(key);
//    Event_post(concentratorEventHandle, RADIO_EVENT_VALID_PACKET_RECEIVED);

}


void TaskManager::create_repeatThread(uint8_t sec)
{
    static Task_Params taskParams;
    static Task_Struct taskStruct;
    bool stateTask = true;
    // Task_destruct(&taskStruct);
    if(stateTask){
        Task_Params_init(&taskParams);
        taskParams.stackSize = TASK_STACK_SIZE;
        taskParams.stack = &taskStack;
        taskParams.arg0 = sec;
        Task_construct(&taskStruct, (Task_FuncPtr)hello_world, &taskParams, NULL);
    }else{
        Task_destruct(&taskStruct);
    }
    stateTask != stateTask;

    // taskHandle = Task_Handle(&taskStruct);
    // Task_create((Task_FuncPtr)hello_world, &taskParams);
}

void TaskManager::create_Clock()
{
    Clock_Params clkParams;
    Clock_Params_init(&clkParams);
    clkParams.period = 100000;
    clkParams.startFlag = true;
    Clock_construct(&clk0Struct, (Clock_FuncPtr)hello_world1, 1, &clkParams);


}


void TaskManager::create_Event()
{

    /* Create event used internally for state changes */
        Event_Params eventParam;
        Event_Params_init(&eventParam);
        Event_construct(&concentratorEvent, &eventParam);
        concentratorEventHandle = Event_handle(&concentratorEvent);
    /* Wait for event */

}


