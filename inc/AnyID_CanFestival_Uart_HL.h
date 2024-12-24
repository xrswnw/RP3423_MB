#ifndef _ANYID_CANFESTIVAL_UART_HL_
#define _ANYID_CANFESTIVAL_UART_HL_

#include "AnyID_CanFestival_Config.h"

#define UART_PORT                       USART1
#define Uart_IRQHandler                 USART1_IRQHandler

#define UART_BAUDRARE                   115200
#define Uart_ReadByte()                 ((u16)(UART_PORT->DR & (u16)0x01FF))

#define Uart_ChkTxOver()                while(((UART_PORT)->SR & USART_FLAG_TC) == (u16)RESET)

#define UART_TX_FRAME_LEN                   UART_BUFFER_MAX_LEN
#define UART_RX_FRAME_LEN                   UART_BUFFER_MAX_LEN

#define UART_SR_IDLE                        0x0010  
#define UART_DMA                            DMA1

#define UART_TXDMA_CH                       DMA1_Channel4
#define UART_TXDMA_INT                      DMA1_Channel4_IRQn
#define Uart_TxDMAIRQHandler                DMA1_Channel4_IRQHandler
#define UART_TXDMA_TC_FLAG                  DMA1_FLAG_TC4
#define Uart_EnableTxDma(p, s)               do{\
                                                    (UART_DMA)->IFCR = UART_TXDMA_TC_FLAG; \
                                                    (UART_TXDMA_CH)->CMAR = ((u32)(p)); \
                                                    (UART_TXDMA_CH)->CNDTR = (s); \
                                                    (UART_TXDMA_CH)->CCR |= CCR_ENABLE_Set; \
                                                }while(0)
                                                                                             
#define Uart_DisableTxDma()                  do{\
                                                    (UART_DMA)->IFCR = UART_TXDMA_TC_FLAG;\
                                                    (UART_TXDMA_CH)->CCR &= CCR_ENABLE_Reset;\
                                                    Uart_ChkTxOver();\
                                                }while(0)


#define UART_RXDMA_CH                       DMA1_Channel5
#define UART_RXDMA_INT                      DMA1_Channel5_IRQn
#define Uart_RxDMAIRQHandler                DMA1_Channel5_IRQHandler
#define UART_RXDMA_TC_FLAG                  DMA1_FLAG_TC5
#define Uart_EnableRxDma()                   do{\
                                                    (UART_DMA)->IFCR = UART_RXDMA_TC_FLAG; \
                                                    (UART_RXDMA_CH)->CNDTR = UART_RX_FRAME_LEN; \
                                                    (UART_RXDMA_CH)->CCR |= CCR_ENABLE_Set; \
                                                }while(0)
                                            
#define Uart_GetRxLen()                      (UART_RX_FRAME_LEN - (UART_RXDMA_CH)->CNDTR)  

#define Uart_DisableRxDma()                  do{\
                                                    (UART_DMA)->IFCR = UART_RXDMA_TC_FLAG;\
                                                    (UART_RXDMA_CH)->CCR &= CCR_ENABLE_Reset;\
                                                }while(0)

void Uart_InitInterface(u32 baudrate);
void Uart_ConfigInt(void);
void Uart_EnableInt(FunctionalState rxState, FunctionalState txState);
void Uart_WriteByte(u8 ch);
void Uart_WriteBuffer(u8 *pFrame, u16 len);

void Uart_InitRxDma(u8 *pRxBuffer, u32 len);
void Uart_InitTxDma(u8 *pTxBuffer, u32 len);
#endif



