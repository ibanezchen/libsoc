/*-****************************************************************************/
/*-                                                                           */
/*-            Copyright (c) 2013 by SOCware Inc.                             */
/*-                                                                           */
/*-  This software is copyrighted by and is the sole property of SOCware, Inc.*/
/*-  All rights, title, ownership, or other interests in the software remain  */
/*-  the property of SOCware, Inc. The source code is for FREE short-term     */
/*-  evaluation, educational or non-commercial research only. Any commercial  */
/*-  application may only be used in accordance with the corresponding license*/
/*-  agreement. Any unauthorized use, duplication, transmission, distribution,*/
/*-  or disclosure of this software is expressly forbidden.                   */
/*-                                                                           */
/*-  Knowledge of the source code may NOT be used to develop a similar product*/
/*-                                                                           */
/*-  This Copyright notice may not be removed or modified without prior       */
/*-  written consent of SOCware, Inc.                                         */
/*-                                                                           */
/*-  SOCWare, Inc. reserves the right to modify this software                 */
/*-  without notice.                                                          */
/*-                                                                           */
/*-             socware.help@gmail.com                                        */
/*-             http://socware.net                                            */
/*-                                                                           */
/*-****************************************************************************/
#include <hcos/tmr.h>
#include <hcos/irq.h>
#include <hcos/mut.h>
#include <hcos/mq.h>
#include <hcos/io.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#if HAL

#include "os_util.h"
#include "_soc.h"
#include "term.h"
#include "free-rtos.h"

#define portSTACK_TYPE	unsigned
#define pdPASS		( 1 )
#define pdFAIL		( 0 )
#define errQUEUE_EMPTY	( ( BaseType_t ) 0 )
#define errQUEUE_FULL	( ( BaseType_t ) 0 )

#define	 queueSEND_TO_BACK		( ( BaseType_t ) 0 )
#define	 queueSEND_TO_FRONT		( ( BaseType_t ) 1 )
#define queueOVERWRITE			( ( BaseType_t ) 2 )

int __exidx_start, __exidx_end;

void vPortFree( void *pv )
{
	_printf("free %x\n", (unsigned)pv);
	free(pv);
}

void *pvPortMalloc( size_t xWantedSize )
{
	void* p = malloc(xWantedSize);
	_printf("malloc %x\n", (unsigned)p);
	return p;
}

void *pvPortCalloc( size_t nmemb, size_t size )
{
	void* p = calloc(nmemb, size);
	_printf("calloc %x\n", (unsigned)p);
	return p;
}

void *pvPortRealloc( void *pv, size_t size )
{
	void* p = realloc(pv, size);
	_printf("realloc %x\n",(unsigned)pv);
	return p;
}

#define queueQUEUE_TYPE_MUTEX 				( ( uint8_t ) 1U )
void* xQueueGenericCreate( const long uxQueueLength, const long uxItemSize, 
	const unsigned char ucQueueType )
{
	unsigned buf_sz, no_words, * buf, token;
	void*p;
	mq_t* m;
	
	if(!uxItemSize)
		no_words = 1;
	else
		no_words = (BI_RUP(uxItemSize, 2))>>2;
	buf_sz = sizeof(mq_t) + uxQueueLength * (no_words << 2);
	p      = pvPortMalloc(buf_sz);
	m      = (mq_t*)p;
	buf    = (unsigned*)(m+1);
	mq_init(m, no_words, buf, buf_sz);
	if(ucQueueType == queueQUEUE_TYPE_MUTEX){
		token = 0xbeefbeef;
		mq_put(m, &token, 0);
	}
	return m;
}

void* xQueueCreateMutex( const uint8_t ucQueueType )
{
	return xQueueGenericCreate(1, 0, queueQUEUE_TYPE_MUTEX);
}

void vQueueDelete( void* xQueue )
{
	vPortFree(xQueue);
}

BaseType_t xQueueGenericSend( QueueHandle_t xQueue, void * const pvItemToQueue, TickType_t xTicksToWait, const BaseType_t xCopyPosition )
{
	mq_t* m = (mq_t*)xQueue;
	if(xCopyPosition != queueSEND_TO_BACK)
		_fail(cpu_cur_pc(),0,__LINE__);
	printf("xQueueGenericSend %s\r\n", _task_cur->name);
	return !mq_put(m, pvItemToQueue, xTicksToWait);
}

BaseType_t xQueueGenericSendFromISR( QueueHandle_t xQueue,  void * const pvItemToQueue, BaseType_t * const pxHigherPriorityTaskWoken, const BaseType_t xCopyPosition )
{
	mq_t* m = (mq_t*)xQueue;
	if(xCopyPosition != queueSEND_TO_BACK)
		_fail(cpu_cur_pc(),0,__LINE__);
	if(pxHigherPriorityTaskWoken)
		*pxHigherPriorityTaskWoken = !ll_empty(&m->waitg);
	printf("xQueueGenericSendFromISR\r\n");
	return !mq_put(m, pvItemToQueue, 0);
}

BaseType_t xQueueGiveFromISR( QueueHandle_t xQueue, BaseType_t * const pxHigherPriorityTaskWoken )
{
	long dummy = 0;
	printf("xQueueGiveFromISR\r\n");
	return xQueueGenericSendFromISR(xQueue, &dummy, pxHigherPriorityTaskWoken, queueSEND_TO_BACK);
}

BaseType_t xQueueGenericReceive( QueueHandle_t xQueue, void * const pvBuffer, TickType_t xTicksToWait, const BaseType_t xJustPeeking )
{
	mq_t* m = (mq_t*)xQueue;
	printf("xQueueGenericReceive %s\r\n", _task_cur->name);
	if(xJustPeeking)
		return !mq_peek(m, pvBuffer, xTicksToWait);
	else
		return !mq_get(m, pvBuffer, xTicksToWait);
}

BaseType_t xQueueReceiveFromISR( QueueHandle_t xQueue, 
	void * const pvBuffer, BaseType_t * const pxHigherPriorityTaskWoken )
{
	mq_t* m = (mq_t*)xQueue;
	if(pxHigherPriorityTaskWoken)
		*pxHigherPriorityTaskWoken = !ll_empty(&m->waitg);
	printf("xQueueGenericReceiveFromISR\r\n");
	return xQueueGenericReceive(xQueue, pvBuffer, 0, 0);
}

void vQueueAddToRegistry( QueueHandle_t xQueue, const char *pcQueueName )
{}

UBaseType_t uxQueueSpacesAvailable( const QueueHandle_t xQueue )
{
	mq_t* m = (mq_t*)xQueue;
	return m->sz - m->n;
}

UBaseType_t uxQueueMessagesWaiting( const QueueHandle_t xQueue )
{
	mq_t* m = (mq_t*)xQueue;
	return m->n;
}

void vTaskDelay( const unsigned xTicksToDelay )
{
	task_sleep(xTicksToDelay/10);
}

char* pcTaskGetTaskName(void* _task)
{
	task_t* task = (task_t*)_task;
	return (char*)task->name;
}

void* xTaskGetCurrentTaskHandle( void )
{
	return _task_cur;	
}

unsigned xTaskGetTickCount( void )
{
	return tmr_ticks;
}

#define taskSCHEDULER_SUSPENDED		(  0 )
#define taskSCHEDULER_NOT_STARTED	(  1 )
#define taskSCHEDULER_RUNNING		(  2 )

long xTaskGetSchedulerState( void )
{
	return taskSCHEDULER_NOT_STARTED;
}

void vTaskPrioritySet( TaskHandle_t xTask, UBaseType_t uxNewPriority )
{
	_printf("vTaskPrioritySet\r\n");
	task_pri(xTask, uxNewPriority );
}

static void gc(task_t * t)
{
	_printf("gc\r\n");
	vPortFree(t->stack);
	vPortFree(t);
}

void vTaskDelete( void* xTaskToDelete )
{
	task_t* t = (task_t*)xTaskToDelete;
	_printf("vTaskDelete\r\n");
	_task_dest(t);
	if( t == _task_cur)
		task_yield();
}

BaseType_t xTaskGenericCreate( TaskFunction_t pxTaskCode, 
	const char * const pcName, 
	const uint16_t usStackDepth, 
	void * const pvParameters, 
	UBaseType_t uxPriority, 
	TaskHandle_t * const pxCreatedTask, 
	StackType_t * puxStackBuffer, 
	const MemoryRegion_t * const xRegions )
{
	task_t* t;
	unsigned stack_sz;
	
	if( !task_gc )
		task_gc = gc;
	t = pvPortMalloc(sizeof(task_t));
	stack_sz = sizeof(StackType_t) * usStackDepth;
	if(puxStackBuffer == 0);
		puxStackBuffer = pvPortMalloc(stack_sz);
	
	_printf("xTaskGenericCreate %s\r\n", pcName);
	*pxCreatedTask = task_init(
		t, pcName, pxTaskCode, uxPriority, puxStackBuffer, 
		stack_sz, 1, pvParameters);
	 
	return 1;
}

void vPortEnterCritical(void)
{
	irq_lock();
}

void vPortExitCritical( void )
{
	irq_unlock();
}
#endif
