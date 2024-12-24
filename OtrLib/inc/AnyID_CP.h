#ifndef _ANYID_CP_
#define _ANYID_CP_

#include "AnyID_Type.h"
#include "AnyID_Stdlib.h"


//֡���ջ���������
#if (defined(_ANYID_STM32_) | defined(_ANYID_LM3S_) || defined(_ANYID_GD32_) )
    #define CP_FRAME_BUF_MAX_LEN                250
#elif (defined(_ANYID_PIC18_) || defined(_ANYID_STM8_))
    #define CP_FRAME_BUF_MAX_LEN                101
#endif
extern u8 g_aCPRcvBuffer[CP_FRAME_BUF_MAX_LEN];



extern const u8 CP_Version[];
#define CP_GetVersion(pVersion, len)        (pVersion = CP_Version, len = sizeof(CP_Version))



//��������ֽڵ�λ��
extern u16 g_nCPFrameRcvPos;
#define CP_GetFrameLen()                    (g_nCPFrameRcvPos)
#define CP_SetFrameRcvPos(pos)              (g_nCPFrameRcvPos = pos)

//ͬһ֡�ĸ����ֽ�֮��ļ��ʱ����50~100ms������100ms��ʾһ֡�������
extern u16 g_nCPFrameIdleTime;
//���ձ�ʶ
extern u8 g_nCPFrameRcvFlag;
//֡���ձ�ʶ
#define CP_FRAME_FLAG_IDLE                  0x00
#define CP_FRAME_FLAG_RCV                   0x01
#define CP_FRAME_FLAG_COPY                  0x02


//������ڽ���״̬���������ֽڽ��ռ��ʱ��
//����ѭ���м���ֽڽ��ռ��ʱ��g_nCPFrameIdleTime�����g_nCPFrameIdleTime > CP_FRAME_TIME����ʾ"֡"�������
#define CP_IncFrameTime(t)                  do{                                                    \
                                                if(g_nCPFrameRcvFlag == CP_FRAME_FLAG_RCV)          \
                                                {                                                   \
                                                    g_nCPFrameIdleTime += (t);                      \
                                                }                                                   \
                                             }while(0)
#define CP_ClearFrameTime()                 (g_nCPFrameIdleTime = 0)



//����֡״̬
#define CP_SetRcvFlag(flag)                 (g_nCPFrameRcvFlag = flag)
#define CP_GetRcvFlag()                     (g_nCPFrameRcvFlag)

//��֡�Ƿ�������:CP_FRAME_TIMEʱ����û�������ݵ�������������������
#define CP_IsRcvFrame(t)                     ((g_nCPFrameRcvFlag == CP_FRAME_FLAG_RCV) && (g_nCPFrameIdleTime >= (t)))
#define CP_IsIdleState(t)                    (((g_nCPFrameRcvFlag & CP_FRAME_FLAG_RCV) != CP_FRAME_FLAG_RCV) || (g_nCPFrameIdleTime > (t)))


//������������Ͳ���������
//�������������ѭ��COPY���Ͳ���������
//���ݽ�����У�����ռ��ʱ�䣬���ý��ձ�־
#define CP_ReceiveByte(byte)                do{                                                    \
                                                if((g_nCPFrameRcvPos < CP_FRAME_BUF_MAX_LEN) &&     \
                                                   (g_nCPFrameRcvFlag != CP_FRAME_FLAG_COPY))       \
                                                {                                                   \
                                                    g_aCPRcvBuffer[g_nCPFrameRcvPos++] = byte;      \
                                                    g_nCPFrameIdleTime = 0;                         \
                                                    g_nCPFrameRcvFlag = CP_FRAME_FLAG_RCV;          \
                                                }                                                   \
                                             }while(0)


//֡������ʽ
//֡ͷ��֡���ȡ�֡�����֡����������֡�е�λ��
#define CP_FRAME_HEAD1_POS                  0
#define CP_FRAME_HEAD2_POS                  1
#define CP_FRAME_SCRADDR_POS                2
#define CP_FRAME_DESADDR_POS                4
#define CP_FRAME_CTRL_POS                   6
#define CP_FRAME_CMD_POS                    7
#define CP_FRAME_STATE_POS                  8
#define CP_FRAME_DLEN_POS                   9
#define CP_FRAME_DSTART_POS                 10

#define CP_FRAME_ADDR_LEN                   2
#define CP_FRAME_CRC_LEN                    2

#define CP_FRAME_HEAD1_FLAG                 0x7E    //֡ͷ1
#define CP_FRAME_HEAD2_FLAG                 0x55    //֡ͷ2
#define CP_FRAME_TRAIL_FLAG                 0xAA    //֡β

#define CP_FRAME_BROADCAST_ADDR             0xFFFF  //�㲥��ַ

//������
#define CP_FRAME_CTRL_REQUEST               0x80    //����֡
#define CP_FRAME_CTRL_RESPONSE              0x00    //Ӧ��֡
#define CP_FRAME_CTRL_NEXT                  0x20    //���к���֡
#define CP_FRAME_CTRL_OVER                  0x00    //û�к���֡


//֡״̬���ò�����ʱֻ��Ӧ��֡������
#define CP_FRAME_STATE_RESPONE_OK           0x00
#define CP_FRAME_STATE_RESPONE_FAIL         0x01

#define CP_FRAME_STATE_REQUEST              0x00    //����֡ʱ���ò���ȫ�㣬������

//
#define CP_FRAME_NO_DATA                    0x00    //��������



//֡ͷ�ṹ
typedef struct cp_frame_head{
    union{                  //֡ͷ
        u8 head[2];
        struct{
            u8 head1;
            u8 head2;
        }sHead;
    }uHead;
    union{                  //Դ��ַ
        u8 scrAddr[2];
        struct{
            u8 scrAddrLo;
            u8 scrAddrHi;
        }sScrAddr;
    }uScrAddr;
    union{                  //Ŀ���ַ
        u8 desAddr[2];
        struct{
            u8 desAddrLo;
            u8 desAddrHi;
        }sDesAddr;
    }uDesAddr;
    u8 ctrl;                //������
    u8 cmd;                 //������
    u8 state;               //״̬��
    u8 dataLen;             //���ݳ���
} CP_FRAME_HEAD;

//֡β�ṹ
typedef struct cp_frame_trail{
    union{                  //CRC��
        u8 crc[2];
        struct{
            u8 crcLo;
            u8 crcHi;
        }sCrc;
    }uCrc;
    u8 trail;               //֡β
} CP_FRAME_TRAIL;

//��������󳤶�
#define CP_FRAME_DATA_MAX_LEN                       200
#define CP_FRAME_HEAD_LEN                           (sizeof(CP_FRAME_HEAD))
#define CP_FRAME_TRAIL_LEN                          (sizeof(CP_FRAME_TRAIL))


#define CP_SetScrAddr(pHead, addr)                  (*((u16 *)((pHead)->uScrAddr.scrAddr)) = (addr))
#define CP_SetDesAddr(pHead, addr)                  (*((u16 *)((pHead)->uDesAddr.desAddr)) = (addr))
#define CP_SetCrc(pTrail, crc)                      (*((u16 *)((pTrail)->uCrc.crc)) = (crc))
#define CP_GetScrAddr(pHead)                        (*((u16 *)((pHead)->uScrAddr.scrAddr)))
#define CP_GetDesAddr(pHead)                        (*((u16 *)((pHead)->uDesAddr.desAddr)))



BOOL CP_CheckCrc(u8 *pFrame, u8 len);
BOOL CP_CheckFrame(u8 *pInFrame, u16 inLen, u8 **pOutFrame, u16 *pOutLen);
void CP_Init(u16 scrAddr, u8 ctrl);
u8 CP_GetFrame(u16 scrAddr, u16 desAddr, u8 ctrl, u8 cmd, u8 state, u8 *pFrame, u8 dataLen);

void CP_Task(void (*process)(u8 *pFrame, u16 len), u8 delayms);

#if 0
	//����ר��
	#define CP_FRAME_CMD_TEST               0xFF
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//ͨ�Žӿ�
	#define CP_TEST_ITME_UART0              0x00    //����
	#define CP_TEST_ITME_UART1              0x01
	#define CP_TEST_ITME_UART2              0x02
	#define CP_TEST_ITME_UART3              0x03
	#define CP_TEST_ITME_UART4              0x04
	#define CP_TEST_ITME_NET0               0x05    //����
	#define CP_TEST_ITME_NET1               0x06
	#define CP_TEST_ITME_NET2               0x07
	
	//�洢��
	#define CP_TEST_ITME_FLASH              0x10    //�ⲿFLASH
	#define CP_TEST_ITME_EEPROM             0x11    //eeprom
	#define CP_TEST_ITME_FRAM               0x12    //����
	
	//�弶IO
	#define CP_TEST_ITME_LED                0x20    //LED
	#define CP_TEST_ITME_BEEP               0x21    //������
	#define CP_TEST_ITME_RELAY              0x22    //�̵���
	
	//����
	#define CP_TEST_ITME_CC2500             0x30    //CC2500����
	#define CP_TEST_ITME_MP3                0x31    //Mp3����
	#define CP_TEST_ITME_HFREADER           0x32    //HFReader����
	#define CP_TEST_ITME_UHFREADER          0x32    //UHFReader����
	#define CP_TEST_ITME_GPS                0x33    //GPS����
	#define CP_TEST_ITME_GPRS               0x34    //GPRS����
	#define CP_TEST_ITME_BLDC               0x35    //�������
	#define CP_TEST_ITME_LCD                0x36    //LCD����





//����֡
u8 CP_SysTestReqFrame(u8 *pFrame, u8 testItem);

#define CP_TEST_RESULT_OK               0x00
#define CP_TEST_RESULT_FAIL             0x01
//��Ӧ֡
u8 CP_SysTestRspFrame(u8 *pFrame, u8 result);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

//������2.4Gͨ��Э��
#define CP_RF_FRAME_LEN_POS           0x00
#define CP_RF_FRAME_ADR_POS           0x01
#define CP_RF_FRAME_CMD_POS           0x02
#define CP_RF_FRAME_DAT_POS           0x03

#endif
