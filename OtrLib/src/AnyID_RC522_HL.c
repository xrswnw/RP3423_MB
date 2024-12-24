#include "AnyID_RC522_HL.h"

/******************************************************************************
** ��������RC522_InitInterface
** ˵  ��: ��ʼ��RC522�����ӿ�
**
** ��  ����void
** ��  �أ�void
******************************************************************************/
void RC522_InitInterface(void)
{
#ifdef _ANYID_PIC18_
    //C3_CLK, C4_SDI, C5_SDO
    //����SPI���ܿڵ����/����
    LATC |= 0x38;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC4 = 1;
    TRISCbits.TRISC5 = 0;

    //Set the SPI module
    SSPSTAT = 0x40;
    SSPCON1 = 0x20;
    
    //CS���
    RC522_CS_TRIS = 0;

    //RST���
    RC522_RST_TRIS = 0;
#endif
}


/******************************************************************************
** ��������RC522_WriteByte
** ˵  ��: д��RC522һ���ֽ�
**
** ��  ����д��SPI��ֵ
** ��  �أ�SPI��ȡ��ֵ
******************************************************************************/
u8 RC522_WriteByte(u8 byte)
{
#ifdef _ANYID_PIC18_
    do
    {
        SSPCON1bits.WCOL = 0;
        SSPBUF = byte;
    } while(SSPCON1bits.WCOL);


    return SSPBUF;
#endif
}


