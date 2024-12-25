#ifndef _ANYID_CANFESTIVAL_LAN_HL_
#define _ANYID_CANFESTIVAL_LAN_HL_

#include "AnyID_CanFestival_Config.h"

#define LAN_PORT                       USART3
#define Lan_IRQHandler                 USART3_IRQHandler

#define LAN_BAUDRARE                   115200

#define Lan_ReadByte()                 ((u16)(LAN_PORT->DR & (u16)0x01FF))


#define Lan_ChkTxOver()                while(((LAN_PORT)->SR & USART_FLAG_TC) == (u16)RESET)

#define LAN_TX_FRAME_LEN                   UART_BUFFER_MAX_LEN
#define LAN_RX_FRAME_LEN                   UART_BUFFER_MAX_LEN

#define LAN_SR_IDLE                        0x0010  
#define LAN_DMA                            DMA1

#define LAN_TXDMA_CH                       DMA1_Channel2
#define LAN_TXDMA_INT                      DMA1_Channel2_IRQn
#define Lan_TxDMAIRQHandler                DMA1_Channel2_IRQHandler
#define LAN_TXDMA_TC_FLAG                  DMA1_FLAG_TC2
#define Lan_EnableTxDma(p, s)               do{\
                                                    (LAN_DMA)->IFCR = LAN_TXDMA_TC_FLAG; \
                                                    (LAN_TXDMA_CH)->CMAR = ((u32)(p)); \
                                                    (LAN_TXDMA_CH)->CNDTR = (s); \
                                                    (LAN_TXDMA_CH)->CCR |= CCR_ENABLE_Set; \
                                                }while(0)
                                                                                             
#define Lan_DisableTxDma()                  do{\
                                                    (LAN_DMA)->IFCR = LAN_TXDMA_TC_FLAG;\
                                                    (LAN_TXDMA_CH)->CCR &= CCR_ENABLE_Reset;\
                                                    Lan_ChkTxOver();\
                                                }while(0)


#define LAN_RXDMA_CH                       DMA1_Channel3
#define LAN_RXDMA_INT                      DMA1_Channel3_IRQn
#define Lan_RxDMAIRQHandler                DMA1_Channel3_IRQHandler
#define LAN_RXDMA_TC_FLAG                  DMA1_FLAG_TC3
#define Lan_EnableRxDma()                   do{\
                                                    (LAN_DMA)->IFCR = LAN_RXDMA_TC_FLAG; \
                                                    (LAN_RXDMA_CH)->CNDTR = LAN_RX_FRAME_LEN; \
                                                    (LAN_RXDMA_CH)->CCR |= CCR_ENABLE_Set; \
                                                }while(0)
                                            
#define Lan_GetRxLen()                      (LAN_RX_FRAME_LEN - (LAN_RXDMA_CH)->CNDTR)  

#define Lan_DisableRxDma()                  do{\
                                                    (LAN_DMA)->IFCR = LAN_RXDMA_TC_FLAG;\
                                                    (LAN_RXDMA_CH)->CCR &= CCR_ENABLE_Reset;\
                                                }while(0)






void Lan_InitInterface(u32 baudrate);
void Lan_ConfigInt(void);
void Lan_EnableInt(FunctionalState rxState, FunctionalState txState);
void Lan_WriteByte(u8 ch);
void Lan_WriteBuffer(u8 *pFrame, u16 len);

void Lan_InitTxDma(u8 *pTxBuffer, u32 len);
void Lan_InitRxDma(u8 *pRxBuffer, u32 len);
#endif



