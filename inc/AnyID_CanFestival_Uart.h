#ifndef _ANYID_CANFESTIVAL_UART_
#define _ANYID_CANFESTIVAL_UART_

#include "AnyID_CanFestival_Uart_HL.h"
#include "AnyID_CanFestival_Uart_Receive.h"

#define UART_TX_FRAME_MAX_LEN             5
#define UART_RX_FRAME_MAX_LEN             5


extern QueueHandle_t g_hUartRxQueue;
extern QueueHandle_t g_hUartTxQueue;

BOOL Uart_Init(u32 bud);
#endif
