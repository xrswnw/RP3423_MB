#ifndef _ANYID_CANFESTIVAL_RS485_HL_
#define _ANYID_CANFESTIVAL_RS485_HL_

#include "AnyID_CanFestival_Config.h"

#define RS485_PORT                       UART4
#define Rs485_IRQHandler                 UART4_IRQHandler

#define RS485_BAUDRARE                   9600

#define Rs485_ReadByte()                 ((u16)(RS485_PORT->DR & (u16)0x01FF))

extern const PORT_INF UART_485_CTRL;
#define Rs485_EnableRx()            (UART_485_CTRL.Port->BRR = UART_485_CTRL.Pin)
#define Rs485_EnableTx()            (UART_485_CTRL.Port->BSRR = UART_485_CTRL.Pin)
#define Rs485_ChkTxOver()            while(((RS485_PORT)->SR & USART_FLAG_TC) == (u16)RESET)


void Rs485_Delayms(u32 n);
void Rs485_InitInterface(u32 baudrate);
void Rs485_ConfigInt(void);
void Rs485_EnableInt(FunctionalState rxState, FunctionalState txState);
void Rs485_WriteByte(u8 ch);
void Rs485_WriteBuffer(u8 *pFrame, u16 len);
void Rs485_WriteErrStr(char *str);
void Rs485_WriteWarStr(char *str);
void Rs485_InitFloating();
#endif



