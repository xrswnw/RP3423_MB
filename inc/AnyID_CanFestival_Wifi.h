#ifndef _ANYID_CANFESTIVAL_WIFI_
#define _ANYID_CANFESTIVAL_WIFI_

#include "AnyID_CanFestival_Uart_Receive.h"
#include "AnyID_CanFestival_Wifi_HL.h"

#define WIFI_FRAME_MAX_LENTH        1024


#define WIFI_STAT_TX_IDLE               0
#define WIFI_STAT_TX_BUSY               1


typedef struct wifiTxInfo{
    u8 state;
    u16 txLenth;
    u8 txFrame[WIFI_FRAME_MAX_LENTH];
}WIFI_TXINFO;

extern WIFI_TXINFO g_sWifiTxInfo;
extern UART_RCVFRAME g_sWifiRcvFrame;
#endif
