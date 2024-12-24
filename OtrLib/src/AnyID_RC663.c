#include "AnyID_RC663.h"
RC663_FRAME g_sRc663Frame = {0};

const u8 g_sRc663RegAdd[RC663_PROTOCOL_CONFIG_SIZE] =
{
    RC663_REG_DRVMODE,
    RC663_REG_TXAMP,
    RC663_REG_DRVCON,
    RC663_REG_TXI,
    RC663_REG_TXCRCCON,
    RC663_REG_RXCRCCON,
    RC663_REG_TXDATANUM,
    RC663_REG_TXMODWIDTH,
    RC663_REG_TXSYM10BURSTLEN,
    RC663_REG_TXWAITCTRL,
    RC663_REG_FRAMECON,
    RC663_REG_RXCTRL,
    RC663_REG_RXTHRESHOLD,
    RC663_REG_RCV,
    RC663_REG_RXANA
};


void RC663_WriteReg(u8 addr, u8 val)
{
    RC663_CS_Low();

    RC663_WriteByte(RC663_WADDR(addr));
    RC663_WriteByte(val);

    RC663_CS_High();
}

u8 RC663_ReadReg(u8 addr)
{
    u8 val = 0;

    RC663_CS_Low();

    RC663_WriteByte(RC663_RADDR(addr));
    val = RC663_WriteByte(0xFF);

    RC663_CS_High();

    return val;
}

void RC663_SetRegBit(u8 addr, u8 bit)
{
    u8 val = 0;

    val = RC663_ReadReg(addr);
    val |= bit;
    RC663_WriteReg(addr, val);
}

void RC663_ClearRegBit(u8 addr, u8 bit)
{
    u8 val = 0;

    val = RC663_ReadReg(addr);
    val &= (~bit);
    RC663_WriteReg(addr, val);
}

void RC663_SetRegBits(u8 addr, u8 msk, u8 bit)
{
    u8 val = 0;

    val = RC663_ReadReg(addr);
    val &= (~msk);
    val |= bit;
    RC663_WriteReg(addr, val);
}

void RC663_ReadFIFO(u8 *pBuffer, u8 len)
{
    u8 i = 0;

    RC663_CS_Low();

    RC663_WriteByte(RC663_RADDR(RC663_REG_FIFODATA));

    for(i = 0; i < (len - 1); i++)
    {
        pBuffer[i] = RC663_WriteByte(RC663_RADDR(RC663_REG_FIFODATA));
    }

    pBuffer[i] = RC663_WriteByte(0xFF);

    RC663_CS_High();
}

void RC663_WriteFIFO(u8 *pBuffer, u8 len)
{
    u8 i = 0;

    RC663_CS_Low();

    RC663_WriteByte(RC663_WADDR(RC663_REG_FIFODATA));

    for(i = 0; i < len; i++)
    {
        RC663_WriteByte(pBuffer[i]);
    }

    RC663_CS_High();
}

BOOL RC663_Reset(void)
{
    u8 regVal = RC663_STAT_ERROR;
    BOOL bOk = FALSE;
    u8 t = 0;

    //ϵͳӲ����λ
    RC663_PowerHigh();                                          //�͵�ƽ�����ȴ���100ns����
    RC663_Delay16us(1000);
    RC663_PowerLow();
    RC663_Delay16us(10000);
    //�ȴ�ϵͳ�ȶ�
    do{
        RC663_Delay16us(1000);
        t++;
        regVal = RC663_ReadReg(RC663_REG_VERSION);
        if((regVal != 0) && (regVal != 0xFF))
        {
            bOk = TRUE;
            break;
        }
    }while(t < 100);

    return bOk;
}

u8 RC663_Init(void)
{
    u8 state = RC663_STAT_ERROR;
    if(RC663_Reset())
    {
        state = RC663_STAT_OK;
    }

    RC663_WriteReg(RC663_REG_COMMAND, 0x00);
    //256�ֽڡ����FIFO��waterLevel8bit��fifoLength8bit
    RC663_WriteReg(RC663_REG_FIFOCONTROL, RC663_BIT_FIFOSIZE | RC663_BIT_FLUSHFIFO);
    //WaterLevel = 64�ֽ�
    RC663_WriteReg(RC663_REG_WATERLEVEL, RC663_FIFO_LEVEL);
    
    
    return state;
}

void RC663_Config(u8 bClearCrypt, u8 bEnTxCrc, u8 bEnRxCrc, u8 txLastBits, u8 rxAlignBits)
{
    u8 reg = 0;

    if(bClearCrypt)
    {
        RC663_ClearRegBit(RC663_REG_STATUS, RC663_BIT_CRYPTO1ON);
    }

    if(bEnTxCrc)
    {
        RC663_SetRegBit(RC663_REG_TXCRCCON, RC663_BIT_CRCEN);
    }
    else
    {
        RC663_ClearRegBit(RC663_REG_TXCRCCON, RC663_BIT_CRCEN);
    }

    if(bEnRxCrc)
    {
        RC663_SetRegBit(RC663_REG_RXCRCCON, RC663_BIT_CRCEN);
    }
    else
    {
        RC663_ClearRegBit(RC663_REG_RXCRCCON, RC663_BIT_CRCEN);
    }

    reg = RC663_ReadReg(RC663_REG_TXDATANUM);
    reg &= (~RC663_MASK_TXLASTBITS);
    reg |= (RC663_MASK_TXLASTBITS & txLastBits);
    RC663_WriteReg(RC663_REG_TXDATANUM, reg);

    reg = RC663_ReadReg(RC663_REG_RXBITCTRL);
    reg &= (~RC663_MASK_RXALIGN);
    reg |= ((rxAlignBits << 4) & RC663_MASK_RXALIGN);
    RC663_WriteReg(RC663_REG_RXBITCTRL, reg);
}

u8 RC663_SelectProtocol(u8 tx, u8 rx)
{
    g_sRc663Frame.frame[0] = rx;
    g_sRc663Frame.frame[1] = tx;
    g_sRc663Frame.txLen = 2;

    g_sRc663Frame.timeout = 10000;
    g_sRc663Frame.cmd = RC663_CMD_LOADPROTOCOL;
    
    //��ֹ��ǰ���в���
    RC663_WriteReg(RC663_REG_COMMAND, RC663_CMD_IDLE);
    
    //����������д��fifo
    RC663_WriteFIFO(g_sRc663Frame.frame, g_sRc663Frame.txLen);
    
    //����д��CMDReg����Զ�����
    RC663_WriteReg(RC663_REG_COMMAND, g_sRc663Frame.cmd & RC663_MASK_COMMAND);

    return RC663_STAT_OK;//RC663_Command();//
}

u8 RC663_TopazCommand(void)
{
    u8 pos = 0;
    u8 state = RC663_STAT_OK;
    u8 reg = 0;
    u32 tick = 0;

    reg = RC663_ReadReg(RC663_REG_FRAMECON);
    RC663_WriteReg(RC663_REG_FRAMECON, reg & (~RC663_BIT_TXPARITYEN));

    //��ֹ��ǰ���в���
    RC663_WriteReg(RC663_REG_COMMAND, RC663_CMD_IDLE);
    //���FIFO
    RC663_FlushFifo();


    RC663_Config(FALSE, FALSE, FALSE, 7, 0);
    RC663_WriteFIFO(g_sRc663Frame.frame, 1);
    RC663_ClearAllIrq0Flag();
    RC663_WriteReg(RC663_REG_COMMAND, RC663_CMD_TRANSMIT);
    tick = 10; //106k, 9.44us/bit, 94.4us/byte, tick = 10us
    while(tick--)
    {
        reg = RC663_ReadReg(RC663_REG_IRQ0);
        if(reg & RC663_BIT_TXIRQ)
        {
            break;
        }
    }

    if(tick == 0)
    {
        state = RC663_STAT_TIMEOUT;
    }
    else
    {
        RC663_Config(FALSE, FALSE, FALSE, 0, 0);
        for(pos = 1; pos < g_sRc663Frame.txLen - 1; pos++)
        {
            //��ֹ��ǰ���в���
            RC663_WriteReg(RC663_REG_COMMAND, RC663_CMD_IDLE);
            //���FIFO
            RC663_FlushFifo();
            RC663_WriteFIFO(g_sRc663Frame.frame + pos, 1);
            RC663_ClearAllIrq0Flag();
            RC663_WriteReg(RC663_REG_COMMAND, RC663_CMD_TRANSMIT);
            tick = 10; //106k, 9.44us/bit, 94.4us/byte, tick = 10us
            while(tick--)
            {
                reg = RC663_ReadReg(RC663_REG_IRQ0);
                if(reg & RC663_BIT_TXIRQ)
                {
                    break;
                }
            }
            if(tick == 0)
            {
                state = RC663_STAT_TIMEOUT;
            }
        }
    }

    if(state == RC663_STAT_OK)
    {
        g_sRc663Frame.frame[0] = g_sRc663Frame.frame[pos];
        g_sRc663Frame.txLen = 1;
        g_sRc663Frame.cmd = RC663_CMD_TRANSCEIVE;
        state = RC663_Command();
    }
    //��ԭ
    reg = RC663_ReadReg(RC663_REG_FRAMECON);
    RC663_WriteReg(RC663_REG_FRAMECON, reg | RC663_BIT_TXPARITYEN);

    return state;
}

u8 RC663_Command(void)
{
    u8 state = RC663_STAT_OK;
    u32 tick = 0;
    u8 reg = 0;
    u8 fifoLevel = 0;

    g_sRc663Frame.rxLen = 0;
    

    //��ֹ��ǰ���в���
    RC663_WriteReg(RC663_REG_COMMAND, RC663_CMD_IDLE);

    RC663_FlushFifo();
   
    RC663_WriteFIFO(g_sRc663Frame.frame, g_sRc663Frame.txLen);

    RC663_WriteReg(RC663_REG_IRQ0EN, RC663_BIT_IDLEIRQ | RC663_BIT_TXIRQ | RC663_BIT_RXIRQ | RC663_BIT_ERRIRQ | RC663_BIT_SOFIRQ);
    //��������sofDetected��subc_Detected�Ĵ�����գ�����Ӱ��SOFIRQ��־
    RC663_WriteReg(RC663_REG_RXSOFD, RC663_BIT_SOF_EN);
    //��������жϱ�־λ
    RC663_ClearAllIrq0Flag();
    //����д��CMDReg����Զ�����
    RC663_WriteReg(RC663_REG_COMMAND, g_sRc663Frame.cmd & RC663_MASK_COMMAND);
    
    tick = RC663_TX_FRAME_TIMEOUT;
    while(tick) //����
    {
        RC663_Delay16us(1);
        //�����ʱ������10us
        reg = RC663_ReadReg(RC663_REG_IRQ0);
        if(reg & RC663_BIT_ERRIRQ)
        {
            reg = RC663_ReadReg(RC663_REG_ERROR);  //0x08 NoDataErr
            state = RC663_STAT_ERROR;
            if(reg & RC663_STAT_COLLISION_ERROR) //������ײ����
            {
                state = RC663_STAT_COLLISION_ERROR;
            }
            g_sRc663Frame.rxLen = 0;
            
            break;
        }
        else if(reg & RC663_BIT_TXIRQ)
        {
            break;
        }
        tick--;
    }
    //�����Ҫ���գ����ж�SOF��־
    if(state == RC663_STAT_OK && tick > 0 && (g_sRc663Frame.cmd != RC663_CMD_TRANSMIT))
    {
        tick = g_sRc663Frame.timeout >> 4;  //ע������Ҫ��16��Э�鶼����us��Ϊ��λ
        while(tick)
        {
            RC663_Delay16us(1);
            //�����ʱ������10us
            reg = RC663_ReadReg(RC663_REG_IRQ0);
            if(reg & RC663_BIT_ERRIRQ)              //��������
            {
                reg = RC663_ReadReg(RC663_REG_ERROR);  //0x08 NoDataErr
                state = RC663_STAT_ERROR;
                if(reg & RC663_STAT_COLLISION_ERROR) //������ײ����
                {
                    state = RC663_STAT_COLLISION_ERROR;
                }
                g_sRc663Frame.rxLen = 0;
                break;
            }
            else if(reg & RC663_BIT_SOFIRQ)         //���յ�SOF
            {
                g_sRc663Frame.rxLen = 0;
                break;
            }
            else if(reg & RC663_BIT_RXIRQ)          //�������
            {
                break;
            }
            tick--;
        }
    }
    //��������
    if(state == RC663_STAT_OK && tick > 0 && (g_sRc663Frame.cmd != RC663_CMD_TRANSMIT))
    {
        tick = RC663_RX_FRAME_TIMEOUT;
        while(tick)
        {
            RC663_Delay16us(1);
            //�����ʱ������10us
            reg = RC663_ReadReg(RC663_REG_IRQ0);
            if(reg & RC663_BIT_ERRIRQ)
            {
                reg = RC663_ReadReg(RC663_REG_ERROR);  //0x08 NoDataErr
                state = RC663_STAT_ERROR;
                if(reg & RC663_STAT_COLLISION_ERROR) //������ײ����
                {
                    state = RC663_STAT_COLLISION_ERROR;
                }
                g_sRc663Frame.rxLen = 0;
                break;
            }
            else if(reg & RC663_BIT_RXIRQ)
            {
                fifoLevel = RC663_ReadReg(RC663_REG_FIFOLENGTH);
                RC663_Delay16us(50);
                RC663_ReadFIFO(g_sRc663Frame.frame + g_sRc663Frame.rxLen, fifoLevel);
                g_sRc663Frame.rxLen += fifoLevel;
                break;
            }
            else if(reg & RC663_BIT_IDLEIRQ)
            {
                fifoLevel = RC663_ReadReg(RC663_REG_FIFOLENGTH);
                RC663_ReadFIFO(g_sRc663Frame.frame + g_sRc663Frame.rxLen, fifoLevel);
                g_sRc663Frame.rxLen += fifoLevel;
                break;
            }
            tick--;
        }
    }

    //��ֹ��ǰ���в���
    RC663_WriteReg(RC663_REG_COMMAND, RC663_CMD_IDLE);

    if(tick == 0)
    {
        state = RC663_STAT_TIMEOUT;
    }
    else if(state == RC663_STAT_OK)
    {
        if(g_sRc663Frame.rxLen == 0)
        {
            state = RC663_STAT_NAK;
        }
    }

    return state;
}

#define RC663_PRODUCTID_LEN         3
u8 RC663_ReadProductID(void)
{
    //256�ֽڡ����FIFO��waterLevel8bit��fifoLength8bit
    RC663_WriteReg(RC663_REG_FIFOCONTROL, RC663_BIT_FIFOSIZE | RC663_BIT_FLUSHFIFO);
    
    g_sRc663Frame.frame[0] = 0;
    g_sRc663Frame.frame[1] = 0;
    g_sRc663Frame.frame[2] = RC663_PRODUCTID_LEN;
    RC663_WriteFIFO(g_sRc663Frame.frame, RC663_PRODUCTID_LEN);

    RC663_WriteReg(RC663_REG_COMMAND, RC663_CMD_READE2);
    RC663_Delay16us(100);
    g_sRc663Frame.rxLen = RC663_ReadReg(RC663_REG_FIFOLENGTH);
    if(g_sRc663Frame.rxLen == RC663_PRODUCTID_LEN)
    {
        RC663_ReadFIFO(g_sRc663Frame.frame, RC663_PRODUCTID_LEN);
        return g_sRc663Frame.frame[1];
    }
    else
    {
        return 0xFF;
    }
}