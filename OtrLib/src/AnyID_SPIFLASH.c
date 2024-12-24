#include "AnyID_SPIFLASH.h"


//�ȴ�SPIFLASH��Write�������
void SPIFLASH_WaitNoBusy(void)
{
    u8 reg = 0xFF;

    while((reg & SPIFLASH_STATE_BUSY) == SPIFLASH_STATE_BUSY)
    {
        SPIFLASH_CS_Low();
        
        SPIFLASH_WriteByte(SPIFLASH_CMD_RDSR);
        
        reg = SPIFLASH_WriteByte(SPIFLASH_DUMMY);
        
        SPIFLASH_CS_High();
    }
}

//��ȡָ����ַ��ʼ�����ݣ����Զ�ȡ���ⳤ�ȣ�����FLASH�����һ���ֽ�ʱ����ַ�Զ���ת��0x0000
void SPIFLASH_Read(u32 address, u8 *pBuffer, u32 len)
{
    u32 i = 0;

    SPIFLASH_WaitNoBusy();

    SPIFLASH_CS_Low();
    SPIFLASH_WriteByte(SPIFLASH_CMD_READ);
    SPIFLASH_WriteByte((address >> 16) & 0xff);
    SPIFLASH_WriteByte((address >> 8) & 0xff);
    SPIFLASH_WriteByte((address >> 0) & 0xff);
    for(i = 0; i < len; i++)
    {
        pBuffer[i] = SPIFLASH_WriteByte(SPIFLASH_DUMMY);
    }
    SPIFLASH_CS_High();

}

//��ȡSPIFLASH��ID
void SPIFLASH_ReadID(u8 *pSPIFLASHID)
{
    u32 i = 0;
    SPIFLASH_WaitNoBusy();

    SPIFLASH_CS_Low();
    SPIFLASH_WriteByte(SPIFLASH_CMD_RDID);

    for(i = 0; i < 5; i++)
    {
        pSPIFLASHID[i] = SPIFLASH_WriteByte(SPIFLASH_DUMMY);
    }
    SPIFLASH_CS_High();
}

//��ȡSPIFLASH���豸ID
void SPIFLASH_ReadDeviceID(u8 *pDeviceID, u8 *pManufacturerID)
{
    SPIFLASH_WaitNoBusy();

    SPIFLASH_CS_Low();

    SPIFLASH_WriteByte(SPIFLASH_CMD_READ_ID);

    SPIFLASH_WriteByte(0x00);
    SPIFLASH_WriteByte(0x00);
    SPIFLASH_WriteByte(0x00);

    *pManufacturerID = SPIFLASH_WriteByte(SPIFLASH_DUMMY);
    *pDeviceID = SPIFLASH_WriteByte(SPIFLASH_DUMMY);

    SPIFLASH_CS_High();
}

//ʹ��SPIFLASHд�������κ�д����֮ǰ��Ҫʹ�øú���
void SPIFLASH_WriteEnable(void)
{
    SPIFLASH_CS_Low();
    SPIFLASH_WriteByte(SPIFLASH_CMD_WREN);

    SPIFLASH_CS_High();
}

//����һ��sector
void SPIFLASH_SectorErase(u32 address)
{
    SPIFLASH_WaitNoBusy();

    SPIFLASH_WriteEnable();

    SPIFLASH_CS_Low();
    SPIFLASH_WriteByte(SPIFLASH_CMD_SE);
    SPIFLASH_WriteByte((address >> 16) & 0xff);
    SPIFLASH_WriteByte((address >> 8) & 0xff);
    SPIFLASH_WriteByte((address >> 0) & 0xff);

    SPIFLASH_CS_High();
}

void SPIFLASH_Parameter4kSectorErase(u32 address)
{
    SPIFLASH_WaitNoBusy();

    SPIFLASH_WriteEnable();

    SPIFLASH_CS_Low();
    SPIFLASH_WriteByte(SPIFLASH_CMD_P4E);
    SPIFLASH_WriteByte((address >> 16) & 0xff);
    SPIFLASH_WriteByte((address >> 8) & 0xff);
    SPIFLASH_WriteByte((address >> 0) & 0xff);

    SPIFLASH_CS_High();
}


//��̲�����SPIFLASH_PAGE_SIZE���ֽڵ�����
//�����̵ĵ�ַ����Page����ʼ��ַ(bit0~7 == 0)��
//�ﵽPage��βʱ���������д�����ݣ���ַ�ᷭת��Page�Ŀ�ʼ
//page��̲��ᳬ��һ��Page��
void SPIFLASH_PageProgram(u32 address, u8 *pBuffer, u16 len)
{
    u16 i = 0;
    if(len > SPIFLASH_PAGE_SIZE)
    {
        len = SPIFLASH_PAGE_SIZE;
    }

    SPIFLASH_WaitNoBusy();

    SPIFLASH_WriteEnable();


    SPIFLASH_CS_Low();
    SPIFLASH_WriteByte(SPIFLASH_CMD_PP);
    SPIFLASH_WriteByte((address >> 16) & 0xff);
    SPIFLASH_WriteByte((address >> 8) & 0xff);
    SPIFLASH_WriteByte((address >> 0) & 0xff);
    for(i = 0; i < len; i++)
    {
        SPIFLASH_WriteByte(pBuffer[i]);
    }

    SPIFLASH_CS_High();
}

//��ȡSPIFLASH��״̬�Ĵ���
u8 SPIFLASH_ReadStatusRegister(void)
{
    u8 reg = 0;

    SPIFLASH_CS_Low();

    SPIFLASH_WriteByte(SPIFLASH_CMD_RDSR);

    reg = SPIFLASH_WriteByte(SPIFLASH_DUMMY);

    SPIFLASH_CS_High();
    return reg;
}

//��ȡSPIFLASH�����üĴ���
u8 SPIFLASH_ReadConfigRegister(void)
{
    u8 reg = 0;
	
    SPIFLASH_CS_Low();

    SPIFLASH_WriteByte(SPIFLASH_CMD_RCR);

    reg = SPIFLASH_WriteByte(SPIFLASH_DUMMY);

    SPIFLASH_CS_High();
    return reg;
}

//дSPIFLASH��״̬�Ĵ���
void SPIFLASH_WriteStatusRegister(u8 reg)
{
    SPIFLASH_WriteEnable();

    SPIFLASH_CS_Low();
    SPIFLASH_WriteByte(SPIFLASH_CMD_WRR);
    SPIFLASH_WriteByte(reg);

    SPIFLASH_CS_High();
}

//���SPIFLASH��״̬�Ĵ���
void SPIFLASH_ClearStatusRegister(void)
{
    SPIFLASH_WriteEnable();

    SPIFLASH_CS_Low();
    SPIFLASH_WriteByte(SPIFLASH_CMD_CLSR);

    SPIFLASH_CS_High();
}


u32 SPIFLASH_ReadData(u32 address, u32 len)
{
    u8 buf[4] = {0};
    u32 temp = 0;
    u8 i = 0;

    SPIFLASH_Read(address, buf, len);

    for(i = 0; i < len; i++)
    {
        temp <<= 8;
        temp += buf[i];
    }

    return temp;
}

void SPIFLASH_WriteData(u32 address, u32 data, u32 len)
{
    u8 buf[4] = {0};
    u8 temp = 0;
    u8 i = 0;

    for(i = len; i > 0; i--)
    {
        temp = data & 0xFF;
        buf[i - 1] = temp;
        data >>= 8;
    }
	
    SPIFLASH_PageProgram(address, buf, len);
}


void SPIFLASH_WriteBuffer(u32 address, u8 *pBuffer, u32 len)
{
    u32 offset = 0;

    while(len >= SPIFLASH_PAGE_SIZE)
    {
        SPIFLASH_PageProgram(address + offset, pBuffer + offset, SPIFLASH_PAGE_SIZE);
        len -= SPIFLASH_PAGE_SIZE;
        offset += SPIFLASH_PAGE_SIZE;
    }

    SPIFLASH_PageProgram(address + offset, pBuffer + offset, len);
}


