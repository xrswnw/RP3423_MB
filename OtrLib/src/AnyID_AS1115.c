#include "AnyID_AS1115.h"


/******************************************************************************
** ��������AS1115_Init
** ˵  ������ʼ��AS1115���趨����ģʽ���Լ��Ƿ�ʹ��Ĭ�ϵ�ַ
**
** ��  �������룺decodeMode������ģʽ
** ��  �أ�void
******************************************************************************/
void AS1115_Init(u8 decodeMode)
{
    //10��������,ȫ������
    AS1115_WriteByte(AS1115_FACTORY_ADDR, AS1115_ADDR_DECODE_MOD, AS1115_DECODE_BHEX(decodeMode));
    //����
    AS1115_WriteByte(AS1115_FACTORY_ADDR, AS1115_ADDR_GLOBAL_INTENSITY, AS1115_ITST_DUTYCYCLE_02);//AS1115_ITST_DUTYCYCLE_0F
    //ȫ��ɨ��
    AS1115_WriteByte(AS1115_FACTORY_ADDR, AS1115_ADDR_SCAN_LIMIT, AS1115_SCAN_DISPLAY_D07);
     //Ĭ�Ͽ���:�ڲ�ʱ�ӡ����ƼĴ�������λ��10�������롢����˸
    AS1115_WriteByte(AS1115_FACTORY_ADDR, AS1115_ADDR_FEATURE, AS1115_SYS_CTRL_DEFAULT);

    //Normal״̬
    AS1115_WriteByte(AS1115_FACTORY_ADDR, AS1115_ADDR_SHUT_DOWN, AS1115_NORMAL_UNCHANGE_FEATURE);
//MTD��ʹ��Ĭ�ϵ�ַ�Ϳ����ˣ�����Ҫʹ���û���ַģʽ
#ifndef _ANYID_STM32_MTD_
    //��ȡ�û��Զ����ַ��ֻ����Normal״̬�ſ���ʹ��SEFL_ADDRESSָ��
    AS1115_WriteByte(AS1115_FACTORY_ADDR, AS1115_ADDR_SELF_ADRESS, AS1115_ADDR_MODE_USER);
#endif
}

/******************************************************************************
** ��������AS1115_WriteByte
** ˵  ������AS1115�ڲ��Ĵ���д������
**
** ��  �������룺addr��AS1115�豸��ַ(I2C��ַ)
                 command��AS1115����
                 data��д�������(�ֽ�)
** ��  �أ�д�����Ƿ�ɹ�
******************************************************************************/
BOOL AS1115_WriteByte(u8 addr, u8 command, u8 data)
{
    BOOL b = FALSE;

    if(!I2C_Start())return FALSE;

    I2C_SendByte(addr & I2C_WRITE_ADDR);//���ø���ʼ��ַ+������ַ
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}

    I2C_SendByte(command);   //���õ���ʼ��ַ
    b = I2C_WaitAck();

    I2C_SendByte(data);
    b = I2C_WaitAck();

    I2C_Stop();

    return b;
}

