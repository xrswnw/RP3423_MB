#include "AnyID_VS10XX.h"


u8 g_nMp3ErrorTime = 0;;
u8 g_nMp3VolVal = VS10XX_VOL_DEFAULT;

u16 VS10XX_ReadRegister(u8 addressbyte)
{
    u16 resultvalue = 0;
    u8 temp1,temp2;

    VS10XX_SelectControl();
    VS10XX_Delay(1);
    VS10XX_WriteByte(VS10XX_READ_COMMAND);                                                //���ȷ���READ����
    VS10XX_WriteByte((addressbyte));                                                  //�ٷ��ͼĴ�����ַ
    temp1 = VS10XX_WriteByte(0xff);                                                     //��ȡ�Ĵ������ݸ��ֽ�
    resultvalue = (u16)temp1;
    resultvalue = (resultvalue) << 8;
    temp2 = VS10XX_WriteByte(0xff);                                                   //��ȡ�Ĵ������ݵ��ֽ�
    resultvalue |= (u16)temp2;
    VS10XX_Delay(1);
    VS10XX_DeselectControl();
    return resultvalue;
}

void VS10XX_SoftReset(void)
{
    VS10XX_WriteRegister(VS10XX_SPI_MODE, 0x08, 0x04); //Newmode, Reset, No L1-2

    VS10XX_Delay(1);        //One millisecond delay
    while(!VS10XX_DREQ);   //wait for startup

    //Send null bytes to data shorterface
    VS10XX_SelectData();
    VS10XX_WriteByte(0);
    VS10XX_WriteByte(0);
    VS10XX_WriteByte(0);
    VS10XX_WriteByte(0);
    VS10XX_DeselectData();
}

void VS10XX_Reset(void)
{
    VS10XX_PutInReset();
    VS10XX_Delay(1);

    VS10XX_WriteByte(0xFF);

    VS10XX_DeselectControl();
    VS10XX_DeselectData();
    VS10XX_ReleaseFromReset();

    while(!VS10XX_DREQ);

    VS10XX_SoftReset();

    //�����ʣ�48kHz�� ������                                                  //����VS1003ϵͳ����
    VS10XX_WriteRegister (VS10XX_SPI_AUDATA, 0xbb, 0x81);
    //����
    VS10XX_WriteRegister(VS10XX_SPI_VOL, g_nMp3VolVal, g_nMp3VolVal);
    g_nMp3VolVal = VS10XX_VOL_DEFAULT;
    //Set clock register, CLKI = Fosc * 4 = 12.288M * 4 = 49.1526M
    VS10XX_WriteRegister(VS10XX_SPI_CLOCKF, 0xD0, 0x00);
}

//����Mp3�Ƿ����������������ź�
void VS10XX_VsSineTest(void)
{
    VS10XX_PutInReset();
    VS10XX_Delay(100);

    VS10XX_WriteByte(0xFF);

    VS10XX_DeselectControl();
    VS10XX_DeselectData();
    VS10XX_ReleaseFromReset();
    VS10XX_Delay(100);

    VS10XX_WriteRegister(VS10XX_SPI_MODE, 0x08, 0x20);
    while (!VS10XX_DREQ);


    //��vs1003�������Ҳ������0x53 0xef 0x6e n 0x00 0x00 0x00 0x00
    //����n = 0x24, �趨vs1003�����������Ҳ���Ƶ��ֵ��������㷽����vs1003��datasheet
    //�����Ӧ���ܴӶ�������һ����һƵ�ʵ�����
    VS10XX_SelectData();
    VS10XX_WriteByte(0x53);
    VS10XX_WriteByte(0xef);
    VS10XX_WriteByte(0x6e);
    VS10XX_WriteByte(0x24);
    VS10XX_WriteByte(0x00);
    VS10XX_WriteByte(0x00);
    VS10XX_WriteByte(0x00);
    VS10XX_WriteByte(0x00);
    VS10XX_DeselectData();
    VS10XX_Delay (500);


    //�Ƴ�����
    VS10XX_SelectData();
    VS10XX_WriteByte(0x45);
    VS10XX_WriteByte(0x78);
    VS10XX_WriteByte(0x69);
    VS10XX_WriteByte(0x74);
    VS10XX_WriteByte(0x00);
    VS10XX_WriteByte(0x00);
    VS10XX_WriteByte(0x00);
    VS10XX_WriteByte(0x00);
    VS10XX_DeselectData();
    VS10XX_Delay(500);


    //����n = 0x44, �趨vs1003�����������Ҳ���Ƶ��ֵ��������㷽����vs1003��datasheet
    //�����Ӧ���ܴӶ���������һ����һƵ�ʵ�����
    VS10XX_SelectData();
    VS10XX_WriteByte(0x53);
    VS10XX_WriteByte(0xef);
    VS10XX_WriteByte(0x6e);
    VS10XX_WriteByte(0x44);
    VS10XX_WriteByte(0x00);
    VS10XX_WriteByte(0x00);
    VS10XX_WriteByte(0x00);
    VS10XX_WriteByte(0x00);
    VS10XX_DeselectData();
    VS10XX_Delay (500);

    //�˳�����
    VS10XX_SelectData();
    VS10XX_WriteByte(0x45);
    VS10XX_WriteByte(0x78);
    VS10XX_WriteByte(0x69);
    VS10XX_WriteByte(0x74);
    VS10XX_WriteByte(0x00);
    VS10XX_WriteByte(0x00);
    VS10XX_WriteByte(0x00);
    VS10XX_WriteByte(0x00);
    VS10XX_DeselectData();
    VS10XX_Delay(500);
}


//���ܣ�����1048��0��WMA��ʽ�������ж�ʱ����
void VS10XX_SendZeros(void)
{
    u16 i = 0;
    VS10XX_WriteByte(0xff);
    VS10XX_SelectData();
    VS10XX_WriteByte(0);
    for(i = 0; i < 1048; i++)
    {
        while(!VS10XX_DREQ);
        VS10XX_WriteByte(0);
    }
    VS10XX_DeselectData();
}

void VS10XX_Delay(u16 mS)
{
    u16 i;
    u32 j;
    for(i = 0; i < mS; i++)
    {
        for(j = 0; j < 0xfff; j++);
    }
}


void VS10XX_WriteRegister(u8 addressbyte, u8 highbyte, u8 lowbyte)
{
    VS10XX_SelectControl();
    while(!VS10XX_DREQ);
    VS10XX_WriteByte(VS10XX_WRITE_COMMAND);                                               //����Write����
    VS10XX_WriteByte((addressbyte));                                                  //���ͼĴ�����ַ
    VS10XX_WriteByte((highbyte));                                                     //�������ݸ��ֽ�
    VS10XX_WriteByte((lowbyte));                                                      //�������ݵ��ֽ�
    VS10XX_Delay(1);
    VS10XX_DeselectControl();
}

void VS10XX_Send32ByteData(u8 *pBuffer)
{
    u8 i = 0;
    for(i = 0; i < VS10XX_MINSENDTOMP3; i++)
    {
        VS10XX_WriteByte(pBuffer[i]);
    }
}

void VS10XX_SetVolumn(u8 vol)
{
    VS10XX_SelectControl();
    while(!VS10XX_DREQ);
    VS10XX_WriteByte(VS10XX_WRITE_COMMAND);
    VS10XX_WriteByte((VS10XX_SPI_VOL));
    VS10XX_WriteByte(vol);
    VS10XX_WriteByte(vol);
    VS10XX_Delay(1);
    VS10XX_DeselectControl();
}

