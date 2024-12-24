#ifndef ANYID_CANFESTIVAL_DEVICE_H_
#define ANYID_CANFESTIVAL_DEVICE_H_

#include "AnyID_CanFestival_FRam.h"

#include "AnyID_CanFestival_Lan.h"
#include "AnyID_CanFestival_RTC.h"
#include "AnyID_CanFestival_WDG_HL.h"
#include "AnyID_CanFestival_Tim_HL.h"
#include "AnyID_CanFestival_Can_HL.h"
#include "AnyID_CanFestival_RS485.h"
#include "AnyID_CanFestival_Periph_HL.h"
#include "AnyID_CanFestival_Wifi.h"
#include "AnyID_CanFestival_Uart.h"
extern u32 g_nDeviceState;


#define DEVICE_VERSION_SIZE 			50
#define DEVICE_VERSION_STEP_LEN         5

//canopen相关--------------------------------------------------------------

#define CANOPEN_PDO_TRANSIONTYPE_ACYCLIC_SYNCHRONIZATION    					0x00	//非循环同步
#define CANOPEN_PDO_TRANSIONTYPE_LOOP_SYNCHRONIZATION_1     					0x01	//循环同步一次
#define CANOPEN_PDO_TRANSIONTYPE_REMOTE_SYNCHRONIZATION   						0xFC	//远程同步
#define CANOPEN_PDO_TRANSIONTYPE_REMOTE_ASYNCHRONOUS   							0xFD	//远程异步
#define CANOPEN_PDO_TRANSIONTYPE_MAKER_SPECIFIC_ASYNCHRONOUS   					0xFE	//制造商特定异步
#define CANOPEN_PDO_TRANSIONTYPE_DEVICE_SUBPROTOCOL_SPECIFIC_ASYNCHRONOUS 		0xFF	//设备子协议异步

//-----------------------------------------------------------------
typedef struct wokeInfo{
    u8 type;
}WORK_INFO;

typedef struct pemaceInfo{
    u8 pwr;
}PEMACE_INFO;

typedef struct rfWokeInfo{
    u8 mode;
}RF_WOKEINFO;

typedef struct comInfo{
    u32 mode;
}COM_INFO;

typedef struct reportInfo{
    u32 keepTime;
}REPORT_INFO;

typedef struct recordInfo{
    u8 mode;
}RECORD_INFO;

typedef struct infraredInfo{
    u8 mode;
}INFRARED_INFO;

typedef struct ioInfo{
    u8 mode;
}OUTPUT_INFO;



//
typedef struct deviceParamenter{
	u8 addr;
	u8 bud;
	u16 flag;
    OUTOUT_PERIPH outParams;
    u32 rfu;
	u32 crc;
}DEVICE_PARAMS;
extern DEVICE_PARAMS g_sDeviceParams;
                                       
                                                            
#define DEVICE_FRAME_BROADCAST_ADDR             0xFFFF

#define READER_RSPFRAME_LEN                     UART_BUFFER_MAX_LEN

#define READER_COM_TYPE_485                     0x01
typedef struct readerRspFrame{
    u16 len;
    u8 buffer[READER_RSPFRAME_LEN];
    u8 flag;
    u8 err;
    u16 destAddr;
    u8 cmd;
    u8 comType;
}READER_RSPFRAME;
extern READER_RSPFRAME g_sReaderRspFrame;

extern const u8 DEVICE_VERSION[DEVICE_VERSION_SIZE];
//
//CAN_ID
#define DEVICE_COM_BASE_BOAD_COBID          0x7FF
#define DEVICE_COM_BASE_BOAD_RSP_COBID      0x700


#define DEVICE_COB_ID_MASK_ID               0xFFFFFFF0
#define DEVICE_COB_ID_MASK_ADDR             0x0000000F

#define DEVICE_COB_ID_MT_SDO                0x100
#define DEVICE_COB_ID_MR_SDO                0x120

#define DEVICE_COB_ID_ST_SDO                0x140
#define DEVICE_COB_ID_SR_SDO                0x160

//cmd
    
#define DEVICE_COM_CMD_HEART                0x00
#define DEVICE_COM_CMD_BOOT_UP              0x01
#define DEVICE_COM_CMD_RESET                0x02
#define DEVICE_COM_CMD_GET_VERSION          0x07


#define DEVICE_COM_CMD_ANT_CTR              0x30
#define DEVICE_COM_CMD_OUTPUT_CTR           0x32

#define DEVICE_COM_CMD_RF_AMAR_INFO         0x40

//

#define DEVICE_CAN_RSP_LEN                  0x02

#define DEVICE_CAN_RSP_POS_CMD              0x00
#define DEVICE_CAN_RSP_POS_RESULT           0x01
#define DEVICE_CAN_RSP_POS_PARAMS           0x02
#define DEVICE_CAN_RSP_POS_LOMG_PARAMS      0x05



#define DEVICE_CAN_FRAME_RSP_OK             0x00
#define DEVICE_CAN_FRAME_RSP_BUSY           0x01
#define DEVICE_CAN_FRAME_RSP_PARAS_ERR      0x02
#define DEVICE_CAN_FRAME_RSP_NO_SUPP        0x04

#define DEVICE_CAN_FRAME_RESULT_NO_RSP      0x00
#define DEVICE_CAN_FRAME_RESULT_RSP         0x01
#define DEVICE_CAN_FRAME_RESULT_CHKER       0x02
#define DEVICE_CAN_FRAME_RESULT_BOAD        0x03
#define DEVICE_CAN_FRAME_RESULT_CHKOK       0x04
    
#define DEVICE_CAN_FRAME_RFU                0x00   


#define DEVICE_CAN_FRAME_NOPARAMS_LEN       2
#define DEVICE_CAN_FRAME_BOOT_UP_LEN        4
#define DEVICE_CAN_FRAME_GETVERSION_LEN     3
#define DEVICE_CAN_FRAME_LIMITDATA_LEN      8

//设备状态
extern u32 g_nDeviceStatus;

#define DEVICE_STATUS_BOO_UP_OK             0x00000001


//
//其它外设

#define DEVICE_SUBDEVICE_TYPE_MB                0x01
#define DEVICE_SUBDEVICE_TYPE_MX                0x02
#define DEVICE_SUBDEVICE_TYPE_RF                0x04


#define DEVICE_SUBDEVICE_STATUS_HEART           0x00
#define DEVICE_SUBDEVICE_STATUS_MEM_ERR         0x01
#define DEVICE_SUBDEVICE_STATUS_RF_ERR          0x02
#define DEVICE_SUBDEVICE_STATUS_PWR_ERR         0x04
#define DEVICE_SUBDEVICE_STATUS_TMPR_ERR        0x08
#define DEVICE_SUBDEVICE_STATUS_HEART_LOADING   0x40
#define DEVICE_SUBDEVICE_STATUS_STD_ONLINE      0x80
#define DEVICE_SUBDEVICE_STATUS_OFFLINE         0xFF

#define DEVICE_SUBDEVICE_ALONE_RF_INDEX         0

#define DEVICE_SUBDEVICE_ALONE_ANT_OP_NUM       3
#define DEVICE_SUBDEVICE_RF_ANT_NUM             5
#define DEVICE_SUBDEVICE_MX_ANT_NUM             2

    

#define DEVICE_SUBDEVICE_ANT_0                  0      
#define DEVICE_SUBDEVICE_ANT_1                  1
#define DEVICE_SUBDEVICE_ANT_2                  2
#define DEVICE_SUBDEVICE_ANT_3                  3
#define DEVICE_SUBDEVICE_ANT_4                  4
#define DEVICE_SUBDEVICE_ANT_5                  5
#define DEVICE_SUBDEVICE_ANT_6                  6
#define DEVICE_SUBDEVICE_ANT_7                  7
#define DEVICE_SUBDEVICE_ANT_8                  8
#define DEVICE_SUBDEVICE_ANT_9                  9

#define Device_SeltctAnt(ant)           ({u8 index = 0;index = (1 << ant); (index);})

typedef struct deviceMxInfo{
    u8 addr;
    u8 status;
    u32 comErrTick;
    u32 rfu;
    char version[DEVICE_VERSION_SIZE];
}DEVICE_MXINFO;

typedef struct deviceRfInfo{
    u8 addr;
    u8 status;
    u32 comErrTick;
    u32 rfu;
    char version[DEVICE_VERSION_SIZE];
}DEVICE_RFINFO;



#define DEVICE_SUB_RFINFO_LEN               (8 + 16)

typedef struct flashRecord
{
    u8 data[DEVICE_SUB_RFINFO_LEN + 20];
    u32 crc;
}FLASH_RECORD;

typedef struct deviceInfo{
    u8 addr;
    u8 type;
    u8 status;
    u32 comErrTick;
    u32 heartTime;
    u32 rfu;
    char version[DEVICE_VERSION_SIZE];
}DEVICE_INFO;

#define DEVICE_SUBDEVICE_MB_MAX_NUM             2
#define DEVICE_SUBDEVICE_MX_MAX_NUM             8
#define DEVICE_SUBDEVICE_RF_MAX_NUM             4

#define DEVICE_SUBDEVICE_MAX_NUM                (DEVICE_SUBDEVICE_MB_MAX_NUM + DEVICE_SUBDEVICE_MX_MAX_NUM + DEVICE_SUBDEVICE_RF_MAX_NUM)   

//后期可做分布式自由控制，由上报地址类型区分设备控制
typedef struct deviceSubInfo{
    u8 num;
    u8 index;
    DEVICE_INFO subInfo[DEVICE_SUBDEVICE_MAX_NUM];
    u32 rfu;
}DEVICE_SUBINFO;
//

#define DEVICE_RF_OP_STATE_IDLE                 0x00
#define DEVICE_RF_OP_STATE_START_OP             0x01
#define DEVICE_RF_OP_STATE_OPEN_ANT_AND_OP      0x02
#define DEVICE_RF_OP_STATE_GET_AMAR_DATA        0x04
#define DEVICE_RF_OP_STATE_WAIT                 0x08
#define DEVICE_RF_OP_STATE_ANT_OP_OK            0x10
#define DEVICE_RF_OP_STATE_ANT_OP_FAIL          0x20
#define DEVICE_RF_OP_STATE_CHANGE_ANT           0x40
#define DEVICE_RF_OP_STATE_SLEEP                0x80

typedef struct rfOp{
    u8 state;
    u8 index;
    u8 antNum;
    u8 antIndex;
    u8 repaet;
    u8 rfIndex;
    u8 rfAddr[DEVICE_SUBDEVICE_RF_MAX_NUM];
    u8 mxIndex;
    u8 mxAddr[DEVICE_SUBDEVICE_MX_MAX_NUM];
    
    u8 antPort;
    u8 amar;
    u8 amarValue;
    u8 opAddr;
    u8 opLen;
    u8 data[DEVICE_SUB_RFINFO_LEN];
    u32 opTime;
}RF_OP;

typedef struct deviceOp{
    RF_OP rfInfo;
}DEVICE_OP;


void Device_Delayms();
void Device_Delay100us(u32 t);
void Device_Init();

void Device_ReadDeviceParamenter();
BOOL Device_WriteDeviceParamenter(void);	
void Device_ResetDeviceParamenter();
u8 Device_ProcessCanFrame(CAN_FRAME *pRxFrame);
u16 Reader_ProcessUartFrame(u8 *pFrame, UART_TXFRAME *txFrame);
u16 Device_ResponseFrame(u8 *pParam, u16 len, UART_TXFRAME *pRspFrame);

void Device_AddCanTxFrame(u32 coBid, u8 cmd, u8 result, u8 len, u8 *pBuffer);
u8 Device_SubDeviceGetIndex(u8 addr);
void Device_SubDeviceBootUp(u8 addr, u8 type, u8 status);
DEVICE_INFO *Device_SubDeviceGet(u8 addr);
void Device_BootDown(u32 addr);
void Device_RspFormatCanFrame(u32 coBid, u8 cmd, u8 result, u8 len, u8 *pBuffer);
u8 Device_ProcessCobMTSdo(u32 addr, u8 cmd, u8 paramsLen, u8 *pParams);
//u8 Device_ProcessCobMRSdo(u32 addr, u8 cmd, u8 paramsLen, u8 *pParams);
u8 Device_ProcessCobSTSdo(u32 addr, u8 cmd, u8 paramsLen, u8 *pParams);
u8 Device_ProcessCobSRSdo(u32 addr, u8 cmd, u8 paramsLen, u8 *pParams);

void Device_GetSubVersion(u32 addr);
void Device_CanAutoTask();

void Device_OpTask();




//------------------------

u8 Device_ProcessCobMRSdo(u32 addr, u8 cmd, u8 paramsLen, u8 *pParams, CAN_FRAME *txFrame);
void Device_CanRxDispatch(void *p);
void Device_CanTxDispatch(void *p);
void Device_MainDispatch(void *p);
void Device_HeartDispatch(void *p);

void Device_UartRxDispatch(void *p);
void Device_UartTxDispatch(void *p);
#endif