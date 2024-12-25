#ifndef _ANYID_CANFESTIVAL_WIFI_HL_
#define _ANYID_CANFESTIVAL_WIFI_HL_

#include "AnyID_CanFestival_Config.h"

#define WIFI_PORT                       USART2
#define Wifi_IRQHandler                 USART2_IRQHandler

#define STR_HEX_CR			            0x0D			//\r
#define STR_HEX_LF			            0x0A			//\n

#define WIFI_FEAME_RX_LEN               4096
#define WIFI_FEAME_TX_LEN               512
#define WIFI_RRAME_RX_MIN_LEN           4               //OK\R\N
   
#define WIFI_BAUDRARE                   115200
#define Wifi_ReadByte()                 ((u16)(WIFI_PORT->DR & (u16)0x01FF))
                                                 
extern const PORT_INF WIFI_PORT_RST;
#define WIFI_RstLow()				    WIFI_PORT_RST.Port->BRR = WIFI_PORT_RST.Pin
#define WIFI_RstHigh()				    WIFI_PORT_RST.Port->BSRR = WIFI_PORT_RST.Pin

#define WIFI_RCV_TIMER                  TIM4
#define WIFI_RCV_TIM_WIFI               20000 
#define WIFI_RCV_TIM_9600               4010  //3.5 * 11 * (1/9600) * 1000
#define WIFI_RCV_TIM_9600M              1750  //超过19200bps都采用1750us
#define WIFI_RCV_TIM_PRESCALER          119   //120Mhz
#define WIFI_RCV_TIMER_INT              TIM4_IRQn
#define Wifi_ClearRcvTimFlag()          (WIFI_RCV_TIMER)->SR = (uint16_t)(~TIM_FLAG_Update)
#define Wifi_StartRcvTim()              (WIFI_RCV_TIMER)->CR1 |= TIM_CR1_CEN
#define Wifi_StopRcvTim()               (WIFI_RCV_TIMER)->CR1 &= (~TIM_CR1_CEN)
#define Wifi_UpdateTimerPeriod(t)       ((WIFI_RCV_TIMER)->ARR = (t), (WIFI_RCV_TIMER)->EGR = TIM_PSCReloadMode_Immediate)
#define Wifi_ResetTimCnt()              WIFI_RCV_TIMER->CNT = 1


void Wifi_InitInterface(u32 baudrate);
void Wifi_InitTimer(u32 baudrate);
void Wifi_Delayms(u32 n) ;
void Wifi_EnableInt(FunctionalState rxState, FunctionalState txState);
void Wifi_WriteByte(u8 ch);
void Wifi_WriteBuffer(u8 *pBuffer, u32 len);
void Wifi_ConfigInt();
void Wifi_WriteAtCmd(char *str);
#endif



