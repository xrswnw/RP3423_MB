#include "AnyID_Fm17xx.h"
#include "AnyID_Des.h"

FM17XX_FRAME g_sFm17xxFrame = {0};

/******************************************************************************
** ��������FM17xx_WriteReg
** ˵  ��: д��FM17xx�Ĵ���
**
** ��  ����addr����ַ
           val��ֵ
** ��  �أ�void
******************************************************************************/
void FM17xx_WriteReg(u8 addr, u8 val)
{
    FM17xx_CS_Low();

    FM17xx_WriteByte(FM17XX_WADDR(addr));
    FM17xx_WriteByte(val);

    FM17xx_CS_High();
}

/******************************************************************************
** ��������FM17xx_ReadReg
** ˵  ��: ��FM17xx�Ĵ���
**
** ��  ����addr����ַ
** ��  �أ��Ĵ���ֵ
******************************************************************************/
u8 FM17xx_ReadReg(u8 addr)
{
    u8 val = 0;

    FM17xx_CS_Low();

    FM17xx_WriteByte(FM17XX_RADDR(addr));
    val = FM17xx_WriteByte(0x00);

    FM17xx_CS_High();

    return val;
}

/******************************************************************************
** ��������FM17xx_SetRegBit
** ˵  ��: ���üĴ�����ĳһλ
**
** ��  ����addr����ַ
           bit����Ҫ��1��λ
** ��  �أ�void
******************************************************************************/
void FM17xx_SetRegBit(u8 addr, u8 bit)
{
    u8 val = 0;

    val = FM17xx_ReadReg(addr);
    val |= bit;
    FM17xx_WriteReg(addr, val);
}

/******************************************************************************
** ��������FM17xx_ClearRegBit
** ˵  ��: ����Ĵ�����ĳһλ
**
** ��  ����addr����ַ
           bit��������Ҫ��1��λ
** ��  �أ�void
******************************************************************************/
void FM17xx_ClearRegBit(u8 addr, u8 bit)
{
    u8 val = 0;

    val = FM17xx_ReadReg(addr);
    val &= (~bit);
    FM17xx_WriteReg(addr, val);
}

/******************************************************************************
** ��������FM17xx_ReadFIFO
** ˵  ��: ��FM17xx��FIFO
**
** ��  ����pBuffer����ȡ�����ݻ�����
           len����Ҫ��ȡ�ĳ���
** ��  �أ�void
******************************************************************************/
void FM17xx_ReadFIFO(u8 *pBuffer, u8 len)
{
    u8 i = 0;

    FM17xx_CS_Low();

    FM17xx_WriteByte(FM17XX_RADDR(FM17XX_REG_FIFODATA));

    for(i = 0; i < (len - 1); i++)
    {
        pBuffer[i] = FM17xx_WriteByte(FM17XX_RADDR(FM17XX_REG_FIFODATA));
    }

    pBuffer[i] = FM17xx_WriteByte(0x00);

    FM17xx_CS_High();
}

/******************************************************************************
** ��������FM17xx_ReadFIFO
** ˵  ��: дFM17xx��FIFO
**
** ��  ����pBuffer��д���ݶ�ȡ�Ļ�����
           len����Ҫд�ĳ���
** ��  �أ�void
******************************************************************************/
void FM17xx_WriteFIFO(u8 *pBuffer, u8 len)
{
    u8 i = 0;

    FM17xx_CS_Low();

    FM17xx_WriteByte(FM17XX_WADDR(FM17XX_REG_FIFODATA));

    for(i = 0; i < len; i++)
    {
        FM17xx_WriteByte(pBuffer[i]);
    }

    FM17xx_CS_High();
}

/******************************************************************************
** ��������FM17xx_Reset
** ˵  ��: ��ʼ��FM17xx
**
** ��  ����void
** ��  �أ�void
******************************************************************************/
BOOL FM17xx_Reset(void)
{
    u8 regVal = 0;
    BOOL b = FALSE;

    //ϵͳӲ����λ
    FM17xx_PowerLow();                                          //�͵�ƽ�����ȴ���1ms
    FM17xx_Delay16us(2560);
    FM17xx_PowerHigh();

    //�����ȶ�ʱ��800us������2ms
    FM17xx_Delay16us(5120);

    //�����������
    FM17xx_WriteReg(FM17XX_REG_TXAUTO, FM17XX_BIT_100ASK);     //force to 100% ASK
    regVal = FM17xx_ReadReg(FM17XX_REG_TXAUTO);
    if(regVal == FM17XX_BIT_100ASK)
    {
        b = TRUE;
    }

    return b;
}

u8 FM17xx_Command(void)
{
    u8 state = 0;
    u32 tick = 0;
    u8 reg = 0;
    u8 txPos = 0;
    u8 fifoLevel = 0;

    g_sFm17xxFrame.rxLen = 0;
    tick = g_sFm17xxFrame.timeout >> 4; //16us = tick

    //��ֹ��ǰ���в���
    FM17xx_WriteReg(FM17XX_REG_COMMAND, FM17XX_CMD_IDLE);
    //���FIFO
    FM17xx_WriteReg(FM17XX_REG_FIFOLEVEL, FM17XX_BIT_FLUSHBUFFER);
    //��������жϱ�־λ
    FM17xx_WriteReg(FM17XX_REG_COMMIRQ, 0x7F);

    //ϣ�����ݵ�FIFO��������ݳ���FIFO�ĳ��ȣ��͵ȷ��ͺ�FIFO�ճ�λ����д
    if(g_sFm17xxFrame.txLen > FM17XX_SIZE_FIFO)
    {
        //����������д��fifo
        FM17xx_WriteFIFO(g_sFm17xxFrame.frame, FM17XX_SIZE_FIFO);
        txPos += FM17XX_SIZE_FIFO;
        g_sFm17xxFrame.txLen -= FM17XX_SIZE_FIFO;
    }
    else
    {
        //����������д��fifo
        FM17xx_WriteFIFO(g_sFm17xxFrame.frame, g_sFm17xxFrame.txLen);
        txPos += g_sFm17xxFrame.txLen;
        g_sFm17xxFrame.txLen -= g_sFm17xxFrame.txLen;
    }

    //��������жϱ�־λ
    FM17xx_WriteReg(FM17XX_REG_COMMIRQ, 0x7F);

    //�������FM17XX_CMD_TRANSCEIVEʹ��FM17XX_CMD_STARTSEND����
    //          ��������д��CMDReg����Զ�����
    FM17xx_WriteReg(FM17XX_REG_COMMAND, g_sFm17xxFrame.cmd & FM17XX_MASK_COMMAND);

    if(g_sFm17xxFrame.cmd == FM17XX_CMD_TRANSCEIVE)
    {
        //��������
        FM17xx_SetRegBit(FM17XX_REG_BITFRAMING, FM17XX_BIT_STARTSEND);
    }

    //������������֡
    while(g_sFm17xxFrame.txLen > 0)
    {
        reg = FM17xx_ReadReg(FM17XX_REG_COMMIRQ);
        //���FIFO���жϱ�־���֣���ʾFIFO�п�λ������д������
        if(reg & FM17XX_BIT_LOALERTI)
        {
            if(g_sFm17xxFrame.txLen > (FM17XX_SIZE_FIFO - FM17XX_FIFO_LEVEL))
            {
                //����������д��fifo
                FM17xx_WriteFIFO(g_sFm17xxFrame.frame + txPos, FM17XX_SIZE_FIFO - FM17XX_FIFO_LEVEL);
                txPos += (FM17XX_SIZE_FIFO - FM17XX_FIFO_LEVEL);
                g_sFm17xxFrame.txLen -= (FM17XX_SIZE_FIFO - FM17XX_FIFO_LEVEL);
            }
            else
            {
                //����������д��fifo
                FM17xx_WriteFIFO(g_sFm17xxFrame.frame + txPos, g_sFm17xxFrame.txLen);
                txPos += g_sFm17xxFrame.txLen;
                g_sFm17xxFrame.txLen -= g_sFm17xxFrame.txLen;
            }
            FM17xx_WriteReg(FM17XX_REG_COMMIRQ, FM17XX_BIT_LOALERTI); //���lo��־
        }
        else if(reg & FM17XX_BIT_ERRI)
        {
            tick = 0;
            break;
        }
    }

    //��������жϱ�־λ
    FM17xx_WriteReg(FM17XX_REG_COMMIRQ, 0x7F);

    while(tick)
    {
        //�����ʱ������10us
        FM17xx_Delay16us(1);
        reg = FM17xx_ReadReg(FM17XX_REG_COMMIRQ);
        if(reg & FM17XX_BIT_ERRI)
        {
            state = FM17XX_STAT_ERROR;
            g_sFm17xxFrame.rxLen = 0;
            break;
        }
        else if(reg & FM17XX_BIT_RXI)
        {
            fifoLevel = FM17xx_ReadReg(FM17XX_REG_FIFOLEVEL);
            FM17xx_ReadFIFO(g_sFm17xxFrame.frame + g_sFm17xxFrame.rxLen, fifoLevel);
            g_sFm17xxFrame.rxLen += fifoLevel;
            break;
        }
        else if(reg & FM17XX_BIT_TXI)
        {
            if(reg & FM17XX_BIT_HIALERTI)
            {
                fifoLevel = FM17xx_ReadReg(FM17XX_REG_FIFOLEVEL);
                FM17xx_ReadFIFO(g_sFm17xxFrame.frame + g_sFm17xxFrame.rxLen, fifoLevel);
                FM17xx_WriteReg(FM17XX_REG_COMMIRQ, FM17XX_BIT_HIALERTI);
                g_sFm17xxFrame.rxLen += fifoLevel;
            }
            else if(g_sFm17xxFrame.cmd == FM17XX_CMD_TRANSMIT)
            {
                break;
            }
        }
        else if(reg & FM17XX_BIT_IDLEI)
        {
            fifoLevel = FM17xx_ReadReg(FM17XX_REG_FIFOLEVEL);
            FM17xx_ReadFIFO(g_sFm17xxFrame.frame + g_sFm17xxFrame.rxLen, fifoLevel);
            g_sFm17xxFrame.rxLen += fifoLevel;
            break;
        }
        tick--;
    }

    //��ֹ��ǰ���в���
    FM17xx_WriteReg(FM17XX_REG_COMMAND, FM17XX_CMD_IDLE);

    if(g_sFm17xxFrame.cmd == FM17XX_CMD_TRANSCEIVE)
    {
        //ֹͣ����
        FM17xx_ClearRegBit(FM17XX_REG_BITFRAMING, FM17XX_BIT_STARTSEND);
    }

    if(tick == 0)
    {
        state = FM17XX_STAT_TIMEOUT;
    }
    return state;
}


void FM17xx_Config(BOOL bClearCrypt, BOOL bTxCrc, BOOL bRxCrc, u8 txLastBits, u8 rxAlignBits)
{
    if(bClearCrypt)
    {
        FM17xx_ClearRegBit(FM17XX_REG_STATUS2, FM17XX_BIT_CRYPTO1ON);   //���ķ���
    }

    if(bTxCrc)
    {
        FM17xx_SetRegBit(FM17XX_REG_TXMODE, FM17XX_BIT_CRCEN);
    }
    else
    {
        FM17xx_ClearRegBit(FM17XX_REG_TXMODE, FM17XX_BIT_CRCEN);
    }

    if(bRxCrc)
    {
        FM17xx_SetRegBit(FM17XX_REG_RXMODE, FM17XX_BIT_CRCEN);
    }
    else
    {
        FM17xx_ClearRegBit(FM17XX_REG_RXMODE, FM17XX_BIT_CRCEN);
    }

    FM17xx_WriteReg(FM17XX_REG_BITFRAMING, txLastBits | (rxAlignBits << 4));
}




