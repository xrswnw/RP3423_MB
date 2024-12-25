/*
 * FreeRTOS V202212.01
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
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H


/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* Ensure stdint is only used by the compiler, and not the assembler. */
#ifdef __ICCARM__
	#include <stdint.h>
	#include "../../inc/Rtos_HL.h"
	extern uint32_t SystemCoreClock;
    #define Sys_LogInit()           Log_Init()
   
#endif /* __ICCARM__ */


#define WORKAROUND_PMU_CM001					1

#define configUSE_PREEMPTION					1									//1:��ռʽ���ȣ�0:Эͬʽ����
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1                                   //���ȼ������㷨��1:Ӳ���㷨���ȣ����ʿ죬���֧�����ȼ�31��0:����㷨���ȣ����ȼ����ޣ����������
#define configUSE_TIME_SLICING					1									//ͬ���ȼ�����ִ��
#define configIDLE_SHOULD_YIELD					1									//���������Ƿ��ò�(ÿִ��һ�ο������񣬲鿴һ���Ƿ��и����ȼ�����)

#define configSUPPORT_DYNAMIC_ALLOCATION		1									//ʹ�ö�̬���񴴽���ʽ����ջ��ϵͳ���䣩
#define configSUPPORT_STATIC_ALLOCATION			0									//��ʹ�þ�̬���񴴽���ʽ����ջ������䣩��ʹ�ܿ������񡢶�ʱ��(��ѡ)�����

#define configUSE_IDLE_HOOK						0									//����������
#define configUSE_TICK_HOOK						0                                   //�δ�ʱ������
#define configCPU_CLOCK_HZ						SYSCLK//( SystemCoreClock )					//��ӦMCU��Ƶ
#define configTICK_RATE_HZ						( ( TickType_t ) 1000 )				//ϵͳ����ʱ��������Ӧ�޸ģ����������ٶȣ�ʱ��Ƭ��
#define configMAX_PRIORITIES					( 31 )                              //32λ���ȼ�
#define configMINIMAL_STACK_SIZE				( ( unsigned short ) 128  + 172)     //���������ջ��С      ����λ �� �֣�
#define configTOTAL_HEAP_SIZE					( ( size_t ) ( 30 * 1024 ) )		//�ѿռ�
#define configMAX_TASK_NAME_LEN					( 20 )								//��������������
#define configUSE_TRACE_FACILITY				1                                   //���ӻ���������,��ȡ����״̬��Ϣ
#define configUSE_16_BIT_TICKS					0                                   //�δ�ʱ����Χ��1 ��u16; 0 : u32��ͬʱ�����¼���־����
#define configTICK_TYPE_WIDTH_IN_BITS           TICK_TYPE_WIDTH_32_BITS             //�δ�s��ִ�еĴ�����32λ
#define configUSE_TICKLESS_IDLE                 0                                   //�͹���ģʽ
#define configUSE_MUTEXES						1                                   //�������ź���
#define configQUEUE_REGISTRY_SIZE				8
//#define configCHECK_FOR_STACK_OVERFLOW			2
#define configUSE_RECURSIVE_MUTEXES				1
#define configUSE_MALLOC_FAILED_HOOK			0                                   //malloc�����ص���1:���ã����д�ص�������0:������   void vApplicationMallocFailedHook( void );
#define configUSE_APPLICATION_TASK_TAG			0
#define configUSE_COUNTING_SEMAPHORES			1
#define configUSE_QUEUE_SETS                    1								//ʹ�ö��м�

#define configGENERATE_RUN_TIME_STATS                   1                               //��ʱ������
#define configUSE_STATS_FORMATTING_FUNCTIONS            1                              //ͳ����������ʱ��  vTaskList()

#if configGENERATE_RUN_TIME_STATS
    //extern uint32_t FreeRTOSRunTimeTicks;
    #define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()    ConfigureTimeForRunTimeStats()                        //��ʼ����ʱ�����ʱ����ʱ��
    #define portGET_RUN_TIME_COUNTER_VALUE()            FreeRTOSRunTimeTicks	//��ȡʱ��ͳ��ʱ��ֵ
#endif

/* Software timer definitions. */
#define configUSE_TIMERS				1											//��̬���񴴽���Ҫ��ʱ������
#define configTIMER_TASK_PRIORITY		( 2 )
#define configTIMER_QUEUE_LENGTH		5
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 2 )
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0                                   //��ʱ���״�����ʱ�ص���1:���ã����д�ص�������0:������ 
/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet		1                                           //�������ȼ�����
#define INCLUDE_uxTaskPriorityGet		1                                           //��ȡ�������ȼ�����
#define INCLUDE_vTaskDelete				1											//����ɾ������ 
#define INCLUDE_uxTaskGetStackHighWaterMark		1									//�����ȡ�����ջ��ʷʣ����Сֵ
#define INCLUDE_vTaskCleanUpResources	1
#define INCLUDE_vTaskSuspend			1											//�������
#define INCLUDE_xTaskResumeFromISR		1											//�����ж��е��ù������
#define INCLUDE_vTaskDelayUntil			1											//�����������ʱ�����
#define INCLUDE_vTaskDelay				1											//�����������
#define INCLUDE_xTaskAbortDelay         1                                           //��������ǿ���뿪����̬

#define INCLUDE_xTaskGetHandle          1       //��ȡ������
/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4        /* 63 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15					//freertos���������ж����ȼ�						���жϷ�������ռ���ȼ�������ߺ����֮��

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5					//freertos���������ж����ȼ�

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */



#define FREE_RTOS_ENABLE_ERR_LOG                1

#if FREE_RTOS_ENABLE_ERR_LOG
    #define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); printf( "Assert failed! File: %s, Line: %d\n", __FILE__, __LINE__ ); for( ;; ); }
#else
    #define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS();for( ;; ); }
#endif
//


/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names.  WORKAROUND_PMU_CM001 is defined at the top of this file. */
#define vPortSVCHandler SVC_Handler
#define xPortSysTickHandler SysTick_Handler
#define xPortPendSVHandler PendSV_Handler



#endif /* FREERTOS_CONFIG_H */

