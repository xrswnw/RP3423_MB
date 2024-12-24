/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "AnyID_CanFestival_SysCfg.h"


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
/*
void SVC_Handler(void)
{
}
*/
/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
/*
void PendSV_Handler(void)
{
}
*/
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
/*
void SysTick_Handler(void)
{
    g_nSysTick++;
    if(g_nSysTick % 5 == 0)
	{
		a_SetStateBit(g_nSysState, SYS_STAT_RUNLED);
	}
	
	if(g_nSysTick % 21 == 0)
	{
		a_SetStateBit(g_nSysState,  SYS_STAT_ADDRECORD | SYS_STAT_AD | SYS_STAT_IN_SENSOR | SYS_STAT_CTLOUTPUT);
	}
}
*/

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/
//IRQ
void TIM2_IRQHandler()
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET) //溢出中断
	{
		FreeRTOSRunTimeTicks++;
        g_nSysTick = (FreeRTOSRunTimeTicks / 1000);                 //  1mstick
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除中断标志位
	}
}


void Log_RxDMAIRQHandler(void)
{
    Log_DisableRxDma(); //接收缓冲区满，一般不会出现这样的情况，如果有，就可能系统有故障

}

void Log_TxDMAIRQHandler(void)
{
    Log_DisableRxDma();                                             //DMA完成后，最后一个字节可能没有发送出去，需要在主循环中做判断处理
    Log_EnableRxDma();                                              //使能接收
}

u16 g_nLogSr = 0;
u16 g_nLogDr = 0;
void Log_IRQHandler(void)
{
    if(LOG_PORT->SR & LOG_SR_IDLE)
    {    
        Log_DisableRxDma();
    }

    g_nLogSr = LOG_PORT->SR;  //通过读取SR和DR清空中断标志
    g_nLogDr = LOG_PORT->DR;    
}

CanRxMsg g_sCanRcvInfo = {0};
void CAN_IRQHandler(void)
{
	// 没有接收到数据,直接退出 
	if(CAN_MessagePending(CAN_HARDPORT, CAN_FIFO0))
	{
        CanRxMsg *pCanRcvInfo = NULL;
        
        pCanRcvInfo = &g_sCanRcvInfo;
		CAN_Receive(CAN_HARDPORT, CAN_FIFO0, pCanRcvInfo);   // 读取数据	
        if(pCanRcvInfo->IDE == CAN_ID_STD && pCanRcvInfo->DLC)
        {
            g_sTempCanFrame.coBid = pCanRcvInfo->StdId;
            g_sTempCanFrame.staus = CAN_FRAME_STATUS_STD;
            g_sTempCanFrame.len = pCanRcvInfo->DLC;
            memcpy(g_sTempCanFrame.buffer, pCanRcvInfo->Data, pCanRcvInfo->DLC);
          
            if(xQueueSendFromISR(g_hCanRxQueue, &g_sTempCanFrame, 0) == pdPASS)
            {
                pCanRcvInfo->DLC = 0;
            }
            else
            {
                //队列已满,数据溢出？硬件问题？考虑清除队列缓冲区
                pCanRcvInfo->DLC = 0;
            }
        }
	}
}
//

void Uart_RxDMAIRQHandler(void)
{
    Uart_DisableRxDma(); //接收缓冲区满，一般不会出现这样的情况，如果有，就可能系统有故障
    g_sUartRxFrame.length = 0;
}

void Uart_TxDMAIRQHandler(void)
{
    Uart_DisableTxDma();                                             //DMA完成后，最后一个字节可能没有发送出去，需要在主循环中做判断处理
    Uart_EnableRxDma();                                              //使能接收
}

u16 g_nUartSr = 0;
u16 g_nUartDr = 0;
void Uart_IRQHandler(void)
{
    if(UART_PORT->SR & UART_SR_IDLE)
    {    
        Uart_DisableRxDma();
        u8 byte =Uart_ReadByte();
        g_sUartRxFrame.length = Uart_GetRxLen();
        g_sUartRxFrame.com = UART_COM_232;
        if(g_sUartRxFrame.length)
        {
            if(xQueueSendFromISR(g_hUartRxQueue, &g_sUartRxFrame, 0) == pdPASS)
            {
                g_sUartRxFrame.length = 0;
                Uart_EnableRxDma();
            }
            else
            {
                //队列已满,数据溢出？硬件问题？考虑清除队列缓冲区
                g_sUartRxFrame.length = 0;
            }
        }
    }

    g_nUartSr = UART_PORT->SR;                                       //通过读取SR和DR清空中断标志
    g_nUartSr = UART_PORT->DR;    
}