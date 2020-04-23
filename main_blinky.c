/*
 * FreeRTOS Kernel V10.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

 /* Standard includes. */
#include <stdio.h>
#include <conio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"

/* Priorities at which the tasks are created. */
#define taskA_PRIORITY		( tskIDLE_PRIORITY + 3 )
#define	taskB_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	taskC_PRIORITY		( tskIDLE_PRIORITY + 1 )

/*-----------------------------------------------------------*/
#define BIT_0	( 1 << 0 )
#define BIT_4	( 1 << 4 )



TaskHandle_t xHandle_A = NULL;
TaskHandle_t xHandle_B = NULL;
TaskHandle_t xHandle_C = NULL;

EventGroupHandle_t xEventGroup;
/*
 * The tasks as described in the comments at the top of this file.
 */
static void taskA(void* pvParameters);
static void taskB(void* pvParameters);
static void taskC(void* pvParameters);


void main_blinky(void)
{ 
    xTaskCreate(taskA,			/* The function that implements the task. */
        "Rx", 							/* The text name assigned to the task - for debug only as it is not used by the kernel. */
        configMINIMAL_STACK_SIZE, 		/* The size of the stack to allocate to the task. */
        NULL, 							/* The parameter passed to the task - not used in this simple case. */
        taskA_PRIORITY,/* The priority assigned to the task. */
        &xHandle_A);		/*Handle to task A*/

    xTaskCreate(taskB, "TX", configMINIMAL_STACK_SIZE, NULL, taskB_PRIORITY, &xHandle_B);/*Handle to task B*/

    xTaskCreate(taskC, "ZX", configMINIMAL_STACK_SIZE, NULL, taskC_PRIORITY, NULL);	/* The task handle is not required, so NULL is passed. */

    xEventGroup = xEventGroupCreate();
    vTaskStartScheduler();

    for (;; );
}
/*----------------------------------------------------------*/


static void taskA(void* pvParameters)
{
    /* Prevent the compiler warning about the unused parameter. */
    (void)pvParameters;    
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 2000;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        printf("In task A\n");
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        xEventGroupSetBits(
            xEventGroup,    /* The event group being updated. */
            BIT_0 | BIT_4);
        vTaskDelay(600);
    }
}

static void taskB(void* pvParameters)
{
    /* Prevent the compiler warning about the unused parameter. */
    (void)pvParameters;
    EventBits_t uxBits;

    for (;; )
    {

        printf("   In task B\n");
      //  BIT_0 | BIT_4, /* The bits within the event group to wait for. */
            uxBits = xEventGroupWaitBits(
                xEventGroup,   /* The event group being tested. */
                BIT_0 | BIT_4,
                pdTRUE,        /* BIT_0 & BIT_4 should be cleared before returning. */
                pdTRUE,
                500);
        if ((uxBits & (BIT_0 | BIT_4)) == (BIT_0 | BIT_4))
        {
            /* xEventGroupWaitBits() returned because both bits were set. */
            printf("   In task B: event has happend\n");
        }
        else
        {
            /* xEventGroupWaitBits() returned because xTicksToWait ticks passed*/
             printf("   In task B: timeout\n");
         
        }
       
        vTaskDelay(500);
    }
}



static void taskC(void* pvParameters)
{
    /* Prevent the compiler warning about the unused parameter. */
    (void)pvParameters;

    for (;; )
    {
        printf("      In task C\n");
        vTaskDelay(5000);
    }
}

static void idelWork(int t) {
    //do some idel work
    for (int i = 0; i < 20000; i++) {
        for (int j = 0; j < 2000; j++)
            for (int r = 0; r < t; r++)
                ;
    }
}

/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/


