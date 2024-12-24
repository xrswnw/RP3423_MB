#ifndef _ANYID_CANFESTIVAL_WIFI_HL_
#define _ANYID_CANFESTIVAL_WIFI_HL_

#include "AnyID_CanFestival_Config.h"

#define WIFI_PORT                       USART2
#define Wifi_IRQHandler                 USART2_IRQHandler

#define WIFI_BAUDRARE                   115200

#define Wifi_ReadByte()                 ((u16)(WIFI_PORT->DR & (u16)0x01FF))
#define Wifi_ChkTxOver()                 while(((WIFI_PORT)->SR & USART_FLAG_TC) == (u16)RESET)


                                                
#define WIFI_SR_IDLE                        0x0010  
#define WIFI_DMA                            DMA1

#define WIFI_TXDMA_CH                       DMA1_Channel7
#define WIFI_TXDMA_INT                      DMA1_Channel7_IRQn
#define WIFI_TXDMA_TC_FLAG                  DMA1_FLAG_TC7
#define Wifi_TxDMAIRQHandler                DMA1_Channel7_IRQHandler


#define Wifi_EnableTxDma(p, s)               do{\
                                                    (WIFI_DMA)->IFCR = WIFI_TXDMA_TC_FLAG; \
                                                    (WIFI_TXDMA_CH)->CMAR = ((u32)(p)); \
                                                    (WIFI_TXDMA_CH)->CNDTR = (s); \
                                                    (WIFI_TXDMA_CH)->CCR |= CCR_ENABLE_Set; \
                                                }while(0)
                                                                                             
#define Wifi_DisableTxDma()                  do{\
                                                    (WIFI_DMA)->IFCR = WIFI_TXDMA_TC_FLAG;\
                                                    (WIFI_TXDMA_CH)->CCR &= CCR_ENABLE_Reset;\
                                                    Wifi_ChkTxOver();\
                                                }while(0)

#define WIFI_RXDMA_CH                       DMA1_Channel6
#define WIFI_RXDMA_INT                      DMA1_Channel6_IRQn
#define WIFI_RXDMA_TC_FLAG                  DMA1_FLAG_TC6
#define Wifi_RxDMAIRQHandler                DMA1_Channel6_IRQHandler
#define Wifi_EnableRxDma()                  do{\
                                                (WIFI_DMA)->IFCR = WIFI_RXDMA_TC_FLAG; \
                                                (WIFI_RXDMA_CH)->CNDTR = UART_BUFFER_MAX_LEN; \
                                                (WIFI_RXDMA_CH)->CCR |= CCR_ENABLE_Set; \
                                            }while(0)
                                            


#define Wifi_DisableRxDma()                 do{\
                                                (WIFI_DMA)->IFCR = WIFI_RXDMA_TC_FLAG;\
                                                (WIFI_RXDMA_CH)->CCR &= CCR_ENABLE_Reset;\
                                            }while(0)

#define Wifi_GetRxLen()                     (UART_BUFFER_MAX_LEN - (WIFI_RXDMA_CH)->CNDTR)  

void Wifi_InitInterface(u32 baudrate);
void Wifi_ConfigInt(void);
void Wifi_EnableInt(FunctionalState rxState, FunctionalState txState);
void Wifi_WriteByte(u8 ch);
void Wifi_WriteBuffer(u8 *pFrame, u16 len);
void Wifi_InitTxDma(u8 *pTxBuffer, u32 len);
void Wifi_InitRxDma(u8 *pRxBuffer, u32 len);
void Wifi_WriteStr(char *str);
#endif



