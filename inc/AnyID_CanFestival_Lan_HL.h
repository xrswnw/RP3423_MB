#ifndef _ANYID_CANFESTIVAL_LAN_HL_
#define _ANYID_CANFESTIVAL_LAN_HL_

#include "AnyID_CanFestival_Config.h"

#define LAN_PORT                       USART3
#define Lan_IRQHandler                 USART3_IRQHandler

#define LAN_BAUDRARE                   115200

#define Lan_ReadByte()                 ((u16)(LAN_PORT->DR & (u16)0x01FF))

#define Lan_ChkTxOver()     while(((LAN_PORT)->SR & USART_FLAG_TC) == (u16)RESET)

void Lan_InitInterface(u32 baudrate);
void Lan_ConfigInt(void);
void Lan_EnableInt(FunctionalState rxState, FunctionalState txState);
void Lan_WriteByte(u8 ch);
void Lan_WriteBuffer(u8 *pFrame, u16 len);

#endif



