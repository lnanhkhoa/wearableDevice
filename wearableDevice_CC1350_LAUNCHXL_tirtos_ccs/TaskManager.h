/*
 * TaskManager.h
 *
 *  Created on: Sep 22, 2017
 *      Author: lnanhkhoa
 */

#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include "stdint.h"

#define TASK_STACK_SIZE 1024
#define TASK_PRIORITY   3

#define RADIO_EVENT_ALL                  0xFFFFFFFF
#define RADIO_EVENT_VALID_PACKET_RECEIVED      (uint32_t)(1 << 0)

class TaskManager
{
public:
    TaskManager();
    virtual ~TaskManager();
    void create_repeatThread(uint8_t sec);
    void create_Clock();
    void create_Event();

};

#endif /* TASKMANAGER_H_ */
