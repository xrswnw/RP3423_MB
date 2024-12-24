#ifndef _ANYID_BOOT_CAN_HL_H
#define _ANYID_BOOT_CAN_HL_H

#include "AnyID_Boot_Config.h"

#define CAN_HARDPORT				CAN1
#define CAN_INT_CHANNEL				USB_LP_CAN1_RX0_IRQn
#define CAN_IRQHandler   			USB_LP_CAN1_RX0_IRQHandler
	
#define CAN_SEND_TIMEOUT    			0x000000FFFF * 2.5						//��ʱʱ���Ƿ��������Ϊ��׼ֵ��������������������

#define CAN_SENT_TIMEOUT_TICK			100

#define CAN_HARD_REAST_MAX_TICK			10
#define CAN_RGE_ESR_BOFF_ON				0x00000004			//�豸can���ߣ����豸�Զ��뿪����

#define CAN_STATE_IDLE					0x00
#define CAN_STATE_RX_OVER				0x01
#define CAN_STATE_ERR					0x02
#define CAN_STATE_RX_CMD				0x08
#define CAN_STATE_TX					0x10

#define CAN_COM_FRAME_LEN				0x08
#define CAN_COM_FRAME_MAX_LEN			2048

#define CAN_FRAME_CODE_SUCC			0x00000000
#define CAN_FRAME_CODE_FAIL			0xFFFFFFFF

#define CAN_BUD_1000K_PSCVALUE		4
#define CAN_BUD_800K_PSCVALUE		5
#define CAN_BUD_500K_PSCVALUE		8
#define CAN_BUD_250K_PSCVALUE		16
#define CAN_BUD_125K_PSCVALUE		32
#define CAN_BUD_50K_PSCVALUE		80
#define CAN_BUD_20K_PSCVALUE		200
#define CAN_BUD_10K_PSCVALUE		400

#define CAN_OD_VALUE_SUCC			0x00000000			//��Ч
#define CAN_OD_VALUE_RANGE_EXCEEDED	0x06090030			//������Ч�������أ�
#define CAN_RSP_INFO_LEN			4

#define Can_ClrInfo()				do{a_ClearStateBit(g_sCanFrame.state, CAN_STATE_RX_OVER);memcpy(&g_sCanFrame.rxMasg, &g_sRcvTempMsg, sizeof(CanRxMsg));memset(&g_sRcvTempMsg, 0, sizeof(CanRxMsg));}while(0)

typedef struct canFrame{
	u8 state;
	u8 repat;
	u8 step;													//�ֶδ���
	u8 index;													//�����ж�
	u16 lenth;
	u32 waitTimes;												
	CanTxMsg txMasg;
	CanRxMsg rxMasg;
	u8 rcvBuffer[CAN_COM_FRAME_MAX_LEN];						//��������
}CAN_FRAME;

extern CAN_FRAME g_sCanFrame;
extern CanRxMsg g_sRcvTempMsg;


BOOL Can_InitInterface(u16 psc);
u8 CanSend(CanTxMsg *TxMessage);
void Can_Receive_Msg();
void Can_FormatFrame(u8 len, u8 *code);
#endif 

