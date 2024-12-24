#ifndef _ANYID_RFCP_
#define _ANYID_RFCP_

#include "AnyID_Config.h"

#define RFCP_RX_BUF_LEN              32
#define RFCP_TX_BUF_LEN              32

#define RFCP_FRAME_LEN_POS           0   //֡����λ��
#define RFCP_FRAME_ADDR_POS          1   //֡��ַ
#define RFCP_FRAME_CMD_POS           2   //֡����
#define RFCP_FRAME_RSP_POS           3
#define RFCP_FRAME_CTRL_POS          3
#define RFCP_FRAME_PAR_POS           4

#define RFCP_FRAME_DFT_ADDR          0x00    //Ĭ�ϵ�ַ

#define RFCP_FRAME_RSP_FLAG          0x80    //��Ӧ֡��ʶ

#define RFCP_FRAME_RQST_RSP          0x01    //������ջ��Ը�֡������Ӧ
#define RFCP_FRAME_NO_RSP            0x00    //���ջ�����Ҫ�Ը�֡������Ӧ

//CRCУ��ֵ
#define RFCP_FRAME_CRC_MASK          0x80
#define RFCP_FRAME_CRC_LEN           2


u8 RFCP_RequestFrame(u8 *pFrame, u8 filterAddr, u8 cmd, u8 reqFlag, u8 *pPar, u8 parLen);
u8 RFCP_ReponseFrame(u8 *pFrame, u8 filterAddr, u8 cmd, u8 *pPar, u8 parLen);

#if (defined(_ANYID_STM32_MTD_) | defined(_ANYID_STM32_MID_) | defined(_ANYID_STM32_MPD_) | defined(_ANYID_STM32_MMD_) | defined(_ANYID_STM32_MCD_))

    //����
    #define RFCP_FRAME_CMD_SIGHTID       0x01    //�����ʶ
    #define RFCP_FRAME_CMD_ALARM         0x02    //�������ʶ
    #define RFCP_FRAME_CMD_OFF           0x03    //�ػ�
    #define RFCP_FRAME_CMD_LIC           0x04    //������Ȩ
    #define RFCP_FRAME_CMD_RQTLIC        0x05    //������Ȩ
    #define RFCP_FRAME_CMD_TESTLED       0x06    //������ʾ��LED
    #define RFCP_FRAME_CMD_GET_VISION    0x07    //��ȡ����汾��
    #define RFCP_FRAME_CMD_GET_CPUID     0x08    //��ȡCPUID



    //֡����
    #define RFCP_FRAME_LEN_SIGHTID       0x08    //�����ʶ
    #define RFCP_FRAME_LEN_ALARM         0x03    //�������ʶ
    #define RFCP_FRAME_LEN_OFF           0x03    //�ػ�

    #define RFCP_FRAME_LEN_LIC           0x04    //������Ȩ
    #define RFCP_RFRAME_LEN_LIC          0x11    //������Ȩ

    #define RFCP_FRAME_LEN_RQTLIC        0x13    //������Ȩ
    #define RFCP_RFRAME_LEN_RQTLIC       0x13    //������Ȩ

    #define RFCP_FRAME_LEN_TESTLED       0x03    //������ʾ��LED

    #define RFCP_FRAME_LEN_GET_VISION    0x03    //��ȡ����汾��
    #define RFCP_RFRAME_LEN_GET_VISION   0x07    //��ȡ����汾��

    #define RFCP_FRAME_LEN_GET_CPUID     0x03    //��ȡCPUID
    #define RFCP_RFRAME_LEN_GET_CPUID    0x0F    //��ȡCPUID



    //�����ʶ֡��ʽ
    #define RFCP_FRAME_SIGHTID_ID_POS    RFCP_FRAME_PAR_POS    	//IDλ��
    #define RFCP_FRAME_SIGHTID_ID_LEN    0x02    					//ID����
    #define RFCP_FRAME_SIGHTID_VOL_POS   (RFCP_FRAME_SIGHTID_ID_POS + RFCP_FRAME_SIGHTID_ID_LEN)
    #define RFCP_FRAME_SIGHTID_VOL_LEN   0x02
    #define RFCP_FRAME_SIGHTID_POW_POS   (RFCP_FRAME_SIGHTID_VOL_POS + RFCP_FRAME_SIGHTID_VOL_LEN)
    #define RFCP_FRAME_SIGHTID_POW_LEN   0x01


    //��Ȩ֡��ʽ
    #define RFCP_FRAME_LIC_LG_POS        RFCP_FRAME_PAR_POS
    #define RFCP_FRAME_LIC_LG_LEN        1
    #define RFCP_FRAME_LIC_LG_MAX        0x06
    #define RFCP_FRAME_LIC_LG_DFT        0
    //��Ȩ֡��Ӧ֡��ʽ
    #define RFCP_RFRAME_LIC_CPUID_POS    RFCP_FRAME_PAR_POS
    #define RFCP_RFRAME_LIC_CPUID_LEN    12
    #define RFCP_RFRAME_LIC_MTDVOL_POS   (RFCP_RFRAME_LIC_CPUID_POS + RFCP_RFRAME_LIC_CPUID_LEN)
    #define RFCP_RFRAME_LIC_MTDVOL_LEN   2
    #define RFCP_RFRAME_LIC_MTDVOL_MASK  0x1FFF


    //������Ȩ֡��ʽ
    #define RFCP_FRAME_RQTLIC_CPUID_POS  RFCP_FRAME_PAR_POS
    #define RFCP_FRAME_RQTLIC_CPUID_LEN  12
    #define RFCP_FRAME_RQTLIC_LICU_POS   (RFCP_FRAME_RQTLIC_CPUID_POS + RFCP_FRAME_RQTLIC_CPUID_LEN)
    #define RFCP_FRAME_RQTLIC_LICU_LEN   2
    #define RFCP_FRAME_RQTLIC_PWD_POS    (RFCP_FRAME_RQTLIC_LICU_POS + RFCP_FRAME_RQTLIC_LICU_LEN)
    #define RFCP_FRAME_RQTLIC_PWD_LEN    2
    //������Ȩ֡��Ӧ֡��ʽ
    #define RFCP_RFRAME_RQTLIC_CPUID_POS RFCP_FRAME_PAR_POS
    #define RFCP_RFRAME_RQTLIC_CPUID_LEN 12
    #define RFCP_RFRAME_RQTLIC_LIC_POS   (RFCP_RFRAME_RQTLIC_CPUID_POS + RFCP_RFRAME_RQTLIC_CPUID_LEN)
    #define RFCP_RFRAME_RQTLIC_LIC_LEN   2
    #define RFCP_RFRAME_RQTLIC_PWD_POS   (RFCP_RFRAME_RQTLIC_LIC_POS + RFCP_RFRAME_RQTLIC_LIC_LEN)
    #define RFCP_RFRAME_RQTLIC_PWD_LEN   2


    //��ȡ�汾��֡��Ӧ֡��ʽ
    #define RFCP_RFRAME_VISION_TYPE_POS  RFCP_FRAME_PAR_POS
    #define RFCP_RFRAME_VISION_TYPE_LEN  1
    #define RFCP_RFRAME_VISION_VXX_POS   (RFCP_RFRAME_VISION_TYPE_POS + RFCP_RFRAME_VISION_TYPE_LEN)
    #define RFCP_RFRAME_VISION_VXX_LEN   1
    #define RFCP_RFRAME_VISION_VYY_POS   (RFCP_RFRAME_VISION_VXX_POS + RFCP_RFRAME_VISION_VXX_LEN)
    #define RFCP_RFRAME_VISION_VYY_LEN   1
    #define RFCP_RFRAME_VISION_VZZ_POS   (RFCP_RFRAME_VISION_VYY_POS + RFCP_RFRAME_VISION_VYY_LEN)
    #define RFCP_RFRAME_VISION_VZZ_LEN   1

    //��ȡCPUID֡��Ӧ֡��ʽ
    #define RFCP_RFRAME_GETCPUID_POS     RFCP_FRAME_PAR_POS
    #define RFCP_RFRAME_GETCPUID_LEN     12





    u8 RFCP_SightIDFrame(u8 *pFrame, u16 sightID, u16 voltage, u8 power);

    u8 RFCP_AlarmFrame(u8 *pFrame);

    u8 RFCP_OFFFrame(u8 *pFrame);

    u8 RFCP_LicenceFrame(u8 *pFrame, u8 lg, u8 isRsq);
    u8 RFCP_LicenceRspFrame(u8 *pFrame, u8 *pCPUID, u16 voltage);

    u8 RFCP_RequestLicenceFrame(u8 *pFrame, u8 *pCPUID, u16 licence, u16 pwd);
    u8 RFCP_RequestLicenceRspFrame(u8 *pFrame, u8 *pCPUID, u16 licence, u16 pwd);

    u8 RFCP_TestLEDFrame(u8 *pFrame);

    u8 RFCP_GetVisionFrame(u8 *pFrame);
    u8 RFCP_VisionRsqFrame(u8 *pFrame, u8 visionType, u8 xx, u8 yy, u8 zz);

    u8 RFCP_GetCPUIDFrame(u8 *pFrame);
    u8 RFCP_CPUIDRsqFrame(u8 *pFrame, u8 *pCPUUID);
    BOOL C(u8 *pFrame, u8 len);

/*#elif (defined(_ANYID_PIC18_BWCOM_))

BOOL RFCP_CheckRxFrame(u8 *pFrame, u8 len);

u8 RFCP_MasteFrame(u8 *pFrame, u8 filterAddr, u8 cmd, u8 reqFlag, u8 *pPar, u8 parLen, u32 index);
u8 RFCP_SlaveFrame(u8 *pFrame, u8 filterAddr, u8 cmd, u8 *pPar, u8 parLen, u32 index);
u32 RFCP_GetFrameIndex(u8 *pFrame, u8 len);
*/
#endif



#endif
