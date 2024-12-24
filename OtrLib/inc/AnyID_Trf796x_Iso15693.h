#ifndef _ANYID_TRF796X_ISO15693_
#define _ANYID_TRF796X_ISO15693_

#include "AnyID_Trf796x.h"

#define ISO15693_BLOCK_SIZE_4B                  0
#define ISO15693_BLOCK_SIZE_8B                  1
typedef struct iso15693FlagInfo{
    BOOL bHighBr;
    BOOL bDoubleCarrier;
    u8 blockSize;
}ISO15693_FLAG;
extern ISO15693_FLAG g_sIso15693Flag;
#define ISO15693_MAX_FRAME_LEN                  64
#define Iso15693_CfgTagBlock8B()                (g_sIso15693Flag.blockSize = ISO15693_BLOCK_SIZE_8B)
#define Iso15693_CfgTagBlock4B()                (g_sIso15693Flag.blockSize = ISO15693_BLOCK_SIZE_4B)

#define ISO15693_CMD_INVENTORY                  0x01
#define ISO15693_CMD_STAYQUIET                  0x02
#define ISO15693_CMD_READSINGLEBLOCK            0x20
#define ISO15693_CMD_WRITESINGLEBLOCK           0x21
#define ISO15693_CMD_LOCKBLOCK                  0x22
#define ISO15693_CMD_READMULBLOCKS              0x23
#define ISO15693_CMD_WRITEMULBLOCKS             0x24
#define ISO15693_CMD_SELECT                     0x25
#define ISO15693_CMD_RESETTOREADY               0x26
#define ISO15693_CMD_WRITEAFI                   0x27
#define ISO15693_CMD_LOCKAFI                    0x28
#define ISO15693_CMD_WRITEDSFID                 0x29
#define ISO15693_CMD_LOCKDSFID                  0x2A
#define ISO15693_CMD_GETSYSINFO                 0x2B
#define ISO15693_CMD_GETSECURITYINFO            0x2C
#define ISO15693_CMD_SETEAS                     0xA2
#define ISO15693_CMD_RESETEAS                   0xA3
#define ISO15693_CMD_LOCKEAS                    0xA4

#define ISO15693_ANTICOLLISION_SLOT             16

#define ISO15693_SIZE_UID                       0x08
#define ISO15693_SIZE_BLOCK                     0x04
#define ISO15693_SIZE_CRC16                     0x02
#define ISO15693_SIZE_DSFID                     0x01
#define ISO15693_SIZE_AFI                       0x01
#define ISO15693_SIZE_BLOCKSECURITY             0x01
#define ISO15693_SIZE_REPLYFLAG                 0x01
#define ISO15693_SIZE_INFOFLAG                  0x01
#define ISO15693_SIZE_MEMORYSIZE                0x02
#define ISO15693_SIZE_ICREF                     0x01
#define ISO15693_SIZE_REQUESTFLAG               0x01
#define ISO15693_SIZE_MASK                      8


#define ISO15693_REQ_FRAME_POS_FLAG             0x00
#define ISO15693_REQ_FRAME_POS_CMD              0x01
#define ISO15693_REQ_FRAME_POS_PAR              0x02

#define ISO15693_REQ_FLAG_SIGCARRIER            0x00
#define ISO15693_REQ_FLAG_SUBCARRIER            0x01
#define ISO15693_REQ_FLAG_LRATE                 0x00
#define ISO15693_REQ_FLAG_HRATE                 0x02
#define ISO15693_REQ_FLAG_NOINVT                0x00
#define ISO15693_REQ_FLAG_INVENTORY             0x04
#define ISO15693_REQ_FLAG_NOEXT                 0x00
#define ISO15693_REQ_FLAG_EXTPROTOCOL           0x08
#define ISO15693_REQ_FLAG_NOSELECTM             0x00
#define ISO15693_REQ_FLAG_SELECTM               0x10
#define ISO15693_REQ_FLAG_NOADDRM               0x00
#define ISO15693_REQ_FLAG_ADDRM                 0x20
#define ISO15693_REQ_FLAG_NOOPT                 0x00
#define ISO15693_REQ_FLAG_OPT                   0x40
#define ISO15693_REQ_FLAG_RFU                   0x00


#define ISO15693_REQ_FLAG_AFI                   0x10
#define ISO15693_REQ_FLAG_NOAFI                 0x00
#define ISO15693_REQ_FLAG_SLOTS_1               0x20
#define ISO15693_REQ_FLAG_SLOTS_16              0x00

#define ISO15693_RSP_FRAME_POS_FLAG             0x00
#define ISO15693_RSP_FRAME_POS_PAR              0x01
#define ISO15693_RSP_FRAME_POS_ERR              0x01
#define ISO15693_RSP_FLAG_ERR                   0x01
#define ISO15693_RSP_FLAG_OK                    0x00

#define ISO15693_UID_TYPE_POS                   0x06
#define ISO15693_TAG_TYPE_TI                    0x07        //0x03
#define ISO15693_TAG_TYPE_NXP                   0x04        //0x02
#define ISO15693_TAG_TYPE_ST                    0x02        //0x01
#define ISO15693_TAG_TYPE_DFT                   0x00
#define ISO15693_TAG_TYPE_ERR                   0xFF
#define ISO15693_TAG_TYPE_READ                  0xFF

#define ISO15693_RX_WAIT_MIN                    20      //330us * 2  tick=37.76us
#define ISO15693_RX_WAIT_MAX                    500     //���֧��20ms = 20000 / 37.76
#define ISO15693_OP_BLOCK_MAX_NUM               32
#define ISO15693_OP_UID_MAX_NUM                 25
typedef struct iso15693TagInf{
    u8 flag;
    BOOL bDsfid;
    BOOL bAfi;
    BOOL bViccMemoryInf;
    BOOL bICRef;
    u8 afi;
    u8 dsfid;
    u8 blockSize;
    u8 blockNum;
    u8 icRef;
}ISO15693_TAGINF;

typedef struct iso15693Mask{
    u8 value[ISO15693_SIZE_UID];
    u8 length;
}ISO15693_MASK;

#define ISO15693_MASK_NUM                       32
typedef struct iso15693MaskBuffer{
    ISO15693_MASK mask[ISO15693_MASK_NUM];
    ISO15693_MASK cur;
    u8 index;
    u8 num;
}ISO15693_MSKBUF;


#define ISO15693_OP_REPEAT                      3
#define ISO15693_OP_BLOCK_NUM                   256
typedef struct iso15693Block{
    u8 block[(ISO15693_SIZE_BLOCK + ISO15693_SIZE_BLOCK) << 2];     //���һ��Ҫ������4�������ݿ�
    u8 state;
    u8 rAddr[ISO15693_OP_BLOCK_NUM];
    u8 wAddr[ISO15693_OP_BLOCK_NUM];
    u8 reReadAddr[ISO15693_OP_BLOCK_NUM];
    u8 rIndex;
    u8 rNum;
    u8 wIndex;
    u8 wNum;
    u8 reReadIndex;
    u8 reReadNum;
    u8 rRspLen;     //��������Ӧ֡���ȣ�ÿ�ζ�ȡ����Ŀ��ÿ�鳤�ȣ����ᵼ�·������ݳ��Ȳ���1 + 4+ 2�ֽ�
    u8 opNum;
}ISO15693_BLOCK;
/*
typedef struct iso15693Infonition{
    
    u8 u[ISO15693_SIZE_UID];
    ISO15693_BLOCK b;
    u8 afi;
    u8 dsfid;
    u8 rx[64];
    u8 rlt;
}ISO15693_INFO;
*/

typedef struct iso15693Infonition{
    u8 repeat;
    u8 u[ISO15693_SIZE_UID];
    ISO15693_BLOCK b;
    u8 afi;
    u8 dsfid;
    u8 rx[64];
    u8 c;
    u8 slot;
    //mask
    ISO15693_MSKBUF mskBuf;
    int rlt;
}ISO15693_INFO;

#define ISO15693_INFO_FLAG_DSFID                0x01
#define ISO15693_INFO_FLAG_AFI                  0x02
#define ISO15693_INFO_FLAG_VICC                 0x04
#define ISO15693_INFO_FLAG_ICREF                0x08


void Iso15693_Init(BOOL bHighBr, BOOL bDoubleCarrier, BOOL b1Of256, BOOL bOok, BOOL bOpBlock8, u8 pwrLv);

void Iso15693_AddMask(ISO15693_MSKBUF *pMaskBuf, u8 slot);
u8 Iso15693_Inventory(ISO15693_MASK *pMask, u8 afiCtrl, u8 afiValue, u8 *pUid);
u8 Iso15693_InventoryEof(u8 *pUid);
u8 Iso15693_WriteEof(void);


u8 Iso15693_InventorySlot1(u8 afiCtrl, u8 afiValue, u8 *pUID);
u8 Iso15693_Op(u8 *pUid, u8 cmd, u8 *pTxParams1, u8 *pTxParams2, u8 *pTxParams3, u8 *pRxParams, u8 *pRxParamsLen);
u8 Iso15693_GetTagInf(u8 *pUid, ISO15693_TAGINF *pTagInfo);
u8 Iso15693_Dtu(u8 *pTxFrame, u8 txLen, u32 timeout);
#endif

