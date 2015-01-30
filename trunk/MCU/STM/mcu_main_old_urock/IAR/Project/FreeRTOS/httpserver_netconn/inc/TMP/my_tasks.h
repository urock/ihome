#ifndef __MY_TASKS_H__
#define __MY_TASKS_H__

#include "init_const.h"
#include "main.h"
#include "init_ports.h"
#include "set_ports.h"
#include "stm322xg_eval.h"
#include "parsing_html.h"
#include "lwip/opt.h"
#include "send2host.h"
#include "stm32f2xx_gpio.h"
#include <stdlib.h>
#include <stdio.h>
#include "onewire.h"
#include "adc_sensors.h"
#include "pulse.h"
#include "task.h"


/**
* @brief Header Function tasks
*/
void my_task_TaskList (void *dev);
void my_tasks_vConfigPort (void * dev);
void my_tasks_vConfigPort_init (void * dev);
void my_tasks_vReceiveButton (void *pt);
void my_tasks_vReceiveButton_init (void *pt);
void my_task_vGetTemperature (void * dev);
void vADC_sensor (void *dev);
void vPWM (void *dev);
#endif /* __MY_TASKS_H__ */