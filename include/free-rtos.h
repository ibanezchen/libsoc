#ifndef FREE_RTOS0408
#define FREE_RTOS0408

#include <hcos/task.h>

typedef unsigned char uint8_t;

typedef unsigned short uint16_t;

typedef void *TaskHandle_t;

typedef long BaseType_t;

typedef unsigned long UBaseType_t;

typedef void *QueueHandle_t;

typedef unsigned TickType_t;

typedef unsigned StackType_t;

typedef long BaseType_t;

typedef unsigned long UBaseType_t;

typedef task_ent TaskFunction_t;

typedef void *TaskHandle_t;

typedef void *MemoryRegion_t;

typedef void *SemaphoreHandle_t;

#define pdFALSE		0

BaseType_t xQueueGenericSendFromISR(QueueHandle_t xQueue,
				    void *const pvItemToQueue,
				    BaseType_t *
				    const pxHigherPriorityTaskWoken,
				    const BaseType_t xCopyPosition);

BaseType_t xQueueGiveFromISR(QueueHandle_t xQueue,
			     BaseType_t * const pxHigherPriorityTaskWoken);

#define xSemaphoreGiveFromISR( xSemaphore, pxHigherPriorityTaskWoken )	xQueueGiveFromISR( ( QueueHandle_t ) ( xSemaphore ), ( pxHigherPriorityTaskWoken ) )

void vTaskDelay(const TickType_t xTicksToDelay);

#endif
