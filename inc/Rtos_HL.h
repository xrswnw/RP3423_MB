#ifndef _RTOS_HL_
#define _RTOS_HL_

#include "stdio.h"

#if defined USE_FREE_RTOS
    #if (defined(_ANYID_STM32_) || defined(_ANYID_GD32_))
        #include "stm32f10x.h"
        typedef struct Port_Inf
        {
            GPIO_TypeDef* Port;
            u16 Pin;
        } PORT_INF;
    #endif
#endif

//HL
    void Log_Delayms(u32 n);
    extern const PORT_INF LOG_485_CTRL;
    #define Log_EnableRx()                      (LOG_485_CTRL.Port->BRR  = LOG_485_CTRL.Pin)
    #define Log_EnableTx()                      (LOG_485_CTRL.Port->BSRR = LOG_485_CTRL.Pin)

    #define LOG_PORT                            UART4
    #define Log_IRQHandler                     UART4_IRQHandler
    #define LOG_BAUDRARE                       9600

    #define LOG_TX_FRAME_LEN                   (64 + 32)
    #define LOG_RX_FRAME_LEN                   (256 + 32)

    #define Log_ReadByte()                     ((u16)(LOG_PORT->DR & (u16)0x01FF))
    #define Log_ChkTxOver()                    while(((LOG_PORT)->SR & USART_FLAG_TC) == (u16)RESET)

    #define LOG_SR_IDLE                        0x0010  
    #define LOG_DMA                            DMA2

    #define LOG_TXDMA_CH                       DMA2_Channel5
    #define LOG_TXDMA_INT                      DMA2_Channel4_5_IRQn
    #define Log_TxDMAIRQHandler                DMA2_Channel4_5_IRQHandler
    #define LOG_TXDMA_TC_FLAG                  DMA2_FLAG_TC5

    #define Log_EnableTxDma(p, s)              do{\
                                                    Log_EnableTx();\
                                                    (LOG_DMA)->IFCR = LOG_TXDMA_TC_FLAG; \
                                                    (LOG_TXDMA_CH)->CMAR = ((u32)(p)); \
                                                    (LOG_TXDMA_CH)->CNDTR = (s); \
                                                    (LOG_TXDMA_CH)->CCR |= CCR_ENABLE_Set; \
                                                 }while(0)

    #define Log_DisableTxDma()                 do{\
                                                    (LOG_DMA)->IFCR = LOG_TXDMA_TC_FLAG;\
                                                    (LOG_TXDMA_CH)->CCR &= CCR_ENABLE_Reset;\
                                                    Log_ChkTxOver();\
                                                    Log_EnableRx();\
                                                }while(0)

    #define LOG_RXDMA_CH                       DMA2_Channel3
    #define LOG_RXDMA_INT                      DMA2_Channel3_IRQn
    #define Log_RxDMAIRQHandler                DMA2_Channel3_IRQHandler
    #define LOG_RXDMA_TC_FLAG                  DMA2_FLAG_TC3
    #define Log_EnableRxDma()                  do{\
                                                    Log_EnableRx();\
                                                    (LOG_DMA)->IFCR = LOG_RXDMA_TC_FLAG; \
                                                    (LOG_RXDMA_CH)->CNDTR = LOG_RX_FRAME_LEN; \
                                                    (LOG_RXDMA_CH)->CCR |= CCR_ENABLE_Set; \
                                                }while(0)

    #define Log_GetRxLen()                     (LOG_RX_FRAME_LEN - (LOG_RXDMA_CH)->CNDTR)  

    #define Log_DisableRxDma()                 do{\
                                                    (LOG_DMA)->IFCR = LOG_RXDMA_TC_FLAG;\
                                                    (LOG_RXDMA_CH)->CCR &= CCR_ENABLE_Reset;\
                                                }while(0)

    void Log_InitInterface();

    void Log_InitRxDma(u8 *pRxBuffer, u32 len);
    void Log_InitTxDma(u8 *pTxBuffer, u32 len);
    void Log_WriteStr(char *str);
    void Log_WriteBuffer(u8 *pFrame, u16 len);
    void Log_WriteByte(u8 ch);
    void Log_Delayms(u32 n);
    void Log_Init();

    
extern u32 FreeRTOSRunTimeTicks;
extern u32 g_nSysTick;



#define SYSCLK                          120000000




#define TIM_TOTAL_5MS                       (1 * 5 + 1)                 //偏移一个Tick，减少偶然重发

void Tim_Init(u32 period, u32 prescaler);
void ConfigureTimeForRunTimeStats();
#endif