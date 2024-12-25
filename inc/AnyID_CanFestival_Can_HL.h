#ifndef _ANYID_CANFESTIVAL_CAN_HL_H
#define _ANYID_CANFESTIVAL_CAN_HL_H


#include "AnyID_CanFestival_Config.h"

#define CAN_HARDPORT				CAN1
#define CAN_INT_CHANNEL				USB_LP_CAN1_RX0_IRQn
#define CAN_IRQHandler   			USB_LP_CAN1_RX0_IRQHandler
	
#define CAN_SEND_TIMEOUT            0x1FFF						            //超时时间是否以最低速为基准值？？？？？？？？？？

#define CAN_FRAME_MAX_LEN           8
	
#define CAN_HARD_REAST_MAX_TICK     10
#define CAN_RGE_ESR_BOFF_ON         0x00000004			                                //设备can离线？？设备自动离开网络

#define CAN_PSC_BUD_1M                  12
#define CAN_PSC_BUD_800K                15
#define CAN_PSC_BUD_500K                24
#define CAN_PSC_BUD_250K                48
#define CAN_PSC_BUD_125K                96
#define CAN_PSC_BUD_50K                 240
#define CAN_PSC_BUD_20K                 600
#define CAN_PSC_BUD_10K                 1200

#define CAN_STATUS_IDLE             0x00000000
#define CAN_STATUS_TX               0x00000001
#define CAN_STATUS_WAIT             0x00000002

#define CAN_STATUS_STEP             0x00000008
#define CAN_STATUS_CHECK            0x00000010
#define CAN_STATUS_ERR              0x00000020
#define CAN_STATUS_RSP              0x00000040
#define CAN_STATUS_OP_DELAY         0x00000080


#define CAN_TX_TO_TIME				20
#define CAN_TX_REPET_TICK			7


#define CAN_FRAME_MAX_LENTH         8

#define CAN_FRAME_STATUS_NORMAL     0x00
#define CAN_FRAME_STATUS_REPORT     0x01

#define CAN_FRAME_FLAG_NO_RSP       0x00
#define CAN_FRAME_FLAG_NEED_RSP     0x01
typedef struct canFrame{
    u8 staus;
    u8 flag;
    u8 index;
    u8 len;
    u8 cmd;
    u32 coBid;
    u8 buffer[CAN_FRAME_MAX_LENTH];
}CAN_FRAME;


#define CAN_TX_MAX_SIZE				60
typedef struct canTxFrame{
	u8 index;
	u8 num;
	u8 rfu;
	u8 repet;
	u32 time;
    
    CAN_FRAME frame[CAN_TX_MAX_SIZE];
}CAN_TXFRAME;

#define CAN_RCV_STATUS_RCV_IDLE                 0x00
#define CAN_RCV_STATUS_RCV_DATA_LONG            0x04
#define CAN_RCV_STATUS_RCV_OK                   0x08

#define CAN_RCV_LONG_FRAME_HEARD1               0x21
#define CAN_RCV_LONG_FRAME_HEARD2               0x12


#define CAN_RCV_LONG_FRAME_POS_HEARD1           0x00
#define CAN_RCV_LONG_FRAME_POS_HEARD2           0x01
#define CAN_RCV_LONG_FRAME_POS_LENTH            0x02
#define CAN_RCV_LONG_FRAME_POS_ADDR             0x03

#define CAN_RCV_LONG_FRAME_POS_CMD              0x03
#define CAN_RCV_FRAME_POS_CMD                   0x00
    
#define CAN_RX_MAX_SIZE             60
typedef struct canRxFrame{
    u8 status;
	u8 index;
	u8 num;
	u8 rfu;
    u32 coBid;
    u32 idleTime;
	CAN_FRAME frame[CAN_RX_MAX_SIZE];
}CAN_RXFRAME;





BOOL Can_InitInterface(u8 psc);
BOOL Can_TxBuffer(CanTxMsg *pTxInfo);
BOOL CanTxTransm(CanTxMsg *pFrame);

void Can_ReceiveFrame(CanRxMsg *pRcvFrame);
BOOL Can_TxFrames(CAN_FRAME *pTxFrame);
BOOL Can_TxFramesRepat(CAN_FRAME *pFrame);
//
extern CAN_FRAME g_sCanRspFrame;
extern CAN_FRAME g_sRspCanFrame;
extern QueueHandle_t g_hCanRxQueue;
extern QueueHandle_t g_hCanTxQueue;
extern QueueHandle_t g_hCanComStatus;

#define Can_ClearRspInfo()          do{memset(&g_sRspCanFrame, 0, sizeof(CAN_FRAME));}while(0)

BOOL Can_QueueCreatTask();

#endif

