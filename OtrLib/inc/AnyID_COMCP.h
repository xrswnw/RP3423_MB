#ifndef _ANYID_COMCP_
#define _ANYID_COMCP_

#include "AnyID_Config.h"
///////////////////////////////////////////////////////////////////////////////
extern u8 g_nCOMCPFrameStart;                       //֡��Buffer�е���ʼλ��
#define COMCP_SetFrameStart(len)            (g_nCOMCPFrameStart = len)
#define COMCP_GetFrameStart()               g_nCOMCPFrameStart

extern u32 g_nCOMCPRxOverTime;
#define COMCP_RX_OVERTIME             20
#define COMCP_IsRxOverTime()          (g_nCOMCPRxOverTime > COMCP_RX_OVERTIME)

///////////////////////////////////////////////////////////////////////////////
extern u8 g_nCOMCPState;                                //����״̬
#define COMCP_RXSTAT_IDLE                     0x80      //����״̬
#define COMCP_RXSTAT_HEAD1                    0x01      //���յ�֡ͷ1:0x7E
#define COMCP_RXSTAT_HEAD2                    0x02      //���յ�֡ͷ2:0x55
#define COMCP_RXSTAT_LEN                      0x04      //���յ�֡����
#define COMCP_RXSTAT_TRAIL                    0x08      //���յ�֡β:0xAA
#define COMCP_RXSTAT_START                    0x10



/*
#define COMCP_SetState(state)               (g_nCOMCPState = state)
#define COMCP_CheckState(state)             ((g_nCOMCPState & state) == state)
*/

///////////////////////////////////////////////////////////////////////////////
#define COMCP_FRAME_HEAD1                   0x7E    //֡ͷ1
#define COMCP_FRAME_HEAD2                   0x55    //֡ͷ2
#define COMCP_FRAME_TRAIL                   0xAA    //֡β
#define COMCP_FRAME_HEAD_LEN                2       //֡ͷ����:2�ֽ�


////////////////////////////////////////////////////////////////////////////////////
//����֡������ʽ
//֡ͷ��֡���ȡ�֡�����֡����������֡�е�λ��
#define COMCP_FRAME_HEAD1_POS               0
#define COMCP_FRAME_HEAD2_POS               1
#define COMCP_FRAME_LEN_POS                 2
#define COMCP_FRAME_CMD_POS                 3
#define COMCP_FRAME_PAR_POS                 4

////////////////////////////////////////////////////////////////////////////////////
//��Ӧ֡������ʽ
#define COMCP_RFRAME_HEAD1_POS              0
#define COMCP_RFRAME_HEAD2_POS              1
#define COMCP_RFRAME_LEN_POS                2
#define COMCP_RFRAME_CMD_POS                3
#define COMCP_RFRAME_RSPF_POS               4       //��Ӧ��ʶ
#define COMCP_RFRAME_OKF_POS                5       //�Ƿ�ɹ���ʶ: 00 ʧ�ܣ� 01 �ɹ�
#define COMCP_RFRAME_ERRC_POS               6       //������
#define COMCP_RFRAME_PAR_POS                6       //��Ӧ֡�ɹ�����������ʼλ��
#define COMCP_RFRAME_FPAR_POS               7       //��Ӧ֡�������������ʼλ��


#define COMCP_RFRAME_OK_FLAG                0x01    //�ɹ���Ӧ֡��ʶ
#define COMCP_RFRAME_FAIL_FLAG              0x00    //������Ӧ֡��ʶ


////////////////////////////////////////////////////////////////////////////////////
//������Ӧ֡��ʽ:���еĴ�����Ӧ֡�ĸ�ʽ����һ����
#define COMCP_RFRAME_FFLEN                  6       //֡����
#define COMCP_RFRAME_RSP_FLAG               0x1F    //��Ӧ֡��־
#define COMCP_RFRAME_FBCC_POS               (COMCP_RFRAME_ERRC_POS + 1)
#define COMCP_RFRAME_FTRL_POS               (COMCP_RFRAME_FBCC_POS + 1)

//�޲�������ȷ��Ӧ֡��ʽ:���е��޲�������ȷ��Ӧ֡�ĸ�ʽ����һ����
#define COMCP_RFRAME_OFLEN                  5       //֡����
#define COMCP_RFRAME_RSP_FLAG               0x1F    //��Ӧ֡��־
#define COMCP_RFRAME_OBCC_POS               (COMCP_RFRAME_OKF_POS + 1)
#define COMCP_RFRAME_OTRL_POS               (COMCP_RFRAME_OBCC_POS + 1)

//�޲���������֡
#define COMCP_FRAME_NOPAR_RQST_LEN          3
#define COMCP_FRAME_NOPAR_RQST_BCC_POS      (COMCP_FRAME_PAR_POS)
#define COMCP_FRAME_NOPAR_RQST_TRL_POS      (COMCP_FRAME_NOPAR_RQST_BCC_POS + 1)



u8 COMCP_GetCheckBCC(u8 *pBuffer, u8 len);
BOOL COMCP_CheckFrame(u8 *pFrame);
u8 COMCP_ErrorRspFrame(u8 *pFrame, u8 cmd, u8 errorCode);
u8 COMCP_OkRspNoParFrame(u8 *pFrame, u8 cmd);
u8 COMCP_NoParRequestFrame(u8 *pFrame, u8 cmd);


///////////////////////////////////////////////////////////////////////////////
#if (defined(_ANYID_STM32_MMD_))

    //PC��MMDд��Ȩ��Ϣ��MMD��PC����Ӧ��Ȩ֡��ʽ
    #define COMCP_FRAME_LICINF_NUMBER           100
    #define COMCP_FRAME_LICINF_LEN              22


///////////////////////////////////////////////////////////////////////////////////
    //��Ȩ��Ϣ��ʽ
    #define COMCP_LIC_CPUID_POS                 0
    #define COMCP_LIC_CPUID_LEN                 12
    //�ô�MMD���ø�MTD����Ȩ����
    #define COMCP_LIC_CUR_LIC_POS               (COMCP_LIC_CPUID_POS + COMCP_LIC_CPUID_LEN)
    #define COMCP_LIC_CUR_LIC_LEN               2
    //��MMD�ܹ���MTD������Ȩ�Ĵ���
    #define COMCP_LIC_TOTAL_LIC_POS             (COMCP_LIC_CUR_LIC_POS + COMCP_LIC_CUR_LIC_LEN)
    #define COMCP_LIC_TOTAL_LIC_LEN             2
    //��MTD������N̨���λ�
    #define COMCP_LIC_CUR_CFGT_POS              (COMCP_LIC_TOTAL_LIC_POS + COMCP_LIC_TOTAL_LIC_LEN)
    #define COMCP_LIC_CUR_CFGT_LEN              2
    //��MTD�ܹ�������N̨���λ�
    #define COMCP_LIC_TOTAL_CFGT_POS            (COMCP_LIC_CUR_CFGT_POS + COMCP_LIC_CUR_CFGT_LEN)
    #define COMCP_LIC_TOTAL_CFGT_LEN            2
    //����Ȩ��Ϣ�洢�����
    #define COMCP_LIC_ID_POS                    (COMCP_LIC_TOTAL_CFGT_POS + COMCP_LIC_TOTAL_CFGT_LEN)
    #define COMCP_LIC_ID_LEN                    1
    //����Ȩ��Ϣ�洢�ռ�״̬��Ϣ
    #define COMCP_LIC_STAT_POS                  (COMCP_LIC_ID_POS + COMCP_LIC_ID_LEN)
    #define COMCP_LIC_STAT_LEN                  1

    #define COMCP_LIC_STAT_IDLE                 0x00
    #define COMCP_LIC_STAT_USED                 0x01


///////////////////////////////////////////////////////////////////////////////////
    //��MMD����Ȩ��Ϣ����֡
    #define COMCP_FRAME_CMD_RLIC                0x01
    #define COMCP_FRAME_RLIC_LEN                0x0F
    #define COMCP_FRAME_RLIC_CPUID_POS          COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_RLIC_CPUID_LEN          12
    #define COMCP_FRAME_RLIC_BCC_POS           (COMCP_FRAME_RLIC_CPUID_POS + COMCP_FRAME_RLIC_CPUID_LEN)
    #define COMCP_FRAME_RLIC_BCC_LEN           1
    #define COMCP_FRAME_RLIC_TRL_POS            (COMCP_FRAME_RLIC_BCC_POS + COMCP_FRAME_RLIC_BCC_LEN)


    //��MMD����Ȩ��Ϣ��Ӧ֡
    #define COMCP_RFRAME_RLIC_OLEN              0x1B        //����OKʱ����
    #define COMCP_RFRAME_RLIC_OINF_POS          COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_RLIC_OINF_LEN          COMCP_FRAME_LICINF_LEN
    #define COMCP_RFRAME_RLIC_OBCC_POS         (COMCP_RFRAME_RLIC_OINF_POS + COMCP_RFRAME_RLIC_OINF_LEN)
    #define COMCP_RFRAME_RLIC_OBCC_LEN         1
    #define COMCP_RFRAME_RLIC_OTRL_POS          (COMCP_RFRAME_RLIC_OBCC_POS + COMCP_RFRAME_RLIC_OBCC_LEN)


///////////////////////////////////////////////////////////////////////////////////
    //дMMD����Ȩ��Ϣ����֡
    #define COMCP_FRAME_CMD_WLIC                0x02
    #define COMCP_FRAME_WLIC_LEN                0x19
    #define COMCP_FRAME_WLIC_INF_POS            COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_WLIC_INF_LEN            COMCP_FRAME_LICINF_LEN
    #define COMCP_FRAME_WLIC_BCC_POS            (COMCP_FRAME_WLIC_INF_POS + COMCP_FRAME_WLIC_INF_LEN)
    #define COMCP_FRAME_WLIC_BCC_LEN            1
    #define COMCP_FRAME_WLIC_TRL_POS            (COMCP_FRAME_WLIC_BCC_POS + COMCP_FRAME_WLIC_BCC_LEN)


    //дMMD����Ȩ��Ϣ��Ӧ֡
    #define COMCP_RFRAME_WLIC_OLEN              0x05        //����OKʱ����
    #define COMCP_RFRAME_WLIC_OPAR_POS          COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_WLIC_OPAR_LEN          0
    #define COMCP_RFRAME_WLIC_OBCC_POS          (COMCP_RFRAME_WLIC_OPAR_POS + COMCP_RFRAME_WLIC_OPAR_LEN)
    #define COMCP_RFRAME_WLIC_OBCC_LEN          1
    #define COMCP_RFRAME_WLIC_OTRL_POS          (COMCP_RFRAME_WLIC_OBCC_POS + COMCP_RFRAME_WLIC_OBCC_LEN)



///////////////////////////////////////////////////////////////////////////////
    //��ʽ������������֡
    #define COMCP_FRAME_CMD_FMT                 0x03

    #define COMCP_FRAME_FMT_LEN                 0x04
    #define COMCP_FRAME_FMT_ADDR_POS            COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_FMT_ADDR_LEN            1
    #define COMCP_FRAME_FMT_BCC_POS             (COMCP_FRAME_FMT_ADDR_POS + COMCP_FRAME_FMT_ADDR_LEN)
    #define COMCP_FRAME_FMT_BCC_LEN             1
    #define COMCP_FRAME_FMT_TRL_POS             (COMCP_FRAME_FMT_BCC_POS + COMCP_FRAME_FMT_BCC_LEN)
    #define COMCP_FRAME_FMT_ALL                 0x00

    //��ʽ����������Ӧ֡
    #define COMCP_RFRAME_FMT_OLEN               0x05        //����OKʱ����
    #define COMCP_RFRAME_FMT_OPAR_POS           COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_FMT_OPAR_LEN           0
    #define COMCP_RFRAME_FMT_OBCC_POS           (COMCP_RFRAME_FMT_OPAR_POS + COMCP_RFRAME_FMT_OPAR_LEN)
    #define COMCP_RFRAME_FMT_OBCC_LEN           1
    #define COMCP_RFRAME_FMT_OTRL_POS           (COMCP_RFRAME_FMT_OBCC_POS + COMCP_RFRAME_FMT_OBCC_LEN)



///////////////////////////////////////////////////////////////////////////////
    //��ȡ��Ȩ��Ϣ��Ŷ�Ӧ����Ȩ��Ϣ����֡
    #define COMCP_FRAME_CMD_RLICID              0x04

    #define COMCP_FRAME_RLICID_LEN              0x04
    #define COMCP_FRAME_RLICID_LICID_POS        COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_RLICID_LICID_LEN        1
    #define COMCP_FRAME_RLICID_BCC_POS          (COMCP_FRAME_RLICID_LICID_POS + COMCP_FRAME_RLICID_LICID_LEN)
    #define COMCP_FRAME_RLICID_BCC_LEN          1
    #define COMCP_FRAME_RLICID_TRL_POS          (COMCP_FRAME_RLICID_BCC_POS + COMCP_FRAME_RLICID_BCC_LEN)


    //��ȡ��Ȩ��Ϣ��Ŷ�Ӧ����Ȩ����Ӧ֡
    #define COMCP_RFRAME_RLICID_OLEN            0x1B        //����OKʱ����
    #define COMCP_RFRAME_RLICID_OINF_POS        COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_RLICID_OINF_LEN        COMCP_FRAME_LICINF_LEN
    #define COMCP_RFRAME_RLICID_OBCC_POS        (COMCP_RFRAME_RLICID_OINF_POS + COMCP_RFRAME_RLICID_OINF_LEN)
    #define COMCP_RFRAME_RLICID_OBCC_LEN        1
    #define COMCP_RFRAME_RLICID_OTRL_POS        (COMCP_RFRAME_RLICID_OBCC_POS + COMCP_RFRAME_RLICID_OBCC_LEN)

///////////////////////////////////////////////////////////////////////////////////
    //д��Ȩ��Ϣ��Ŷ�Ӧ����Ȩ��Ϣ����֡
    #define COMCP_FRAME_CMD_WLICID              0x05

    #define COMCP_FRAME_WLICID_LEN              0x1A
    #define COMCP_FRAME_WLICID_INF_POS          COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_WLICID_INF_LEN          COMCP_FRAME_LICINF_LEN
    #define COMCP_FRAME_WLICID_ID_POS           (COMCP_FRAME_PAR_POS + COMCP_FRAME_LICINF_LEN)
    #define COMCP_FRAME_WLICID_ID_LEN           1
    #define COMCP_FRAME_WLICID_BCC_POS          (COMCP_FRAME_WLICID_ID_POS + COMCP_FRAME_WLICID_ID_LEN)
    #define COMCP_FRAME_WLICID_BCC_LEN          1
    #define COMCP_FRAME_WLICID_TRL_POS          (COMCP_FRAME_WLICID_BCC_POS + COMCP_FRAME_WLICID_BCC_LEN)


    //д��Ȩ��Ϣ��Ŷ�Ӧ����Ȩ��Ϣ��Ӧ֡
    #define COMCP_RFRAME_WLICID_OLEN            0x05        //����OKʱ����
    #define COMCP_RFRAME_WLICID_OPAR_POS        COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_WLICID_OPAR_LEN        0
    #define COMCP_RFRAME_WLICID_OBCC_POS        (COMCP_RFRAME_WLIC_OPAR_POS + COMCP_RFRAME_WLIC_OPAR_LEN)
    #define COMCP_RFRAME_WLICID_OBCC_LEN        1
    #define COMCP_RFRAME_WLICID_OTRL_POS        (COMCP_RFRAME_WLIC_OBCC_POS + COMCP_RFRAME_WLIC_OBCC_LEN)


///////////////////////////////////////////////////////////////////////////////////
/*
    u8 COMCP_WriteLICFrame(u8 *pFrame, u8 *pLicence);
    u8 COMCP_WriteLICIDFrame(u8 *pFrame, u8 licID, u8 *pLicence);
    u8 COMCP_FormatLICFrame(u8 *pFrame, u8 addr);
    u8 COMCP_ReadLICFrame(u8 *pFrame, u8 *pCPUID);
    u8 COMCP_ReadLICIDFrame(u8 *pFrame, u8 licID);
*/
    u8 COMCP_WriteLICRspFrame(u8 *pFrame);
    u8 COMCP_WriteLICIDRspFrame(u8 *pFrame);
    u8 COMCP_FormatLICRspFrame(u8 *pFrame);
    u8 COMCP_ReadLICIDRspFrame(u8 *pFrame, u8 *pLicence);
    u8 COMCP_ReadLICRspFrame(u8 *pFrame, u8 *pLicence);
    ///////////////////////////////////////////////////////////////////////////////
#elif defined(_ANYID_STM32_MPD_)

    //���֧�ֵ�MID����
    #define COMCP_MIDINF_NUM                256
    #define COMCP_MIDINF_LEN                3



    //MID��Ϣ�Ĵ洢��ʽ
    #define COMCP_MIDINF_VOL_POS            0
    #define COMCP_MIDINF_VOL_LEN            2
    #define COMCP_MIDINF_POW_POS            (COMCP_MIDINF_VOL_POS + COMCP_MIDINF_VOL_LEN)
    #define COMCP_MIDINF_POW_LEN            1

    ///////////////////////////////////////////////////////////////////////////////////
    //��MID����Ϣ����֡
    #define COMCP_FRAME_CMD_READ                0x06
    #define COMCP_FRAME_READ_LEN                0x05
    #define COMCP_FRAME_READ_ID_POS             COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_READ_ID_LEN             2
    #define COMCP_FRAME_READ_BCC_POS            (COMCP_FRAME_READ_ID_POS + COMCP_FRAME_READ_ID_LEN)
    #define COMCP_FRAME_READ_BCC_LEN            1
    #define COMCP_FRAME_READ_TRL_POS            (COMCP_FRAME_READ_BCC_POS + COMCP_FRAME_READ_BCC_LEN)


    //��MID����Ϣ��Ӧ֡
    #define COMCP_RFRAME_READ_OLEN              0x08        //����OKʱ����
    #define COMCP_RFRAME_READ_OINF_POS          COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_READ_OINF_LEN          COMCP_MIDINF_LEN
    #define COMCP_RFRAME_READ_OBCC_POS          (COMCP_RFRAME_READ_OINF_POS + COMCP_RFRAME_READ_OINF_LEN)
    #define COMCP_RFRAME_READ_OBCC_LEN          1
    #define COMCP_RFRAME_READ_OTRL_POS          (COMCP_RFRAME_READ_OBCC_POS + COMCP_RFRAME_READ_OBCC_LEN)


    ///////////////////////////////////////////////////////////////////////////////
    //��ʽ��Ѳ�������֡
    #define COMCP_FRAME_CMD_FMT                 0x07

    #define COMCP_FRAME_FMT_LEN                 0x05
    #define COMCP_FRAME_FMT_ADDR_POS            COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_FMT_ADDR_LEN            2
    #define COMCP_FRAME_FMT_BCC_POS             (COMCP_FRAME_FMT_ADDR_POS + COMCP_FRAME_FMT_ADDR_LEN)
    #define COMCP_FRAME_FMT_BCC_LEN             1
    #define COMCP_FRAME_FMT_TRL_POS             (COMCP_FRAME_FMT_BCC_POS + COMCP_FRAME_FMT_BCC_LEN)
    #define COMCP_FRAME_FMT_ALL                 0xFFFF

    //��ʽ��Ѳ�����Ӧ֡
    #define COMCP_RFRAME_FMT_OLEN               0x05        //����OKʱ����
    #define COMCP_RFRAME_FMT_OPAR_POS           COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_FMT_OPAR_LEN           0
    #define COMCP_RFRAME_FMT_OBCC_POS           (COMCP_RFRAME_FMT_OPAR_POS + COMCP_RFRAME_FMT_OPAR_LEN)
    #define COMCP_RFRAME_FMT_OBCC_LEN           1
    #define COMCP_RFRAME_FMT_OTRL_POS           (COMCP_RFRAME_FMT_OBCC_POS + COMCP_RFRAME_FMT_OBCC_LEN)

    u8 COMCP_FormatMIDInfRspFrame(u8 *pFrame);
    u8 COMCP_ReadMIDInfRspFrame(u8 *pFrame, u8 *pInf);

#elif (defined(_ANYID_PIC18_HSR100_))

    /////////////////////////////////////////////////////////////////////////////////////
    //Mifare ����������
    #define COMCP_FRAME_PWD_LEN                 6       //��Կ����
    #define COMCP_FRAME_KEYT_A                  0x60    //KEYA
    #define COMCP_FRAME_KEYT_B                  0x61    //KEYB
    #define COMCP_FRAME_RSQT_MODE_IDLE          0x26    //����ģʽ:IDLE
    #define COMCP_FRAME_RSQT_MODE_ALL           0x52    //����ģʽ:ALL
    //����Ǯ������ģʽ
    #define COMCP_FRAME_VALUE_MODE_DEC          0xC0    //��ֵ
    #define COMCP_FRAME_VALUE_MODE_INC          0xC1    //��ֵ
    #define COMCP_FRAME_VALUE_MODE_RSTOR        0xC2    //����
    #define COMCP_FRAME_VALUE_MODE_TRANS        0xB0    //ת��



    ///////////////////////////////////////////////////////////////////////////////
    //��λ��д������
    #define COMCP_FRAME_RST_FLEN                3
    #define COMCP_FRAME_CMD_RST                 0x52
    #define COMCP_FRAME_RST_BCC_POS             COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_RST_TRL_POS             (COMCP_FRAME_RST_BCC_POS + 1)
    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_RST_OFLEN              5
    #define COMCP_RFRAME_RST_OBCC_POS           COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_RST_OTRL_POS           (COMCP_RFRAME_RST_OBCC_POS + 1)



    ///////////////////////////////////////////////////////////////////////////////
    //������������
    #define COMCP_FRAME_CANT_FLEN               4
    #define COMCP_FRAME_CMD_CANT                0x53
    #define COMCP_FRAME_CANT_STAT_POS           COMCP_FRAME_PAR_POS     //��������״̬:��/��
    #define COMCP_FRAME_CANT_STAT_LEN           1
    #define COMCP_FRAME_CANT_BCC_POS            (COMCP_FRAME_CANT_STAT_POS + COMCP_FRAME_CANT_STAT_LEN)
    #define COMCP_FRAME_CANT_TRL_POS            (COMCP_FRAME_CANT_BCC_POS + 1)

    #define COMCP_FRAME_CANT_STAT_ON            0x01                    //������
    #define COMCP_FRAME_CANT_STAT_OFF           0x00                    //������

    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_CANT_OFLEN             5
    #define COMCP_RFRAME_CANT_OBCC_POS          COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_CANT_OTRL_POS          (COMCP_RFRAME_CANT_OBCC_POS + 1)



    ///////////////////////////////////////////////////////////////////////////////
    //��ȡUID: �õ�����ID������������Active״̬
    #define COMCP_FRAME_GUID_FLEN               4
    #define COMCP_FRAME_CMD_GUID                0x54
    #define COMCP_FRAME_GUID_MODE_POS           COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_GUID_MODE_LEN           1
    #define COMCP_FRAME_GUID_BCC_POS            (COMCP_FRAME_GUID_MODE_POS + COMCP_FRAME_GUID_MODE_LEN)
    #define COMCP_FRAME_GUID_TRL_POS            (COMCP_FRAME_GUID_BCC_POS + 1)
    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_GUID_OFLEN             5                      //ע��:�����û�м�������ĳ��ȵģ���Ϊ���ﳤ���ǿɱ��
    #define COMCP_RFRAME_GUID_OCTYPE_POS        COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_GUID_OCTYPE_LEN        2
    #define COMCP_RFRAME_GUID_IDLEN_POS         (COMCP_RFRAME_GUID_OCTYPE_POS + COMCP_RFRAME_GUID_OCTYPE_LEN)
    #define COMCP_RFRAME_GUID_IDLEN_LEN         1
    #define COMCP_RFRAME_GUID_OUID_POS          (COMCP_RFRAME_GUID_IDLEN_POS + COMCP_RFRAME_GUID_IDLEN_LEN)




    ///////////////////////////////////////////////////////////////////////////////
    //����ģʽ:��д�����յ���֡����ALL��ģʽѭ����ȡ����UID�����������ó�Active״̬��
    //���50ms�ڣ�û�н��յ���λ������������ʱ���������ó�HALT״̬
    //��˷���ѭ��
    //�Զ���ȡUID
    #define COMCP_FRAME_AGUID_FLEN               3
    #define COMCP_FRAME_CMD_AGUID                0x55
    #define COMCP_FRAME_AGUID_BCC_POS            COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_AGUID_TRL_POS            (COMCP_FRAME_AGUID_BCC_POS + 1)
    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_AGUID_OFLEN             5                  //ע��:�����û�м�������ĳ��ȵģ���Ϊ���ﳤ���ǿɱ��
    #define COMCP_RFRAME_AGUID_OCTYPE_POS        COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_AGUID_OCTYPE_LEN        2
    #define COMCP_RFRAME_AGUID_IDLEN_POS         (COMCP_RFRAME_AGUID_OCTYPE_POS + COMCP_RFRAME_AGUID_OCTYPE_LEN)
    #define COMCP_RFRAME_AGUID_IDLEN_LEN         1
    #define COMCP_RFRAME_AGUID_OUID_POS          (COMCP_RFRAME_AGUID_IDLEN_POS + COMCP_RFRAME_AGUID_IDLEN_LEN)



    ///////////////////////////////////////////////////////////////////////////////
    //halt����
    #define COMCP_FRAME_HALT_FLEN                3
    #define COMCP_FRAME_CMD_HALT                 0x56
    #define COMCP_FRAME_HALT_BCC_POS             COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_HALT_TRL_POS             (COMCP_FRAME_HALT_BCC_POS + 1)
    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_HALT_OFLEN              5
    #define COMCP_RFRAME_HALT_OBCC_POS           COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_HALT_OTRL_POS           (COMCP_RFRAME_HALT_OBCC_POS + 1)

    /*
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //change pwd
    #define COMCP_FRAME_CHPWD_FLEN              15
    #define COMCP_FRAME_CMD_CHPWD               0x57

    #define COMCP_FRAME_CHPWD_ADDR_POS          COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_CHPWD_ADDR_LEN          1
    #define COMCP_FRAME_CHPWD_OPWD_POS          (COMCP_FRAME_CHPWD_ADDR_POS + COMCP_FRAME_CHPWD_ADDR_LEN)
    #define COMCP_FRAME_CHPWD_OPWD_LEN           COMCP_FRAME_PWD_LEN
    #define COMCP_FRAME_CHPWD_NPWD_POS          (COMCP_FRAME_CHPWD_OPWD_POS + COMCP_FRAME_CHPWD_OPWD_LEN)
    #define COMCP_FRAME_CHPWD_NPWD_LEN           COMCP_FRAME_PWD_LEN
    #define COMCP_FRAME_CHPWD_BCC_POS           (COMCP_FRAME_CHPWD_NPWD_POS + COMCP_FRAME_CHPWD_NPWD_LEN)
    #define COMCP_FRAME_CHPWD_TRL_POS           (COMCP_FRAME_CHPWD_BCC_POS + 1)
    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_CHPWD_OFLEN                5
    #define COMCP_RFRAME_CHPWD_OBCC_POS             COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_CHPWD_OTRL_POS             (COMCP_RFRAME_CHPWD_OBCC_POS + 1)
    */

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //��Ȩд:ͨ����Կ��֤��Ӧ����������Ȼ������д�����ݿ�
    #define COMCP_FRAME_AUTHW_FLEN               27
    #define COMCP_FRAME_CMD_AUTHW                0x11
    #define COMCP_FRAME_AUTHW_KEYT_POS           COMCP_FRAME_PAR_POS                                         //��Ȩģʽ
    #define COMCP_FRAME_AUTHW_KEYT_LEN           1
    #define COMCP_FRAME_AUTHW_PWD_POS            (COMCP_FRAME_AUTHW_KEYT_POS + COMCP_FRAME_AUTHW_KEYT_LEN)   //6�ֽ���Կ
    #define COMCP_FRAME_AUTHW_PWD_LEN            COMCP_FRAME_PWD_LEN
    #define COMCP_FRAME_AUTHW_ADDR_POS           (COMCP_FRAME_AUTHW_PWD_POS + COMCP_FRAME_AUTHW_PWD_LEN)     //���ݿ��ַ
    #define COMCP_FRAME_AUTHW_ADDR_LEN           1
    #define COMCP_FRAME_AUTHW_DATA_POS           (COMCP_FRAME_AUTHW_ADDR_POS + COMCP_FRAME_AUTHW_ADDR_LEN)   //16�ֽ�����
    #define COMCP_FRAME_AUTHW_DATA_LEN           16
    #define COMCP_FRAME_AUTHW_BCC_POS            (COMCP_FRAME_AUTHW_DATA_POS + COMCP_FRAME_AUTHW_DATA_LEN)
    #define COMCP_FRAME_AUTHW_TRL_POS            (COMCP_FRAME_AUTHW_BCC_POS + 1)
    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_AUTHW_OFLEN             5
    #define COMCP_RFRAME_AUTHW_OBCC_POS          COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_AUTHW_OTRL_POS          (COMCP_RFRAME_AUTHW_OBCC_POS + 1)


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //��Ȩ��:ͨ����Կ��֤��Ӧ����������Ȼ���ȡ����
    #define COMCP_FRAME_AUTHR_FLEN               11
    #define COMCP_FRAME_CMD_AUTHR                0x12
    #define COMCP_FRAME_AUTHR_KEYT_POS           COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_AUTHR_KEYT_LEN           1
    #define COMCP_FRAME_AUTHR_PWD_POS            (COMCP_FRAME_AUTHR_KEYT_POS + COMCP_FRAME_AUTHR_KEYT_LEN)
    #define COMCP_FRAME_AUTHR_PWD_LEN            COMCP_FRAME_PWD_LEN
    #define COMCP_FRAME_AUTHR_ADDR_POS           (COMCP_FRAME_AUTHR_PWD_POS + COMCP_FRAME_AUTHR_PWD_LEN)
    #define COMCP_FRAME_AUTHR_ADDR_LEN           1
    #define COMCP_FRAME_AUTHR_BCC_POS            (COMCP_FRAME_AUTHR_ADDR_POS + COMCP_FRAME_AUTHR_ADDR_LEN)
    #define COMCP_FRAME_AUTHR_TRL_POS            (COMCP_FRAME_AUTHR_BCC_POS + 1)
    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_AUTHR_OFLEN             22
    #define COMCP_RFRAME_AUTHR_OADDR_POS         COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_AUTHR_OADDR_LEN         1
    #define COMCP_RFRAME_AUTHR_ODATA_POS         (COMCP_RFRAME_AUTHR_OADDR_POS + COMCP_RFRAME_AUTHR_OADDR_LEN)
    #define COMCP_RFRAME_AUTHR_ODATA_LEN         16
    #define COMCP_RFRAME_AUTHR_OBCC_POS          (COMCP_RFRAME_AUTHR_ODATA_POS + COMCP_RFRAME_AUTHR_ODATA_LEN)
    #define COMCP_RFRAME_AUTHR_OTRL_POS          (COMCP_RFRAME_AUTHR_OBCC_POS + 1)


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //��Ȩдֵ:ͨ����Կ��֤��Ӧ����������Ȼ��ֵ�͵�ַ����ֵ��ʽ���16�ֽ����ݿ飬��д��ָ��Ŀ��ַ
    #define COMCP_FRAME_AUTHWV_FLEN              15
    #define COMCP_FRAME_CMD_AUTHWV               0x13
    #define COMCP_FRAME_AUTHWV_KEYT_POS          COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_AUTHWV_KEYT_LEN          1
    #define COMCP_FRAME_AUTHWV_PWD_POS           (COMCP_FRAME_AUTHWV_KEYT_POS + COMCP_FRAME_AUTHWV_KEYT_LEN)
    #define COMCP_FRAME_AUTHWV_PWD_LEN           COMCP_FRAME_PWD_LEN
    #define COMCP_FRAME_AUTHWV_ADDR_POS          (COMCP_FRAME_AUTHWV_PWD_POS + COMCP_FRAME_AUTHWV_PWD_LEN)
    #define COMCP_FRAME_AUTHWV_ADDR_LEN          1
    #define COMCP_FRAME_AUTHWV_VALUE_POS         (COMCP_FRAME_AUTHWV_ADDR_POS + COMCP_FRAME_AUTHWV_ADDR_LEN)
    #define COMCP_FRAME_AUTHWV_VALUE_LEN         4
    #define COMCP_FRAME_AUTHWV_BCC_POS           (COMCP_FRAME_AUTHWV_VALUE_POS + COMCP_FRAME_AUTHWV_VALUE_LEN)
    #define COMCP_FRAME_AUTHWV_TRL_POS           (COMCP_FRAME_AUTHWV_BCC_POS + 1)
    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_AUTHWV_OFLEN            5
    #define COMCP_RFRAME_AUTHWV_OBCC_POS         COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_AUTHWV_OTRL_POS         (COMCP_RFRAME_AUTHWV_OBCC_POS + 1)



    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //��Ȩֵ����:ͨ����Կ��֤��Ӧ����������Ȼ�����ֵ������ģʽ��ֵ����ɶ���������ֵ����
    #define COMCP_FRAME_AUTHV_FLEN               17
    #define COMCP_FRAME_CMD_AUTHV                0x14
    #define COMCP_FRAME_AUTHV_KEYT_POS           COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_AUTHV_KEYT_LEN           1
    #define COMCP_FRAME_AUTHV_PWD_POS            (COMCP_FRAME_AUTHV_KEYT_POS + COMCP_FRAME_AUTHV_KEYT_LEN)
    #define COMCP_FRAME_AUTHV_PWD_LEN            COMCP_FRAME_PWD_LEN
    #define COMCP_FRAME_AUTHV_MODE_POS           (COMCP_FRAME_AUTHV_PWD_POS + COMCP_FRAME_AUTHV_PWD_LEN)    //����ģʽ
    #define COMCP_FRAME_AUTHV_MODE_LEN           1
    #define COMCP_FRAME_AUTHV_ADDR_POS           (COMCP_FRAME_AUTHV_MODE_POS + COMCP_FRAME_AUTHV_MODE_LEN)  //������ַ
    #define COMCP_FRAME_AUTHV_ADDR_LEN           1
    #define COMCP_FRAME_AUTHV_VALUE_POS          (COMCP_FRAME_AUTHV_ADDR_POS + COMCP_FRAME_AUTHV_ADDR_LEN)  //ֵ
    #define COMCP_FRAME_AUTHV_VALUE_LEN          4
    #define COMCP_FRAME_AUTHV_TADDR_POS          (COMCP_FRAME_AUTHV_VALUE_POS + COMCP_FRAME_AUTHV_VALUE_LEN)//���͵�ַ
    #define COMCP_FRAME_AUTHV_TADDR_LEN          1
    #define COMCP_FRAME_AUTHV_BCC_POS            (COMCP_FRAME_AUTHV_TADDR_POS + COMCP_FRAME_AUTHV_TADDR_LEN)
    #define COMCP_FRAME_AUTHV_TRL_POS            (COMCP_FRAME_AUTHV_BCC_POS + 1)
    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_AUTHV_OFLEN             5
    #define COMCP_RFRAME_AUTHV_OBCC_POS          COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_AUTHV_OTRL_POS          (COMCP_RFRAME_AUTHV_OBCC_POS + 1)



    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //��Ȩ��ֵ:ͨ����Կ��֤��Ӧ����������Ȼ��ָ���ַ��ֵ����
    #define COMCP_FRAME_AUTHRV_FLEN              11
    #define COMCP_FRAME_CMD_AUTHRV               0x15
    #define COMCP_FRAME_AUTHRV_KEYT_POS          COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_AUTHRV_KEYT_LEN          1
    #define COMCP_FRAME_AUTHRV_PWD_POS           (COMCP_FRAME_AUTHRV_KEYT_POS + COMCP_FRAME_AUTHRV_KEYT_LEN)
    #define COMCP_FRAME_AUTHRV_PWD_LEN           COMCP_FRAME_PWD_LEN
    #define COMCP_FRAME_AUTHRV_ADDR_POS          (COMCP_FRAME_AUTHRV_PWD_POS + COMCP_FRAME_AUTHRV_PWD_LEN)
    #define COMCP_FRAME_AUTHRV_ADDR_LEN          1
    #define COMCP_FRAME_AUTHRV_BCC_POS           (COMCP_FRAME_AUTHRV_ADDR_POS + COMCP_FRAME_AUTHRV_ADDR_LEN)
    #define COMCP_FRAME_AUTHRV_TRL_POS           (COMCP_FRAME_AUTHRV_BCC_POS + 1)
    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_AUTHRV_OFLEN            10
    #define COMCP_RFRAME_AUTHRV_OADDR_POS        COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_AUTHRV_OADDR_LEN        1
    #define COMCP_RFRAME_AUTHRV_OVALUE_POS       (COMCP_RFRAME_AUTHRV_OADDR_POS + COMCP_RFRAME_AUTHRV_OADDR_LEN)
    #define COMCP_RFRAME_AUTHRV_OVALUE_LEN       4
    #define COMCP_RFRAME_AUTHRV_OBCC_POS         (COMCP_RFRAME_AUTHRV_OVALUE_POS + COMCP_RFRAME_AUTHRV_OVALUE_LEN)
    #define COMCP_RFRAME_AUTHRV_OTRL_POS         (COMCP_RFRAME_AUTHRV_OBCC_POS + 1)



    //���µ������������ܵĶ��������Ĳ����������ƣ�ֻ�ǲ���Ҫ��Ȩ
    //��Ϊһ������ֻҪ����һ����Ȩ��֤�Ϳ����ˣ�
    //�����û���ĳһ�������е�ĳһ�������Ȩ��֤�Ժ󣬶������Ŀ�����Ͳ�Ҫ��Ҫ�ٴ�������֤��
    //�����û��޸�������
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //write
    #define COMCP_FRAME_W_FLEN                   20
    #define COMCP_FRAME_CMD_W                    0x16
    #define COMCP_FRAME_W_ADDR_POS               COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_W_ADDR_LEN               1
    #define COMCP_FRAME_W_DATA_POS               (COMCP_FRAME_W_ADDR_POS + COMCP_FRAME_W_ADDR_LEN)
    #define COMCP_FRAME_W_DATA_LEN               16
    #define COMCP_FRAME_W_BCC_POS                (COMCP_FRAME_W_DATA_POS + COMCP_FRAME_W_DATA_LEN)
    #define COMCP_FRAME_W_TRL_POS                (COMCP_FRAME_W_BCC_POS + 1)
    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_W_OFLEN                 5
    #define COMCP_RFRAME_W_OBCC_POS              COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_W_OTRL_POS              (COMCP_RFRAME_W_OBCC_POS + 1)


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //read
    #define COMCP_FRAME_R_FLEN                   4
    #define COMCP_FRAME_CMD_R                    0x17
    #define COMCP_FRAME_R_ADDR_POS               COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_R_ADDR_LEN               1
    #define COMCP_FRAME_R_BCC_POS                (COMCP_FRAME_R_ADDR_POS + COMCP_FRAME_R_ADDR_LEN)
    #define COMCP_FRAME_R_TRL_POS                (COMCP_FRAME_R_BCC_POS + 1)
    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_R_OFLEN                 22
    #define COMCP_RFRAME_R_OADDR_POS             COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_R_OADDR_LEN             1
    #define COMCP_RFRAME_R_ODATA_POS             (COMCP_RFRAME_R_OADDR_POS + COMCP_RFRAME_R_OADDR_LEN)
    #define COMCP_RFRAME_R_ODATA_LEN             16
    #define COMCP_RFRAME_R_OBCC_POS              (COMCP_RFRAME_R_ODATA_POS + COMCP_RFRAME_R_ODATA_LEN)
    #define COMCP_RFRAME_R_OTRL_POS              (COMCP_RFRAME_R_OBCC_POS + 1)


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //write value
    #define COMCP_FRAME_WV_FLEN                  8
    #define COMCP_FRAME_CMD_WV                   0x18
    #define COMCP_FRAME_WV_ADDR_POS              COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_WV_ADDR_LEN              1
    #define COMCP_FRAME_WV_VALUE_POS             (COMCP_FRAME_WV_ADDR_POS + COMCP_FRAME_WV_ADDR_LEN)
    #define COMCP_FRAME_WV_VALUE_LEN             4
    #define COMCP_FRAME_WV_BCC_POS               (COMCP_FRAME_WV_VALUE_POS + COMCP_FRAME_WV_VALUE_LEN)
    #define COMCP_FRAME_WV_TRL_POS               (COMCP_FRAME_WV_BCC_POS + 1)
    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_WV_OFLEN                5
    #define COMCP_RFRAME_WV_OBCC_POS             COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_WV_OTRL_POS             (COMCP_RFRAME_WV_OBCC_POS + 1)


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //value
    #define COMCP_FRAME_V_FLEN                   10
    #define COMCP_FRAME_CMD_V                    0x19
    #define COMCP_FRAME_V_MODE_POS               COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_V_MODE_LEN               1
    #define COMCP_FRAME_V_ADDR_POS               (COMCP_FRAME_V_MODE_POS + COMCP_FRAME_V_MODE_LEN)
    #define COMCP_FRAME_V_ADDR_LEN               1
    #define COMCP_FRAME_V_VALUE_POS              (COMCP_FRAME_V_ADDR_POS + COMCP_FRAME_V_ADDR_LEN)
    #define COMCP_FRAME_V_VALUE_LEN              4
    #define COMCP_FRAME_V_TADDR_POS              (COMCP_FRAME_V_VALUE_POS + COMCP_FRAME_V_VALUE_LEN)
    #define COMCP_FRAME_V_TADDR_LEN              1
    #define COMCP_FRAME_V_BCC_POS                (COMCP_FRAME_V_TADDR_POS + COMCP_FRAME_V_TADDR_LEN)
    #define COMCP_FRAME_V_TRL_POS                (COMCP_FRAME_V_BCC_POS + 1)
    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_V_OFLEN                 5
    #define COMCP_RFRAME_V_OBCC_POS              COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_V_OTRL_POS              (COMCP_RFRAME_V_OBCC_POS + 1)


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //read value
    #define COMCP_FRAME_RV_FLEN                  4
    #define COMCP_FRAME_CMD_RV                   0x1A
    #define COMCP_FRAME_RV_ADDR_POS              COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_RV_ADDR_LEN              1
    #define COMCP_FRAME_RV_BCC_POS               (COMCP_FRAME_RV_ADDR_POS + COMCP_FRAME_RV_ADDR_LEN)
    #define COMCP_FRAME_RV_TRL_POS               (COMCP_FRAME_RV_BCC_POS + 1)
    //��Ӧ֡:�ɹ�
    #define COMCP_RFRAME_RV_OFLEN                10
    #define COMCP_RFRAME_RV_OADDR_POS            COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_RV_OADDR_LEN            1
    #define COMCP_RFRAME_RV_OVALUE_POS           (COMCP_RFRAME_RV_OADDR_POS + COMCP_RFRAME_RV_OADDR_LEN)
    #define COMCP_RFRAME_RV_OVALUE_LEN           4
    #define COMCP_RFRAME_RV_OBCC_POS             (COMCP_RFRAME_RV_OVALUE_POS + COMCP_RFRAME_RV_OVALUE_LEN)
    #define COMCP_RFRAME_RV_OTRL_POS             (COMCP_RFRAME_RV_OBCC_POS + 1)



    //�����ṩ����֡����������û�����Ҫ��ϸ���˽�����֡�Ľṹ���Ϳ��Ի����ȷ������֡
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //����֡�����ʽ
/*
    u8 COMCP_ResetReaderFrame(u8 *pFrame);
    u8 COMCP_ControlAntFrame(u8 *pFrame, u8 antState);
    u8 COMCP_GetUIDFrame(u8 *pFrame, u8 rsqtMode);
    u8 COMCP_AutoGetUIDFrame(u8 *pFrame);
    u8 COMCP_HaltFrame(u8 *pFrame);
    u8 COMCP_AuthWFrame(u8 *pFrame, u8 keyType, u8 *pPwd, u8 addr, u8 *pData);
    u8 COMCP_AuthRFrame(u8 *pFrame, u8 keyType, u8 *pPwd, u8 addr);
    u8 COMCP_AuthWValueFrame(u8 *pFrame, u8 keyType, u8 *pPwd, u8 addr, u8 *pValue);
    u8 COMCP_AuthRValueFrame(u8 *pFrame, u8 keyType, u8 *pPwd, u8 addr);
    u8 COMCP_AuthValueFrame(u8 *pFrame, u8 keyType, u8 *pPwd, u8 valueMode, u8 addr, u8 *pValue, u8 transAddr);
    u8 COMCP_WFrame(u8 *pFrame, u8 addr, u8 *pData);
    u8 COMCP_RFrame(u8 *pFrame, u8 addr);
    u8 COMCP_WValueFrame(u8 *pFrame, u8 addr, u8 *pValue);
    u8 COMCP_RValueFrame(u8 *pFrame, u8 addr);
    u8 COMCP_ValueFrame(u8 *pFrame, u8 valueMode, u8 addr, u8 *pValue, u8 transAddr);
*/
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //��Ӧ֡�����ʽ(�û�����Ҫ�����������ο�)
    u8 COMCP_ResetReaderORspFrame(u8 *pFrame);
    u8 COMCP_ControlAntORspFrame(u8 *pFrame);
    u8 COMCP_GetUIDORspFrame(u8 *pFrame, u8 cardTypeH, u8 cardTypeL, u8 uidLen, u8 *pUID);
    u8 COMCP_AGetUIDORspFrame(u8 *pFrame, u8 cardTypeH, u8 cardTypeL, u8 uidLen, u8 *pUID);
    u8 COMCP_HaltORspFrame(u8 *pFrame);
    u8 COMCP_AuthWORspFrame(u8 *pFrame);
    u8 COMCP_AuthRORspFrame(u8 *pFrame, u8 addr, u8 *pData);
    u8 COMCP_AuthWVORspFrame(u8 *pFrame);
    u8 COMCP_AuthRVORspFrame(u8 *pFrame, u8 addr, u8 *pValue);
    u8 COMCP_AuthVORspFrame(u8 *pFrame);
    u8 COMCP_WORspFrame(u8 *pFrame);
    u8 COMCP_RORspFrame(u8 *pFrame, u8 addr, u8 *pData);
    u8 COMCP_WVORspFrame(u8 *pFrame);
    u8 COMCP_RVORspFrame(u8 *pFrame, u8 addr, u8 *pValue);
    u8 COMCP_VORspFrame(u8 *pFrame);

#elif (defined(_ANYID_STM32_BCTRL_) | defined(_ANYID_PIC18_BWCOM_))

    //����Bike�軹״̬��0x01��
    #define COMCP_FRAME_CMD_SBSTAT                   0x01
    #define COMCP_FRAME_SBSTAT_LEN                   4
    #define COMCP_FRAME_SBSTAT_STAT_POS              COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_SBSTAT_STAT_LEN              1
    #define COMCP_FRAME_SBSTAT_BCC_POS               (COMCP_FRAME_SBSTAT_STAT_POS + COMCP_FRAME_SBSTAT_STAT_LEN)
    #define COMCP_FRAME_SBSTAT_BCC_LEN               1
    #define COMCP_FRAME_SBSTAT_TRL_POS               (COMCP_FRAME_SBSTAT_BCC_POS + COMCP_FRAME_SBSTAT_BCC_LEN)
    //���/�黹״̬
    #define COMCP_FRAME_SBSTAT_LEND                  0x01
    #define COMCP_FRAME_SBSTAT_RETURN                0x00


    //��ȡ�ô������û���ʻ��¼��0x02��
    #define COMCP_FRAME_CMD_RBLOG                    0x02
    #define COMCP_FRAME_RBLOG_LEN                    3
    #define COMCP_FRAME_RBLOG_BCC_POS                (COMCP_FRAME_PAR_POS)
    #define COMCP_FRAME_RBLOG_BCC_LEN                1
    #define COMCP_FRAME_RBLOG_TRL_POS                (COMCP_FRAME_RBLOG_BCC_POS + COMCP_FRAME_RBLOG_BCC_LEN)
    //��ȡ�ô������û���ʻ��¼����Ӧ֡
    #define COMCP_RFRAME_RBLOG_OFLEN                 13
    #define COMCP_RFRAME_RBLOG_MILEAGE_POS           (COMCP_RFRAME_PAR_POS)
    #define COMCP_RFRAME_RBLOG_MILEAGE_LEN           4
    #define COMCP_RFRAME_RBLOG_CALORIE_POS           (COMCP_RFRAME_RBLOG_MILEAGE_POS + COMCP_RFRAME_RBLOG_MILEAGE_LEN)
    #define COMCP_RFRAME_RBLOG_CALORIE_LEN           4
    #define COMCP_RFRAME_RBLOG_OBCC_POS              (COMCP_RFRAME_RBLOG_CALORIE_POS + COMCP_RFRAME_RBLOG_CALORIE_LEN)
    #define COMCP_RFRAME_RBLOG_OTRL_POS              (COMCP_RFRAME_RBLOG_OBCC_POS + 1)

    //���³���0x03��
    #define COMCP_FRAME_CMD_UPDATE                   0x03
    #define COMCP_FRAME_UPDATE_LEN                   0x27
    #define COMCP_FRAME_UPDATE_INDEX_POS             COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_UPDATE_INDEX_LEN             4
    #define COMCP_FRAME_UPDATE_DATA_POS              (COMCP_FRAME_UPDATE_INDEX_POS + COMCP_FRAME_UPDATE_INDEX_LEN)
    #define COMCP_FRAME_UPDATE_DATA_LEN              32
    #define COMCP_FRAME_UPDATE_BCC_POS               (COMCP_FRAME_UPDATE_DATA_POS + COMCP_FRAME_UPDATE_DATA_LEN)
    #define COMCP_FRAME_UPDATE_BCC_LEN               1
    #define COMCP_FRAME_UPDATE_TRL_POS               (COMCP_FRAME_UPDATE_BCC_POS + COMCP_FRAME_UPDATE_BCC_LEN)
    //���³��������Ӧ֡
    #define COMCP_RFRAME_UPDATE_FFLEN                10
    #define COMCP_RFRAME_UPDATE_FINDEX_POS           (COMCP_RFRAME_FPAR_POS)
    #define COMCP_RFRAME_UPDATE_FINDEX_LEN           4
    #define COMCP_RFRAME_UPDATE_FBCC_POS             (COMCP_RFRAME_UPDATE_FINDEX_POS + COMCP_RFRAME_UPDATE_FINDEX_LEN)
    #define COMCP_RFRAME_UPDATE_FTRL_POS             (COMCP_RFRAME_UPDATE_FBCC_POS + 1)
    //ϵͳ������IDLE״̬�����ܸ���
    #define COMCP_FRAME_UPDATE_EBUSY                 0xFFFFFFFF //ϵͳæ


    //������³���0x04��
    #define COMCP_FRAME_CMD_RQTUPDATE                0x04
    #define COMCP_FRAME_RQTUPDATE_LEN                12
    #define COMCP_FRAME_RQTUPDATE_TYPE_POS           COMCP_FRAME_PAR_POS
    #define COMCP_FRAME_RQTUPDATE_TYPE_LEN           1
    #define COMCP_FRAME_RQTUPDATE_NUM_POS            (COMCP_FRAME_RQTUPDATE_TYPE_POS + COMCP_FRAME_RQTUPDATE_TYPE_LEN)
    #define COMCP_FRAME_RQTUPDATE_NUM_LEN            4
    #define COMCP_FRAME_RQTUPDATE_SIZE_POS           (COMCP_FRAME_RQTUPDATE_NUM_POS + COMCP_FRAME_RQTUPDATE_NUM_LEN)
    #define COMCP_FRAME_RQTUPDATE_SIZE_LEN           4
    #define COMCP_FRAME_RQTUPDATE_BCC_POS            (COMCP_FRAME_RQTUPDATE_SIZE_POS + COMCP_FRAME_RQTUPDATE_SIZE_LEN)
    #define COMCP_FRAME_RQTUPDATE_BCC_LEN            1
    #define COMCP_FRAME_RQTUPDATE_TRL_POS            (COMCP_FRAME_RQTUPDATE_BCC_POS + COMCP_FRAME_RQTUPDATE_BCC_LEN)
    //������³�����Ӧ֡
    #define COMCP_RFRAME_RQTUPDATE_OFLEN             6
    #define COMCP_RFRAME_RQTUPDATE_OSTATE_POS        COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_RQTUPDATE_OSTATE_LEN        1
    #define COMCP_RFRAME_RQTUPDATE_OBCC_POS          (COMCP_RFRAME_RQTUPDATE_OSTATE_POS + COMCP_RFRAME_RQTUPDATE_OSTATE_LEN)
    #define COMCP_RFRAME_RQTUPDATE_OTRL_POS          (COMCP_RFRAME_RQTUPDATE_OBCC_POS + 1)
    //������³���״̬
    #define COMCP_FRAME_RQTUPDATE_STATE_YES          0x00   //���Ա�����
    #define COMCP_FRAME_RQTUPDATE_STATE_ENALW        0x01   //��������δ�����µ�Ӧ�������������ٴθ���
    #define COMCP_FRAME_RQTUPDATE_STATE_ENFMT        0x02   //δ����ʽ��
    #define COMCP_FRAME_RQTUPDATE_STATE_EBUSY        0x03   //ϵͳæ
    #define COMCP_FRAME_RQTUPDATE_STATE_EPAR         0x04   //��������
    //�����������
    #define COMCP_FRAME_RQTUPDATE_TYPE_BP            0x01
    #define COMCP_FRAME_RQTUPDATE_TYPE_MP3P          0x02
    #define COMCP_FRAME_RQTUPDATE_TYPE_PROGRAME      0x03
    #define COMCP_FRAME_RQTUPDATE_TYPE_BPNAME        0x04
    #define COMCP_FRAME_RQTUPDATE_TYPE_MP3           0x05
    #define COMCP_FRAME_RQTUPDATE_TYPE_FORCE         0x00   //ǿ�Ƹ���
    


    //��ʽ����������0x05��
    #define COMCP_FRAME_CMD_FORMAT                   0x05
    #define COMCP_FRAME_FORMAT_LEN                   0x04
    #define COMCP_FRAME_FORMAT_NUM_POS               (COMCP_FRAME_PAR_POS)
    #define COMCP_FRAME_FORMAT_NUM_LEN               1
    #define COMCP_FRAME_FORMAT_BCC_POS               (COMCP_FRAME_FORMAT_NUM_POS + COMCP_FRAME_FORMAT_NUM_LEN)
    #define COMCP_FRAME_FORMAT_BCC_LEN               1
    #define COMCP_FRAME_FORMAT_TRL_POS               (COMCP_FRAME_FORMAT_BCC_POS + COMCP_FRAME_FORMAT_BCC_LEN)
    #define COMCP_FRAME_FORMAT_ALL                   0x00
    #define COMCP_FRAME_FORMAT_FORCE                 0xFF
    //#define COMCP_FRAME_FORMAT_QUERY                 0xFE   //��ѯ����״̬
    #define COMCP_FRAME_FORMAT_BLOCK_MAX             0x10   //������Ŀ
    //��ʽ����������Ӧ֡
    #define COMCP_RFRAME_FORMAT_OFLEN                6
    #define COMCP_RFRAME_FORMAT_OSTATE_POS           COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_FORMAT_OSTATE_LEN           1
    #define COMCP_RFRAME_FORMAT_OBCC_POS             (COMCP_RFRAME_FORMAT_OSTATE_POS + COMCP_RFRAME_FORMAT_OSTATE_LEN)
    #define COMCP_RFRAME_FORMAT_OTRL_POS             (COMCP_RFRAME_FORMAT_OBCC_POS + 1)
    //��ʽ��״̬
    #define COMCP_FRAME_FORMAT_STATE_OVER            0xFF   //��ʽ�����
    #define COMCP_FRAME_FORMAT_STATE_START           0xFE   //��ʼ��ʽ��
    #define COMCP_FRAME_FORMAT_STATE_ENUM            0xFD   //����Ŀ����
    #define COMCP_FRAME_FORMAT_STATE_ENALW           0xFC   //���������и����ļ����������
    #define COMCP_FRAME_FORMAT_STATE_EBUSY           0xFB   //ϵͳæ
    

    /*
    //��ȡ��������Ȩ״̬��0x05��
    #define COMCP_FRAME_CMD_GETLIC                   0x05
    #define COMCP_FRAME_GETLIC_LEN                   0x03
    #define COMCP_FRAME_GETLIC_BCC_POS               (COMCP_FRAME_PAR_POS)
    #define COMCP_FRAME_GETLIC_BCC_LEN               1
    #define COMCP_FRAME_GETLIC_TRL_POS               (COMCP_FRAME_GETLIC_BCC_POS + COMCP_FRAME_GETLIC_BCC_LEN)
    //��ȡ��������Ȩ״̬��Ӧ֡
    #define COMCP_RFRAME_GETLIC_OFLEN                6
    #define COMCP_RFRAME_GETLIC_OSTATE_POS           COMCP_RFRAME_PAR_POS
    #define COMCP_RFRAME_GETLIC_OSTATE_LEN           1
    #define COMCP_RFRAME_GETLIC_OBCC_POS             (COMCP_RFRAME_GETLIC_OSTATE_POS + COMCP_RFRAME_GETLIC_OSTATE_LEN)
    #define COMCP_RFRAME_GETLIC_OTRL_POS             (COMCP_RFRAME_GETLIC_OBCC_POS + 1)
    //��Ȩ״̬
    #define COMCP_FRAME_LIC_STATE_NO                 0x00
    #define COMCP_FRAME_LIC_STATE_YES                0x01
    */

    //��ȡBike״̬��0x06��
    #define COMCP_FRAME_CMD_GETSTAT                  0x06
    #define COMCP_FRAME_GETSTAT_LEN                  0x03
    #define COMCP_FRAME_GETSTAT_BCC_POS              (COMCP_FRAME_PAR_POS)
    #define COMCP_FRAME_GETSTAT_BCC_LEN              1
    #define COMCP_FRAME_GETSTAT_TRL_POS              (COMCP_FRAME_GETSTAT_BCC_POS + COMCP_FRAME_GETSTAT_BCC_LEN)
    //��ȡBike״̬��Ӧ֡
    #define COMCP_RFRAME_GETSTAT_OFLEN               6
    #define COMCP_RFRAME_GETSTAT_OLNDSTAT_POS        (COMCP_RFRAME_PAR_POS)
    #define COMCP_RFRAME_GETSTAT_OLNDSTAT_LEN        1
    #define COMCP_RFRAME_GETSTAT_OBCC_POS            (COMCP_RFRAME_GETSTAT_OLNDSTAT_POS + COMCP_RFRAME_GETSTAT_OLNDSTAT_LEN)
    #define COMCP_RFRAME_GETSTAT_OTRL_POS            (COMCP_RFRAME_GETSTAT_OBCC_POS + 1)



    //��ȡ�û���Ϣ��0x07��
    #define COMCP_FRAME_CMD_GETUSER                  0x07
    #define COMCP_FRAME_GETUSER_LEN                  0x03
    #define COMCP_FRAME_GETUSER_BCC_POS              (COMCP_FRAME_PAR_POS)
    #define COMCP_FRAME_GETUSER_BCC_LEN              1
    #define COMCP_FRAME_GETUSER_TRL_POS              (COMCP_FRAME_GETUSER_BCC_POS + COMCP_FRAME_GETUSER_BCC_LEN)
    //��ȡ�û���Ϣ��Ӧ֡
    #define COMCP_RFRAME_GETUSER_OFLEN               16
    #define COMCP_RFRAME_GETUSER_OMILEAGE_POS        (COMCP_RFRAME_PAR_POS)
    #define COMCP_RFRAME_GETUSER_OMILEAGE_LEN        4
    #define COMCP_RFRAME_GETUSER_OTIME_POS           (COMCP_RFRAME_GETUSER_OMILEAGE_POS + COMCP_RFRAME_GETUSER_OMILEAGE_LEN)
    #define COMCP_RFRAME_GETUSER_OTIME_LEN           3
    #define COMCP_RFRAME_GETUSER_OID_POS             (COMCP_RFRAME_GETUSER_OTIME_POS + COMCP_RFRAME_GETUSER_OTIME_LEN)
    #define COMCP_RFRAME_GETUSER_OID_LEN             4
    #define COMCP_RFRAME_GETUSER_OBCC_POS            (COMCP_RFRAME_GETUSER_OID_POS + COMCP_RFRAME_GETUSER_OID_LEN)
    #define COMCP_RFRAME_GETUSER_OTRL_POS            (COMCP_RFRAME_GETUSER_OBCC_POS + 1)

    //�����û���Ϣ��0x08��
    #define COMCP_FRAME_CMD_SETUSER                  0x08
    #define COMCP_FRAME_SETUSER_LEN                  10
    #define COMCP_FRAME_SETUSER_TIME_POS             (COMCP_FRAME_PAR_POS)
    #define COMCP_FRAME_SETUSER_TIME_LEN             3
    #define COMCP_FRAME_SETUSER_ID_POS               (COMCP_FRAME_SETUSER_TIME_POS + COMCP_FRAME_SETUSER_TIME_LEN)
    #define COMCP_FRAME_SETUSER_ID_LEN               4
    #define COMCP_FRAME_SETUSER_BCC_POS              (COMCP_FRAME_SETUSER_ID_POS + COMCP_FRAME_SETUSER_ID_LEN)
    #define COMCP_FRAME_SETUSER_BCC_LEN              1
    #define COMCP_FRAME_SETUSER_TRL_POS              (COMCP_FRAME_SETUSER_BCC_POS + COMCP_FRAME_SETUSER_BCC_LEN)

    //�����û���Ϣ��0x09��
    #define COMCP_FRAME_CMD_TICK                     0x09
    //Tick��Ӧ֡
    #define COMCP_RFRAME_TICK_OFLEN                  17
    #define COMCP_RFRAME_TICK_OBIKEID_POS            (COMCP_RFRAME_PAR_POS)
    #define COMCP_RFRAME_TICK_OBIKEID_LEN            12
    #define COMCP_RFRAME_TICK_OBCC_POS               (COMCP_RFRAME_TICK_OBIKEID_POS + COMCP_RFRAME_TICK_OBIKEID_LEN)
    #define COMCP_RFRAME_TICK_OTRL_POS               (COMCP_RFRAME_TICK_OBCC_POS + 1)


    //��ȡFLASH�����ݣ�0x0A���ڲ��������������й
    #define COMCP_FRAME_CMD_RFLASH                   0x0A
    #define COMCP_FRAME_RFLASH_LEN                   0x08
    #define COMCP_FRAME_RFLASH_ADDR_POS              (COMCP_FRAME_PAR_POS)
    #define COMCP_FRAME_RFLASH_ADDR_LEN              4
    #define COMCP_FRAME_RFLASH_SIZE_POS              (COMCP_FRAME_RFLASH_ADDR_POS + COMCP_FRAME_RFLASH_ADDR_LEN)
    #define COMCP_FRAME_RFLASH_SIZE_LEN              1
    #define COMCP_FRAME_RFLASH_BCC_POS               (COMCP_FRAME_RFLASH_SIZE_POS + COMCP_FRAME_RFLASH_SIZE_LEN)
    #define COMCP_FRAME_RFLASH_BCC_LEN               1
    #define COMCP_FRAME_RFLASH_TRL_POS               (COMCP_FRAME_RFLASH_BCC_POS + COMCP_FRAME_RFLASH_BCC_LEN)
    //ÿ�ζ�ȡ�������
    #define COMCP_FRAME_RFLASH_MAX_SIZE              32
    //��ȡFLASH��������Ӧ֡
    #define COMCP_RFRAME_RFLASH_OFLEN               5
    #define COMCP_RFRAME_RFLASH_ODATA_POS           (COMCP_RFRAME_PAR_POS)


    //ֹͣ���£�0x0B��
    #define COMCP_FRAME_CMD_STOPUPDATE              0x0B



    //дFLASH�����ݣ�0x0C���ڲ��������������й
    #define COMCP_FRAME_CMD_WFLASH                   0x0C
    #define COMCP_FRAME_WFLASH_LEN                   0x08
    #define COMCP_FRAME_WFLASH_ADDR_POS              (COMCP_FRAME_PAR_POS)
    #define COMCP_FRAME_WFLASH_ADDR_LEN              4
    #define COMCP_FRAME_WFLASH_SIZE_POS              (COMCP_FRAME_WFLASH_ADDR_POS + COMCP_FRAME_WFLASH_ADDR_LEN)
    #define COMCP_FRAME_WFLASH_SIZE_LEN              1
    #define COMCP_FRAME_WFLASH_DATA_POS              (COMCP_FRAME_WFLASH_SIZE_POS + COMCP_FRAME_WFLASH_SIZE_LEN)
    //ÿ��д��������
    #define COMCP_FRAME_WFLASH_MAX_SIZE              32

    //����FLASH�����ݣ�0x0D���ڲ��������������й
    #define COMCP_FRAME_CMD_EFLASH                   0x0D
    #define COMCP_FRAME_EFLASH_LEN                   0x07
    #define COMCP_FRAME_EFLASH_ADDR_POS              (COMCP_FRAME_PAR_POS)
    #define COMCP_FRAME_EFLASH_ADDR_LEN              4
    #define COMCP_FRAME_EFLASH_BCC_POS               (COMCP_FRAME_EFLASH_ADDR_POS + COMCP_FRAME_EFLASH_ADDR_LEN)
    #define COMCP_FRAME_EFLASH_BCC_LEN               1
    #define COMCP_FRAME_EFLASH_TRL_POS               (COMCP_FRAME_EFLASH_BCC_POS + COMCP_FRAME_EFLASH_BCC_LEN)



    //��ȡEEPROM�����ݣ�0x0E���ڲ��������������й
    #define COMCP_FRAME_CMD_REEPROM                  0x0E
    #define COMCP_FRAME_REEPROM_LEN                  0x08
    #define COMCP_FRAME_REEPROM_ADDR_POS             (COMCP_FRAME_PAR_POS)
    #define COMCP_FRAME_REEPROM_ADDR_LEN             4
    #define COMCP_FRAME_REEPROM_SIZE_POS             (COMCP_FRAME_REEPROM_ADDR_POS + COMCP_FRAME_REEPROM_ADDR_LEN)
    #define COMCP_FRAME_REEPROM_SIZE_LEN             1
    #define COMCP_FRAME_REEPROM_BCC_POS              (COMCP_FRAME_REEPROM_SIZE_POS + COMCP_FRAME_REEPROM_SIZE_LEN)
    #define COMCP_FRAME_REEPROM_BCC_LEN              1
    #define COMCP_FRAME_REEPROM_TRL_POS              (COMCP_FRAME_REEPROM_BCC_POS + COMCP_FRAME_REEPROM_BCC_LEN)
    //ÿ�ζ�ȡ�������
    #define COMCP_FRAME_REEPROM_MAX_SIZE              32
    //��ȡEEPROM��������Ӧ֡
    #define COMCP_RFRAME_REEPROM_OFLEN               5
    #define COMCP_RFRAME_REEPROM_ODATA_POS           (COMCP_RFRAME_PAR_POS)


    //дEEPROM�����ݣ�0x0F���ڲ��������������й
    #define COMCP_FRAME_CMD_WEEPROM                   0x0F
    #define COMCP_FRAME_WEEPROM_LEN                   0x08
    #define COMCP_FRAME_WEEPROM_ADDR_POS              (COMCP_FRAME_PAR_POS)
    #define COMCP_FRAME_WEEPROM_ADDR_LEN              4
    #define COMCP_FRAME_WEEPROM_SIZE_POS              (COMCP_FRAME_WEEPROM_ADDR_POS + COMCP_FRAME_WEEPROM_ADDR_LEN)
    #define COMCP_FRAME_WEEPROM_SIZE_LEN              1
    #define COMCP_FRAME_WEEPROM_DATA_POS              (COMCP_FRAME_WEEPROM_SIZE_POS + COMCP_FRAME_WEEPROM_SIZE_LEN)
    //ÿ��д��������
    #define COMCP_FRAME_WEEPROM_MAX_SIZE              32

    //��ȡBikeID��0x10���ڲ��������������й
    #define COMCP_FRAME_CMD_RBIKEID                   0x10
    //��ȡBikeID����Ӧ֡
    #define COMCP_RFRAME_RBIKEID_OFLEN                17
    #define COMCP_RFRAME_RBIKEID_OBIKEID_POS          (COMCP_RFRAME_PAR_POS)
    #define COMCP_RFRAME_RBIKEID_OBIKEID_LEN          12
    #define COMCP_RFRAME_RBIKEID_OBCC_POS             (COMCP_RFRAME_RBIKEID_OBIKEID_POS + COMCP_RFRAME_RBIKEID_OBIKEID_LEN)
    #define COMCP_RFRAME_RBIKEID_OTRL_POS             (COMCP_RFRAME_RBIKEID_OBCC_POS + 1)
    

//#if defined(_ANYID_STM32_BCTRL_)
    u8 COMCP_SetBikeStateFrame(u8 *pFrame, u8 state);
    #define COMCP_SetBikeStateRspFrame(pFrame) COMCP_OkRspNoParFrame(pFrame, COMCP_FRAME_CMD_SBSTAT)

    
    #define COMCP_ReadBikeLogFrame(pFrame)      COMCP_NoParRequestFrame(pFrame, COMCP_FRAME_CMD_RBLOG)
    u8 COMCP_ReadBikeLogRspFrame(u8 *pFrame, u16 mileage, u16 calorie);


    u8 COMCP_UpdateFrame(u8 *pFrame, u32 index, u8 *pPrograme);
    u8 COMCP_UpdateFRspFrame(u8 *pFrame, u8 errorCode, u32 index);
    #define COMCP_UpdateRspFrame(pFrame) COMCP_OkRspNoParFrame(pFrame, COMCP_FRAME_CMD_UPDATE)


    
    u8 COMCP_RequestUpdateFrame(u8 *pFrame, u8 type, u32 version, u32 size);
    u8 COMCP_RequestUpdateRspFrame(u8 *pFrame, u8 state);

    
    #define COMCP_GetBikeStateFrame(pFrame) COMCP_NoParRequestFrame(pFrame, COMCP_FRAME_CMD_GETSTAT)
    u8 COMCP_GetBikeStateRspFrame(u8 *pFrame, u8 lendState);

    
    #define COMCP_StopUpdateFrame(pFrame)    COMCP_NoParRequestFrame(pFrame, COMCP_FRAME_CMD_STOPUPDATE)
    #define COMCP_StopUpdateRspFrame(pFrame) COMCP_OkRspNoParFrame(pFrame, COMCP_FRAME_CMD_STOPUPDATE)



    #define COMCP_GetUserInfFrame(pFrame)       COMCP_NoParRequestFrame(pFrame, COMCP_FRAME_CMD_GETUSER)
    u8 COMCP_GetUserInfRspFrame(u8 *pFrame, u32 mileage, u32 time, u8 *pUserID);


    u8 COMCP_SetUserInfFrame(u8 *pFrame, u32 time, u8 *pUserID);
    #define COMCP_SetUserInfRspFrame(pFrame)    COMCP_OkRspNoParFrame(pFrame, COMCP_FRAME_CMD_SETUSER)


    #define COMCP_TickFrame(pFrame)       COMCP_NoParRequestFrame(pFrame, COMCP_FRAME_CMD_TICK)
    u8 COMCP_TickRspFrame(u8 *pFrame, u8 *pBikeID);

    u8 COMCP_ReadFlashFrame(u8 *pFrame, u32 address, u8 size);
    u8 COMCP_ReadFlashRspFrame(u8 *pFrame, u8 *pData, u8 size);

    u8 COMCP_WriteFlashFrame(u8 *pFrame, u32 address, u8 size, u8 *pData);
    #define COMCP_WriteFlashRspFrame(pFrame) COMCP_OkRspNoParFrame(pFrame, COMCP_FRAME_CMD_WFLASH)  


    u8 COMCP_EraseFlashFrame(u8 *pFrame, u32 address);
    #define COMCP_EraseFlashRspFrame(pFrame) COMCP_OkRspNoParFrame(pFrame, COMCP_FRAME_CMD_EFLASH)


    u8 COMCP_ReadEEPROMFrame(u8 *pFrame, u32 address, u8 size);
    u8 COMCP_ReadEEPROMRspFrame(u8 *pFrame, u8 *pData, u8 size);

    u8 COMCP_WriteEEPROMFrame(u8 *pFrame, u32 address, u8 size, u8 *pData);
    #define COMCP_WriteEEPROMRspFrame(pFrame) COMCP_OkRspNoParFrame(pFrame, COMCP_FRAME_CMD_WEEPROM) 

    #define COMCP_ReadBikeIDFrame(pFrame)    COMCP_NoParRequestFrame(pFrame, COMCP_FRAME_CMD_RBIKEID)
    u8 COMCP_ReadBikeIDRspFrame(u8 *pFrame, u8 *pBikeID);


    u8 COMCP_FormatUpdateBlockFrame(u8 *pFrame, u8 blockNum);
    u8 COMCP_FormatUpdateBlockRspFrame(u8 *pFrame, u8 formatState);
//#endif

//#if defined(_ANYID_PIC18_BWCOM_)
    
    
    
    
    

    

    
    


    
//#endif



#endif



#endif
