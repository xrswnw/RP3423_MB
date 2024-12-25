#ifndef _ANYID_CANFESTIVAL_LAN_
#define _ANYID_CANFESTIVAL_LAN_


#include "AnyID_CanFestival_Uart_Receive.h"
#include "AnyID_CanFestival_Lan_HL.h"


BOOL Lan_Init(u32 bud);

extern UART_RCVFRAME g_sLanRxFrame;
extern UART_TXFRAME g_sLanTxFrame;
#endif
