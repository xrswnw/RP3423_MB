#ifndef _ANYID_ISO14443A_
#define _ANYID_ISO14443A_

#define ISO14443A_PICC_ACTIVATE_EN        	1       // ��������߸�Ӧ���Ŀ�Ƭ
#define ISO14443A_PCD_RFRESET_EN          	1       // RF����λ(�ر��ز�)


//Mifare ����
#define ISO14443A_CMD_IDLE             0x00   //idle command
#define ISO14443A_CMD_AUTHENT_A        0x60   //AUTHENT A command
#define ISO14443A_CMD_AUTHENT_B        0x61   //AUTHENT B command
#define ISO14443A_CMD_READ             0x30   //READ command
#define ISO14443A_CMD_WRITE16          0xA0   //WRITE 16 bytes command
#define ISO14443A_CMD_WRITE4           0xA2   //WRITE 4 bytes command
#define ISO14443A_CMD_INCREMENT        0xC1   //INCREMENT command
#define ISO14443A_CMD_DECREMENT        0xC0   //DECREMENT command
#define ISO14443A_CMD_RESTORE          0xC2   //RESTORE command
#define ISO14443A_CMD_TRANSFER         0xB0   //TRANSFER command
#define ISO14443A_CMD_NOCOMMAND        0x00   //VOID command (no MIFARE command)
#define ISO14443A_CMD_HALTA            0x50   //��������
#define ISO14443A_CMD_REQALL           0x52    // ��������
#define ISO14443A_CMD_REQIDL           0x26    // ��������
#define ISO14443A_CMD_ANTICOLL_L1      0x93    // ����ײ/ѡ��ȼ�1
#define ISO14443A_CMD_ANTICOLL_L2      0x95    // ����ײ/ѡ��ȼ�2
#define ISO14443A_CMD_ANTICOLL_L3      0x97    // ����ײ/ѡ��ȼ�3


#define ISO14443A_EXPECT_TIMEOUT       0x01   //Tells the library to expect a timeout
#define ISO14443A_EXPECT_ACK           0x02   //Let the library expect an Acknowledge response
#define ISO14443A_EXPECT_DATA          0x04   //The library shall expect data  

#define ISO14443A_REQUEST_BITS         0x07    // ���������λ��


//����ײ/ѡ�������������
#define ISO14443A_BITS_PER_BYTE        0x08    //һ���ֽ�8λ
#define ISO14443A_UPPER_NVB_SHIFT      0x04    //NVB��λ�ƶ�λ������4λ��ʾ��Ч�ֽ�������4λ��ʾ���һ���ֽ�(����������Ч�ֽ���)��Чλ��
#define ISO14443A_COMPLETE_UID_BITS    0x28
#define ISO14443A_NVB_MIN_PARAMETER    0x20    //NVB��Сֵ��ֻ��CMD��NVB�����ֽ�
#define ISO14443A_NVB_MAX_PARAMETER    0x70    //NVB���ֵ��CMD NVB UID(4B) BCC

//
#define ISO14443A_MAX_ANTICOLL_LEVELS  0x03    // ������ײ�ȼ�
#define ISO14443A_SINGLE_UID_LENGTH    0x20     //uid��4�ֽڣ�32λ
#define ISO14443A_COLL_BIT             0x04

//ѡ�������Ӧ���ֽ���
#define ISO14443A_SAK_LENGTH           0x01

//���������������
#define ISO14443A_HALTA_PARAM          0x00

//cascade tag
#define ISO14443A_CT                   0x88

//������
#define ISO14443A_TYPE_MF0_U1          0x0044
#define ISO14443A_TYPE_MF1_S50         0x0004
#define ISO14443A_TYPE_MF1_S70         0x0002
#define ISO14443A_TYPE_MF3_D40         0x0344
#define ISO14443A_TYPE_PROX            0x0008

#define ISO14443A_MF0_U1_ADDR_MAX      0x0F        //512bits, 16Block, 4Bytes pre block
#define ISO14443A_MF1_S50_ADDR_MAX     0x3F        //8Kbits,   64Block, 16Bytes pre block


#define ISO14443A_UID_MAX_LEN          10
#define ISO14443A_UID_L1_LEN           4
#define ISO14443A_UID_L2_LEN           7
#define ISO14443A_UID_L3_LEN           10


//֡��Ϣ
//֡���ȣ�����ȫ���Ǳ�׼֡�������CRC-A�ģ�֡���Ȳ�����CRC�ĳ���
//����֡��ʽ
#define ISO14443A_REQUEST_LEN          1   //����֡����
#define ISO14443A_ATQA_LEN             2   //����֡��Ӧ����
#define ISO14443A_REQUEST_CMD_POS      0



//���յ���ATL(����ײ)����֡��ʽ
#define ISO14443A_ATL_FRM_FPCD_LEN     2   //������֮PCD�ķ���ײ֡
#define ISO14443A_ATL_FPCD_CMD_POS     0
#define ISO14443A_ATL_FPCD_NVB_POS     1
//��ӦPCD��ATL(����ײ)����֡��ʽ
#define ISO14443A_ATL_FRM_TPCD_LEN     5   //��ӦPCD�ķ���ײ֡
#define ISO14443A_ATL_TPCD_CT_POS      0
#define ISO14443A_ATL_TPCD_BCC_POS     4

#define ISO14443A_ATL_TPCD_CT_LEN      1
#define ISO14443A_ATL_TPCD_UID_LEN     4



//��֮PCD��SEL����֡��ʽ
#define ISO14443A_SEL_FRM_FPCD_LEN     7   //������֮PCD��ѡ��֡
#define ISO14443A_SEL_FPCD_CMD_POS     0
#define ISO14443A_SEL_FPCD_NVB_POS     1
#define ISO14443A_SEL_FPCD_UID_POS     2
#define ISO14443A_SEL_FPCD_BCC_POS     6
#define ISO14443A_SEL_FPCD_UID_LEN     4
#define ISO14443A_SAK_FRM_TPCD_LEN     1   //��ӦPCD��SEL����


//AUTH����֡��ʽ
#define ISO14443A_AUTH_FRM_FPCD_LEN    12
#define ISO14443A_AUTH_FPCD_CMD_POS    0
#define ISO14443A_AUTH_FPCD_ADDR_POS   1
#define ISO14443A_AUTH_FPCD_PWD_POS    2
#define ISO14443A_AUTH_FPCD_UID_POS    8
#define ISO14443A_AUTH_FPCD_PWD_LEN    6
#define ISO14443A_AUTH_FPCD_UID_LEN    4


//��д�����Ӧ��֡��ʽ
#define ISO14443A_ACK_MASK             0x0F
#define ISO14443A_ACK_OK               0x0A
#define ISO14443A_ACK_NOT_ALLOW        0x04
#define ISO14443A_ACK_TRS_ERROR        0x05


//Read����֡��ʽ
#define ISO14443A_READ_FRM_FPCD_LEN    2
#define ISO14443A_READ_FPCD_CMD_POS    0
#define ISO14443A_READ_FPCD_ADDR_POS   1
#define ISO14443A_READ_FRM_TPCD_LEN    16
#define ISO14443A_READ_TPCD_DATA_POS   0

//Write4����֡��ʽ
#define ISO14443A_WRITE4_FRM_FPCD_LEN  6
#define ISO14443A_WRITE4_FPCD_CMD_POS  0
#define ISO14443A_WRITE4_FPCD_ADDR_POS 1
#define ISO14443A_WRITE4_FPCD_DATA_POS 2
#define ISO14443A_WRITE4_FPCD_DATA_LEN 4

//Write16����֡��ʽ
#define ISO14443A_WRITE16_S1FRM_FPCD_LEN  2
#define ISO14443A_WRITE16_S1FPCD_CMD_POS  0
#define ISO14443A_WRITE16_S1FPCD_ADDR_POS 1
#define ISO14443A_WRITE16_S2FRM_FPCD_LEN  16
#define ISO14443A_WRITE16_S2FPCD_DATA_POS 0
#define ISO14443A_WRITE16_S2FPCD_DATA_LEN 16

//value����֡��ʽ
#define ISO14443A_VALUE_S1FRM_FPCD_LEN  2
#define ISO14443A_VALUE_S1FPCD_CMD_POS  0
#define ISO14443A_VALUE_S1FPCD_ADDR_POS 1
#define ISO14443A_VALUE_S2FRM_FPCD_LEN  4
#define ISO14443A_VALUE_S2FPCD_VAL_POS  0
#define ISO14443A_VALUE_S3FRM_FPCD_LEN  2
#define ISO14443A_VALUE_S3FPCD_CMD_POS  0
#define ISO14443A_VALUE_S3FPCD_ADDR_POS 1


//��������֡
#define ISO14443A_HALTA_CMD_LEN        0x02
#define ISO14443A_HALTA_CMD_POS        0x00
#define ISO14443A_HALTA_PAR_POS        0x01




#endif

