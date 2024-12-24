#include "AnyID_EEPROM.h"
/********************************************************************************
** ������: EEPROM_WriteByte
** ˵  ��: д������ʱ��һ��ҪҪ�㹻��ʱ����EEPROM�ڲ����д�Ĳ����������������ٵ�д���ݿ��ܲ��ܳɹ�
**
** ��  ��:д���ַ����д�������
** ��  ��:void
** ��  ��: д��ɹ�TRUE,  д��ʧ��FALSE
********************************************************************************/
BOOL EEPROM_WriteByte(u16 addr, u8 data)
{
    BOOL b = TRUE;

    //��ʼ
    if(!I2C_Start())
    {
        b = FALSE;
    }

    //�豸��ַ�����ݸߵ�ַ
    if(b)
    {
        I2C_SendByte((EEPROM_DEV_ADDR & I2C_WRITE_ADDR) | ((addr >> 7) & 0x06));
        if(!I2C_WaitAck())
        {
            b = FALSE;
        }
    }

    //���ݵ͵�ַ
    if(b)
    {
        I2C_SendByte((addr >> 0) & 0xff);
        b = I2C_WaitAck();
    }

    //д������
    if(b)
    {
        I2C_SendByte(data);
        b = I2C_WaitAck();
    }

    //ֹͣ
    I2C_Stop();

    return b;
}

/********************************************************************************
** ������: EEPROM_WritePage
** ˵  ��: ע��ҳ�Ĵ�С
**                 �����ҳ�ı߽翪ʼд�����ݣ�����д��һҳΪֹ
**                 �����ҳ���м俪ʼд�����ݾ֣�д��ҳĩʱ����ַ�ᷭת��ҳ�Ŀ�ʼ����д��
** ��  ��: д���ַ����д���ҳ���ݣ� ҳ���ݳ���
** ��  ��: void
** ��  ��: д��ɹ�TRUE,  д��ʧ��FALSE
********************************************************************************/
BOOL EEPROM_WritePage(u16 addr, u8 *pBuffer, u8 len)
{
    u8 i = 0;
    BOOL b = TRUE;

    u8 actualLen = EEPROM_PAGE_SIZE;

    if(len < actualLen)
    {
        actualLen = len;
    }

    //��ʼ
    if(!I2C_Start())
    {
        b = FALSE;
    }

    //�豸��ַ�����ݸߵ�ַ
    if(b)
    {
        I2C_SendByte((EEPROM_DEV_ADDR & I2C_WRITE_ADDR) | ((addr >> 7) & 0x06));
        if(!I2C_WaitAck())
        {
            b = FALSE;
        }
    }

    //���ݵ͵�ַ
    if(b)
    {
        I2C_SendByte((addr >> 0) & 0xff);
        b = I2C_WaitAck();
    }

    //д������
    for(i = 0; i < actualLen; i++)
    {
        if(b)
        {
            I2C_SendByte(pBuffer[i]);
            b = I2C_WaitAck();
        }
    }

    //ֹͣ
    I2C_Stop();

    return b;
}

/********************************************************************************
** ������: EEPROM_ReadCurrenAddr
** ˵  ��: �ӵ�ǰλ�ý��ж�����
**
** ��  ��: void
** ��  ��: ��ȡ��������
** ��  ��: ��ȡ�ɹ�TRUE,  ��ȡʧ��FALSE
********************************************************************************/
BOOL EEPROM_ReadCurrenAddr(u8 *pData)
{
    BOOL b = TRUE;

    //��ʼ
    if(!I2C_Start())
    {
        b = FALSE;
    }

    //�豸��ַ�Ͷ�����������
    if(b)
    {
        I2C_SendByte(EEPROM_DEV_ADDR | I2C_READ_ADDR);
        if(!I2C_WaitAck())
        {
            b = FALSE;
        }
    }

    //
    if(b)
    {
        *pData = I2C_ReceiveByte();
        b = (BOOL)(!I2C_WaitAck());     //ע��:������û��ACK��
    }

    //ֹͣ
    I2C_Stop();

    return b;
}

/********************************************************************************
** ������: EEPROM_ReadRandomAddr
** ˵  ��: �ӼĴ���������λ�ý��ж�����
**
** ��  ��: ��ȡλ��
** ��  ��: ��ȡ��������
** ��  ��: ��ȡ�ɹ�TRUE,  ��ȡʧ��FALSE
********************************************************************************/
BOOL EEPROM_ReadRandomAddr(u16 addr, u8 *pData)
{

    BOOL b = TRUE;

    //��ʼ
    if(!I2C_Start())
    {
        b = FALSE;
    }


    //�豸��ַ�����ݸߵ�ַ
    if(b)
    {
        I2C_SendByte((EEPROM_DEV_ADDR & I2C_WRITE_ADDR) | ((addr >> 7) & 0x06));
        if(!I2C_WaitAck())
        {
            b = FALSE;
        }
    }

    //���ݵ͵�ַ
    if(b)
    {
        I2C_SendByte((addr >> 0) & 0xff);
        b = I2C_WaitAck();
    }


    //��ʼ
    if(!I2C_Start())
    {
        b = FALSE;
    }

    //�豸��ַ
    if(b)
    {
        I2C_SendByte(EEPROM_DEV_ADDR | I2C_READ_ADDR | ((addr >> 7) & 0x06));
        if(!I2C_WaitAck())
        {
            b = FALSE;
        }
    }

    //������������
    if(b)
    {
        *pData = I2C_ReceiveByte();
        b = (BOOL)(!I2C_WaitAck());     //ע��:������û��ACK��
    }

    //ֹͣ
    I2C_Stop();

    return b;
}

/********************************************************************************
** ������: EEPROM_ReadSequential
** ˵  ��: ������ȡ����
**
** ��  ��:  ��ȡλ�ã� ��ȡ����
** ��  ��:  ��ȡ��������
** ��  ��:  ��ȡ�ɹ�TRUE,  ��ȡʧ��FALSE
********************************************************************************/
BOOL EEPROM_ReadSequential(u16 addr, u8 *pBuffer, u16 len)
{
    BOOL b = TRUE;

    u16 i = 0;

    //��ʼ
    if(!I2C_Start())
    {
        b = FALSE;
    }


    //�豸��ַ�����ݸߵ�ַ
    if(b)
    {
        I2C_SendByte((EEPROM_DEV_ADDR & I2C_WRITE_ADDR) | ((addr >> 7) & 0x06));
        if(!I2C_WaitAck())
        {
            b = FALSE;
        }
    }

    //���ݵ͵�ַ
    if(b)
    {
        I2C_SendByte((addr >> 0) & 0xff);
        b = I2C_WaitAck();
    }


    //��ʼ
	if(b)
	{
		if(!I2C_Start())
		{
			b = FALSE;
        }
	}

    //�豸��ַ
    if(b)
    {
        I2C_SendByte(EEPROM_DEV_ADDR | I2C_READ_ADDR | ((addr >> 7) & 0x06));
        if(!I2C_WaitAck())
        {
            b = FALSE;
        }
    }

    //�����������ݣ����һ������Ҫ��NOACK��β
	len--;		
	if(b)
	{
		for(i = 0; i < len; i++)
		{
			pBuffer[i] = I2C_ReceiveByte();
			I2C_Ack();
		}
		
		pBuffer[i] = I2C_ReceiveByte();
		I2C_NoAck();
	}
	
    I2C_Stop();

    return b;
}



