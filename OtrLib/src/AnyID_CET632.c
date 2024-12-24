#include "AnyID_CET632.h"

CET632_FRAME g_sCET632Frame = {0};

u8 CET632_ReadReg(u8 addr)
{
    u8 reg = 0;
    
    addr &= CET632_ADDR_MASK;
    addr <<= 1;
    addr |= 0x80;
    
    CET632_CsLow();
    CET632_WriteByte(addr);
    reg = CET632_ReadByte();
    CET632_CsHigh();
    
    return reg;
}
void CET632_WriteReg(u8 addr, u8 reg)
{
    addr &= CET632_ADDR_MASK;
    addr <<= 1;
    addr &= 0x7E;
    
    CET632_CsLow();
    CET632_WriteByte(addr);
    CET632_WriteByte(reg);
    CET632_CsHigh();
}

void CET632_ClearRegBit(u8 addr, u8 reg)
{
    u8 r = 0;
    r = CET632_ReadReg(addr);
    r &= (~reg);
    CET632_WriteReg(addr, r);
}
void CET632_SetRegBit(u8 addr, u8 reg)
{
    u8 r = 0;
    r = CET632_ReadReg(addr);
    r |= reg;
    CET632_WriteReg(addr, r);
}


void CET632_ReadFifo(u8 *pBuffer, u8 len)
{
    u8 i = 0;
    if(len > 0)
    {
        CET632_CsLow();
        CET632_WriteByte((CET632_REG_FIFO << 1) | 0x80);
        for(i = 0; i < len - 1; i++)
        {
            pBuffer[i] = CET632_WriteReadByte((CET632_REG_FIFO << 1) & 0x7E);
        }
        pBuffer[i] = CET632_ReadByte();
        CET632_CsHigh();
    }
}

void CET632_WriteFifo(u8 *pBuffer, u8 len)
{
    u8 i = 0;
    if(len > 0)
    {
        CET632_CsLow();
        CET632_WriteByte((CET632_REG_FIFO << 1) & 0x7E);
        for(i = 0; i < len; i++)
        {
            CET632_WriteByte(pBuffer[i]);
        }
        CET632_CsHigh();
    }
}

u8 CET632_CheckErr(u8 err)
{
    if(err & CET632_BIT_COLLERR)
    {
        return CET632_ERR_COLL;
    }
    else if(err & CET632_BIT_PARITYERR)
    {
        return CET632_ERR_PARITY;
    }
    else if(err & CET632_BIT_FRAMEERR)
    {
        return CET632_ERR_FRAME;
    }
    else if(err & CET632_BIT_CRCERR)
    {
        return CET632_ERR_CRC;
    }
    else if(err & CET632_BIT_FIFOOV)
    {
        return CET632_ERR_FIFO;
    }
    else if(err & CET632_BIT_ACCESSERR)
    {
        return CET632_ERR_ACCESS;
    }
    else if(err & CET632_BIT_KEYERR)
    {
        return CET632_ERR_KEY;
    }
    else if(err & CET632_BIT_E2ERR)
    {
        return CET632_ERR_EEPROM;
    }

    return CET632_ERR_SUCCESS;
}


u8 CET632_Transceive(u8 *pTxBuffer, u8 txLen, u8 txLastBits, u8 rxAlignBit, u8 *pRxBuffer, u8 *pRxLen, u32 usTimeOut)
{
    u8 reg = 0;
    u8 irq = 0;
    u8 state = CET632_ERR_SUCCESS;
    u16 tick = 0;
    
    //����жϱ�־
    CET632_WriteReg(CET632_REG_INTRQ, 0x3F);
    //���FIFO
    CET632_SetRegBit(CET632_REG_CTRL, CET632_BIT_FLUSHFIFO);

    irq = CET632_BIT_TXIEN | CET632_BIT_RXIEN | CET632_BIT_IDLEIEN;
    //����������д��fifo
    CET632_WriteFifo(pTxBuffer, txLen);
    //�������һ���ֽڵ�bit���ͽ��յĵ�һ��bit�����ڵ�һ���ֽڵ���ʼλ��
    CET632_WriteReg(CET632_REG_BITFRAME, (rxAlignBit << 4) | txLastBits); 
    //��ʱʱ�䣬����λmsת����Tick = 100us
    tick = usTimeOut / 10;
    //��������
    CET632_WriteReg(CET632_REG_CMD, CET632_CMD_TRANCEIVE);
    while(tick)
    {
        CET632_Delay100us(1);
        reg = CET632_ReadReg(CET632_REG_INTRQ);
        if((reg & irq) == irq)
        {
            *pRxLen = CET632_ReadReg(CET632_REG_FIFOLEN);
            CET632_ReadFifo(pRxBuffer, *pRxLen);
            break; 
        }
        tick--;
    }
    CET632_WriteReg(CET632_REG_CMD, CET632_CMD_IDLE);
    if(tick == 0)
    {
        state = CET632_ERR_TIMEOUT;
    }

    return state;
}


u8 CET632_Command(u8 cmd, u8 *pTxBuffer, u8 txLen, u8 *pRxBuffer, u8 *pRxLen, u32 usTimeOut)
{
    u8 state = CET632_ERR_SUCCESS;
    u32 tick = 0;
    u8 reg = 0;
    u8 rxLen = 0;
    u8 txPos = 0;
    
    //����жϱ�־
    CET632_WriteReg(CET632_REG_INTRQ, 0x3F);
    //���FIFO
    CET632_SetRegBit(CET632_REG_CTRL, CET632_BIT_FLUSHFIFO);

    //��ʱʱ�䣬����λusת����Tick = 100us
    tick = usTimeOut / 100;
    if(tick == 0)
    {
        tick = 1;
    }

    //ϣ�����ݵ�FIFO��������ݳ���FIFO�ĳ��ȣ��͵ȷ��ͺ�FIFO�ճ�λ����д
    if(txLen > CET632_SIZE_FIFO)
    {
        //����������д��fifo
        CET632_WriteFifo(pTxBuffer, CET632_SIZE_FIFO);
        txPos += CET632_SIZE_FIFO;
        txLen -= CET632_SIZE_FIFO;
    }
    else
    {
        //����������д��fifo
        CET632_WriteFifo(pTxBuffer, txLen);
        txPos += txLen;
        txLen -= txLen;
    }
    //���FIFO���жϱ�־
    CET632_ClearRegBit(CET632_REG_INTRQ, CET632_BIT_SETIRQ);
    reg = CET632_ReadReg(CET632_REG_INTRQ);
    //��������
    CET632_WriteReg(CET632_REG_CMD, cmd);
    //������������֡
    while(txLen > 0)
    {
        reg = CET632_ReadReg(CET632_REG_INTRQ);
        //���FIFO���жϱ�־���֣���ʾFIFO�п�λ������д������
        if((reg & CET632_BIT_FIFOLOIRQ) == CET632_BIT_FIFOLOIRQ)
        {
            if(txLen > (CET632_SIZE_FIFO - CET632_FIFO_LEVEL))
            {
                //����������д��fifo
                CET632_WriteFifo(pTxBuffer + txPos, CET632_SIZE_FIFO - CET632_FIFO_LEVEL);
                txPos += (CET632_SIZE_FIFO - CET632_FIFO_LEVEL);
                txLen -= (CET632_SIZE_FIFO - CET632_FIFO_LEVEL);
            }
            else
            {
                //����������д��fifo
                CET632_WriteFifo(pTxBuffer + txPos, txLen);
                txPos += txLen;
                txLen -= txLen;
            }
            //���FIFO�߱�־
            CET632_ClearRegBit(CET632_REG_INTRQ, CET632_BIT_SETIRQ);
        }
    }
    
    
    while(tick)
    {
        CET632_Delay100us(1);
        reg = CET632_ReadReg(CET632_REG_INTRQ);
        if((reg & CET632_BIT_IDLEIRQ) == CET632_BIT_IDLEIRQ)
        {
            if(pRxBuffer)
            {
                rxLen = CET632_ReadReg(CET632_REG_FIFOLEN);
                CET632_ReadFifo(pRxBuffer + *pRxLen, rxLen);
                *pRxLen += rxLen;
            }
            break; 
        }
        else if((reg & CET632_BIT_TXIRQ) == CET632_BIT_TXIRQ)
        {
            //���FIFO�ӽ��������϶�ȡFIFO���ճ��ռ�
            if((reg & CET632_BIT_HIALERT) == CET632_BIT_FIFOHIIRQ)
            {
                if(pRxBuffer)
                {
                    rxLen = CET632_ReadReg(CET632_REG_FIFOLEN);
                    CET632_ReadFifo(pRxBuffer + *pRxLen, rxLen);
                    *pRxLen += rxLen;
                }
            }
        }
        tick--;
    }
    CET632_WriteReg(CET632_REG_CMD, CET632_CMD_IDLE);
    if(tick == 0)
    {
        state = CET632_ERR_TIMEOUT;
    }

    return state;
}



u8 CET632_LoadKey(u8 *pKey, u8 keyLen)
{
    u8 reg = 0;
    u8 irq = 0;
    u8 state = CET632_ERR_SUCCESS;
    u16 tick = 0;
    
    //����жϱ�־
    CET632_WriteReg(CET632_REG_INTRQ, 0x3F);
    //���FIFO
    CET632_SetRegBit(CET632_REG_CTRL, CET632_BIT_FLUSHFIFO);

    irq = CET632_BIT_TXIEN | CET632_BIT_IDLEIEN;
    //����������д��fifo
    CET632_WriteFifo(pKey, keyLen);
    //��ʱʱ�䣬����λmsת����Tick = 100us
    tick = CET632_TIME_WEEPROM / 10;
    //����дEEPROM
    CET632_WriteReg(CET632_REG_CMD, CET632_CMD_WRITEE2);
    while(tick)
    {
        CET632_Delay100us(1);
        reg = CET632_ReadReg(CET632_REG_INTRQ);
        if((reg & irq) == irq)
        {
            u8 err = 0;
            err = CET632_ReadReg(CET632_REG_ERR);
            state = CET632_CheckErr(err);
            break; 
        }
        tick--;
    }
    CET632_WriteReg(CET632_REG_CMD, CET632_CMD_IDLE);
    if(tick == 0)
    {
        state = CET632_ERR_TIMEOUT;
    }

    return state;
}
