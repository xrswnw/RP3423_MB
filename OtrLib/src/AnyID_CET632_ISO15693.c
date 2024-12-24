#include "AnyID_CET632_ISO15693.h"
u8 ISO15693_Init(void)
{
    u8 reg = 0;

    //���Ϳ���
    //�ڲ����룬100%���ƣ�TX1���������TX2���������TX1��TX2��ʹ��
    reg = CET632_BIT_MODSRC_IN | CET632_BIT_100ASK | CET632_BIT_TX2INV | CET632_BIT_TX1RFEN | CET632_BIT_TX2RFEN;
    CET632_WriteReg(CET632_REG_TXCTRL, reg);
    //�ߵ�ƽ��RF�������
    CET632_WriteReg(CET632_REG_TXCW, 0x3F);
    //ISO15693 + 4ȡ1����
    reg = CET632_BIT_ISO15693K | CET632_BIT_1P4;
    CET632_WriteReg(CET632_REG_CODERCTRL, reg);
    //Tmod = 2(modWidth + 1)/fc
    //ISO15693���ƿ����9.44us��sof���ƿ��Ҳ��9.44us
    //26K
    CET632_WriteReg(CET632_REG_MODWIDTH, 0x3F);
    CET632_WriteReg(CET632_REG_MODWSOF, 0x3F);

    //���տ���
    //��������26K
    reg = CET632_ReadReg(CET632_REG_RXCTRL1);
    reg &= 0x07;    //����Gain��lp_off�Ŀ���;
    reg |= (CET632_BIT_SUB16PLS | CET632_BIT_SUB32CLK);
    CET632_WriteReg(CET632_REG_RXCTRL1, reg);
    //���ճ�ͻλ���ó�0����ͻ�������ȫ0��rxFrame��ISO15693�����ս�������˹�ر���(ISO15693)
    reg = CET632_BIT_COLL_ZERO | CET632_BIT_RXFRAM_15693 | CET632_BIT_RXCODE_MCST;
    CET632_WriteReg(CET632_REG_DECODECTRL, reg);
    //����TXEOF��RXSOF֮�����λ��ϵ
    CET632_WriteReg(CET632_REG_BITPHASE, 0x40);
    //��������
    CET632_WriteReg(CET632_REG_RXTHLD, 0xAA);
    //AGCʹ��
    CET632_WriteReg(CET632_REG_BPSKCTRL, CET632_BIT_AGCEN);
    //������ʱ����������EOF��ȴ�һ��ʱ���ٿ������գ����������ڿ�����
    CET632_WriteReg(CET632_REG_RXWAIT, 0x08);
    //����Э���׼����CRC�����պͷ��Ͷ�����Я��CRC
    reg = CET632_BIT_CRC3309 | CET632_BIT_CRCRXEN | CET632_BIT_CRCTXEN;
    CET632_WriteReg(CET632_REG_CRCCTRL, reg);
    //ISO15693��CRCУ���ʼֵ0xFFFF���ڲ�����CRC
    CET632_WriteReg(CET632_REG_CRCPRESETLSB, 0xFF);
    CET632_WriteReg(CET632_REG_CRCPRESETMSB, 0xFF);

    //Timer
    //�����ʱ���Ĺ������ޣ�һ���Լ�ʹ�ö�ʱ��
    CET632_WriteReg(CET632_REG_TIMERCLK, 0x1F);
    CET632_WriteReg(CET632_REG_TIMERRELOAD, 0xFF);

    //��ղ��ԼĴ���
    CET632_WriteReg(CET632_REG_TESTANT, 0x00);
    CET632_WriteReg(CET632_REG_ALGADJ2, 0x00);

    //fifo 32
    CET632_WriteReg(CET632_REG_FIFOLV, CET632_FIFO_LEVEL);

    return TRUE;

}


u8 ISO15693_Inventory(u8 afiCtrl, u8 afiValue, u8 *pUid)
{
    u8 error = CET632_ERR_SUCCESS;
    u8 num = 0;
    u8 flag = 0;
    u8 txLen = 0;
    u8 rxLen = 0;

    a_SetStateBit(flag, ISO15693_REQ_FLAG_INVENTORY);   // inventory command
    a_SetStateBit(flag, ISO15693_REQ_FLAG_SLOTS_1);
    a_SetStateBit(flag, ISO15693_REQ_FLAG_HRATE);       // hight data rate is used
    if(afiCtrl)
    {
        a_SetStateBit(flag, ISO15693_REQ_FLAG_AFI);   //
    }

    g_sCET632Frame.txFrame[txLen++] = flag;
    g_sCET632Frame.txFrame[txLen++] = ISO15693_CMD_INVENTORY;
    if(afiCtrl)
    {
        g_sCET632Frame.txFrame[txLen++] = afiValue;
    }
    g_sCET632Frame.txFrame[txLen++] = 0x00;

    error = CET632_Command(CET632_CMD_TRANCEIVE, g_sCET632Frame.txFrame, txLen, g_sCET632Frame.rxFrame, &rxLen, ISO15693_INVENTORY_TIME);
    if(error == CET632_ERR_SUCCESS)
    {
        u8 reg = 0;
        reg = CET632_ReadReg(CET632_REG_ERR);
        if(reg & (CET632_BIT_COLLERR | CET632_BIT_FRAMEERR | CET632_BIT_CRCERR))
        {
            error = CET632_ERR_COLL;
        }
        else
        {
            u8 rcvFlag = 0;
            rcvFlag = g_sCET632Frame.rxFrame[ISO15693_RSP_FRAME_POS_FLAG];
            if(rcvFlag == 0 && rxLen == 0x0A)
            {
                a_memcpy(g_sCET632Frame.rxFrame + 2, pUid + num * ISO15693_SIZE_UID, ISO15693_SIZE_UID);
                num++;
            }
            else
            {
                error = CET632_ERR_NAK;
            }
        }
    }
    return num;
}
u8 ISO15693_StayQuiet(u8 *pUID)
{
    u8 flag = 0;
    //u8 buffer[ISO15693_MAX_FRAME_LEN] = {0};
    u8 txLen = 0;

    a_SetStateBit(flag, ISO15693_REQ_FLAG_ADDRM);       // inventory command
    a_SetStateBit(flag, ISO15693_REQ_FLAG_HRATE);       // hight data rate is used

    //frame
    g_sCET632Frame.txFrame[txLen++] = flag;
    g_sCET632Frame.txFrame[txLen++] = ISO15693_CMD_STAYQUIET;
    a_memcpy(pUID, g_sCET632Frame.txFrame + txLen, ISO15693_SIZE_UID);
    txLen += ISO15693_SIZE_UID;

    //������û����Ӧ֡
    CET632_Command(CET632_CMD_TRANCEIVE, g_sCET632Frame.txFrame, txLen, 0, 0, ISO15693_INVENTORY_TIME);

    return CET632_ERR_SUCCESS;
}

u8 ISO15693_ReadBlock(u8 *pUID, u8 num, u8 addr, u8 *pData)
{
    u8 error = CET632_ERR_SUCCESS;;
    u8 flag = 0;
    //u8 buffer[ISO15693_MAX_FRAME_LEN] = {0};
    u8 txLen = 0;
    u8 rxLen = 0;
    u8 i = 0;

    a_SetStateBit(flag, ISO15693_REQ_FLAG_ADDRM);       // inventory command
    a_SetStateBit(flag, ISO15693_REQ_FLAG_HRATE);       // hight data rate is used

    for(i = 0; i < num; i++)
    {
        //frame
        txLen = 0;
        rxLen = 0;
        g_sCET632Frame.txFrame[txLen++] = flag;
        g_sCET632Frame.txFrame[txLen++] = ISO15693_CMD_READSINGLEBLOCK;
        a_memcpy(pUID, g_sCET632Frame.txFrame + txLen, ISO15693_SIZE_UID);
        txLen += ISO15693_SIZE_UID;
        g_sCET632Frame.txFrame[txLen++] = addr + i;
        //������û����Ӧ֡
        error = CET632_Command(CET632_CMD_TRANCEIVE, g_sCET632Frame.txFrame, txLen, g_sCET632Frame.rxFrame, &rxLen, ISO15693_READ_TIME);
        if(error == CET632_ERR_SUCCESS)
        {
            u8 reg = 0;
            reg = CET632_ReadReg(CET632_REG_ERR) & (~CET632_BIT_KEYERR);
            if(reg)
            {
                error = CET632_CheckErr(reg);
                break;
            }
            else
            {
                u8 rcvFlag = 0;
                rcvFlag = g_sCET632Frame.rxFrame[ISO15693_RSP_FRAME_POS_FLAG];
                if(rcvFlag == 0)
                {
                    a_memcpy(g_sCET632Frame.rxFrame + ISO15693_RSP_FRAME_POS_PAR, pData + i * ISO15693_SIZE_BLOCK, ISO15693_SIZE_BLOCK);
                    CET632_Delay100us(10); //����FWTʱ�䣬������Щ��ǩ����������
                }
                else
                {
                    error = CET632_ERR_NAK;
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }

    return error;
}

u8 ISO15693_WriteBlock(u8 *pUID, u8 num, u8 addr, u8 *pData)
{
    u8 error = CET632_ERR_SUCCESS;;
    u8 flag = 0;
    //u8 buffer[ISO15693_MAX_FRAME_LEN] = {0};
    u8 txLen = 0;
    u8 rxLen = 0;
    u8 i = 0;

    a_SetStateBit(flag, ISO15693_REQ_FLAG_ADDRM);       // inventory command
    a_SetStateBit(flag, ISO15693_REQ_FLAG_HRATE);       // hight data rate is used
    if(pUID[ISO15693_UID_TYPE_POS] == ISO15693_TAG_TYPE_TI)
    {
        a_SetStateBit(flag, ISO15693_REQ_FLAG_OPT);       // addr mode
    }

    for(i = 0; i < num; i++)
    {
        //frame
        txLen = 0;
        rxLen = 0;
        g_sCET632Frame.txFrame[txLen++] = flag;
        g_sCET632Frame.txFrame[txLen++] = ISO15693_CMD_WRITESINGLEBLOCK;
        a_memcpy(pUID, g_sCET632Frame.txFrame + txLen, ISO15693_SIZE_UID);
        txLen += ISO15693_SIZE_UID;
        g_sCET632Frame.txFrame[txLen++] = addr + i;
        a_memcpy(pData + i * ISO15693_SIZE_BLOCK, g_sCET632Frame.txFrame + txLen, ISO15693_SIZE_BLOCK);
        txLen += ISO15693_SIZE_BLOCK;
        //������û����Ӧ֡
        error = CET632_Command(CET632_CMD_TRANCEIVE, g_sCET632Frame.txFrame, txLen, g_sCET632Frame.rxFrame, &rxLen, ISO15693_WRITE_TIME);
        if(a_CheckStateBit(flag, ISO15693_REQ_FLAG_OPT))
        {
            CET632_SetRegBit(CET632_REG_CODERCTRL, CET632_BIT_SENDEOF);
            error = CET632_Command(CET632_CMD_TRANCEIVE, 0, 0, g_sCET632Frame.rxFrame, &rxLen, ISO15693_INVENTORY_TIME);
            CET632_ClearRegBit(CET632_REG_CODERCTRL, CET632_BIT_SENDEOF);
        }
        if(error == CET632_ERR_SUCCESS)
        {
            u8 reg = 0;
            reg = CET632_ReadReg(CET632_REG_ERR) & (~CET632_BIT_KEYERR);
            if(reg)
            {
                error = CET632_CheckErr(reg);
                break;
            }
            else
            {
                u8 rcvFlag = 0;
                rcvFlag = g_sCET632Frame.rxFrame[ISO15693_RSP_FRAME_POS_FLAG];
                if(rcvFlag != 0)
                {
                    error = CET632_ERR_NAK;
                    break;
                }
                CET632_Delay100us(10); //����FWTʱ�䣬������Щ��ǩ����������
            }
        }
    }

    return error;
}

u8 ISO15693_LockBlock(u8 *pUID, u8 addr)
{
    u8 error = CET632_ERR_SUCCESS;;
    u8 flag = 0;
    //u8 buffer[ISO15693_MAX_FRAME_LEN] = {0};
    u8 txLen = 0;
    u8 rxLen = 0;

    a_SetStateBit(flag, ISO15693_REQ_FLAG_ADDRM);       // inventory command
    a_SetStateBit(flag, ISO15693_REQ_FLAG_HRATE);       // hight data rate is used
    if(pUID[ISO15693_UID_TYPE_POS] == ISO15693_TAG_TYPE_TI)
    {
        a_SetStateBit(flag, ISO15693_REQ_FLAG_OPT);       // addr mode
    }

    //frame
    g_sCET632Frame.txFrame[txLen++] = flag;
    g_sCET632Frame.txFrame[txLen++] = ISO15693_CMD_LOCKBLOCK;
    a_memcpy(pUID, g_sCET632Frame.txFrame + txLen, ISO15693_SIZE_UID);
    txLen += ISO15693_SIZE_UID;
    g_sCET632Frame.txFrame[txLen++] = addr;
    //������û����Ӧ֡
    error = CET632_Command(CET632_CMD_TRANCEIVE, g_sCET632Frame.txFrame, txLen, g_sCET632Frame.rxFrame, &rxLen, ISO15693_WRITE_TIME);
    if(a_CheckStateBit(flag, ISO15693_REQ_FLAG_OPT))
    {
        CET632_SetRegBit(CET632_REG_CODERCTRL, CET632_BIT_SENDEOF);
        error = CET632_Command(CET632_CMD_TRANCEIVE, 0, 0, g_sCET632Frame.rxFrame, &rxLen, ISO15693_INVENTORY_TIME);
        CET632_ClearRegBit(CET632_REG_CODERCTRL, CET632_BIT_SENDEOF);
    }
    if(error == CET632_ERR_SUCCESS)
    {
        u8 reg = 0;
        reg = CET632_ReadReg(CET632_REG_ERR) & (~CET632_BIT_KEYERR);
        if(reg)
        {
            error = CET632_CheckErr(reg);
        }
        else
        {
            u8 rcvFlag = 0;
            rcvFlag = g_sCET632Frame.rxFrame[ISO15693_RSP_FRAME_POS_FLAG];
            if(rcvFlag != 0)
            {
                error = CET632_ERR_NAK;
            }
        }
    }

    return error;
}

u8 ISO15693_WriteAFI(u8 *pUID, u8 afi)
{
    u8 error = CET632_ERR_SUCCESS;;
    u8 flag = 0;
    //u8 buffer[ISO15693_MAX_FRAME_LEN] = {0};
    u8 txLen = 0;
    u8 rxLen = 0;

    a_SetStateBit(flag, ISO15693_REQ_FLAG_ADDRM);       // inventory command
    a_SetStateBit(flag, ISO15693_REQ_FLAG_HRATE);       // hight data rate is used
    if(pUID[ISO15693_UID_TYPE_POS] == ISO15693_TAG_TYPE_TI)
    {
        a_SetStateBit(flag, ISO15693_REQ_FLAG_OPT);       // addr mode
    }

    //frame
    g_sCET632Frame.txFrame[txLen++] = flag;
    g_sCET632Frame.txFrame[txLen++] = ISO15693_CMD_WRITEAFI;
    a_memcpy(pUID, g_sCET632Frame.txFrame + txLen, ISO15693_SIZE_UID);
    txLen += ISO15693_SIZE_UID;
    g_sCET632Frame.txFrame[txLen++] = afi;

    //������û����Ӧ֡
    error = CET632_Command(CET632_CMD_TRANCEIVE, g_sCET632Frame.txFrame, txLen, g_sCET632Frame.rxFrame, &rxLen, ISO15693_WRITE_TIME);
    if(a_CheckStateBit(flag, ISO15693_REQ_FLAG_OPT))
    {
        CET632_SetRegBit(CET632_REG_CODERCTRL, CET632_BIT_SENDEOF);
        error = CET632_Command(CET632_CMD_TRANCEIVE, 0, 0, g_sCET632Frame.rxFrame, &rxLen, ISO15693_INVENTORY_TIME);
        CET632_ClearRegBit(CET632_REG_CODERCTRL, CET632_BIT_SENDEOF);
    }
    if(error == CET632_ERR_SUCCESS)
    {
        u8 reg = 0;
        reg = CET632_ReadReg(CET632_REG_ERR) & (~CET632_BIT_KEYERR);
        if(reg)
        {
            error = CET632_CheckErr(reg);
        }
        else
        {
            u8 rcvFlag = 0;
            rcvFlag = g_sCET632Frame.rxFrame[ISO15693_RSP_FRAME_POS_FLAG];
            if(rcvFlag != 0)
            {
                error = CET632_ERR_NAK;
            }
        }
    }

    return error;
}

u8 ISO15693_LockAFI(u8 *pUID)
{
    u8 error = CET632_ERR_SUCCESS;;
    u8 flag = 0;
    //u8 buffer[ISO15693_MAX_FRAME_LEN] = {0};
    u8 txLen = 0;
    u8 rxLen = 0;

    a_SetStateBit(flag, ISO15693_REQ_FLAG_ADDRM);       // inventory command
    a_SetStateBit(flag, ISO15693_REQ_FLAG_HRATE);       // hight data rate is used
    if(pUID[ISO15693_UID_TYPE_POS] == ISO15693_TAG_TYPE_TI)
    {
        a_SetStateBit(flag, ISO15693_REQ_FLAG_OPT);       // addr mode
    }

    //frame
    g_sCET632Frame.txFrame[txLen++] = flag;
    g_sCET632Frame.txFrame[txLen++] = ISO15693_CMD_LOCKAFI;
    a_memcpy(pUID, g_sCET632Frame.txFrame + txLen, ISO15693_SIZE_UID);
    txLen += ISO15693_SIZE_UID;

    //������û����Ӧ֡
    error = CET632_Command(CET632_CMD_TRANCEIVE, g_sCET632Frame.txFrame, txLen, g_sCET632Frame.rxFrame, &rxLen, ISO15693_WRITE_TIME);
    if(a_CheckStateBit(flag, ISO15693_REQ_FLAG_OPT))
    {
        CET632_SetRegBit(CET632_REG_CODERCTRL, CET632_BIT_SENDEOF);
        error = CET632_Command(CET632_CMD_TRANCEIVE, 0, 0, g_sCET632Frame.rxFrame, &rxLen, ISO15693_INVENTORY_TIME);
        CET632_ClearRegBit(CET632_REG_CODERCTRL, CET632_BIT_SENDEOF);
    }
    if(error == CET632_ERR_SUCCESS)
    {
        u8 reg = 0;
        reg = CET632_ReadReg(CET632_REG_ERR) & (~CET632_BIT_KEYERR);
        if(reg)
        {
            error = CET632_CheckErr(reg);
        }
        else
        {
            u8 rcvFlag = 0;
            rcvFlag = g_sCET632Frame.rxFrame[ISO15693_RSP_FRAME_POS_FLAG];
            if(rcvFlag != 0)
            {
                error = CET632_ERR_NAK;
            }
        }
    }

    return error;
}

u8 ISO15693_WriteDSFID(u8 *pUID, u8 dsfid)
{
    u8 error = CET632_ERR_SUCCESS;;
    u8 flag = 0;
    //u8 buffer[ISO15693_MAX_FRAME_LEN] = {0};
    u8 txLen = 0;
    u8 rxLen = 0;

    a_SetStateBit(flag, ISO15693_REQ_FLAG_ADDRM);       // inventory command
    a_SetStateBit(flag, ISO15693_REQ_FLAG_HRATE);       // hight data rate is used
    if(pUID[ISO15693_UID_TYPE_POS] == ISO15693_TAG_TYPE_TI)
    {
        a_SetStateBit(flag, ISO15693_REQ_FLAG_OPT);       // addr mode
    }

    //frame
    g_sCET632Frame.txFrame[txLen++] = flag;
    g_sCET632Frame.txFrame[txLen++] = ISO15693_CMD_WRITEDSFID;
    a_memcpy(pUID, g_sCET632Frame.txFrame + txLen, ISO15693_SIZE_UID);
    txLen += ISO15693_SIZE_UID;
    g_sCET632Frame.txFrame[txLen++] = dsfid;
    //������û����Ӧ֡
    error = CET632_Command(CET632_CMD_TRANCEIVE, g_sCET632Frame.txFrame, txLen, g_sCET632Frame.rxFrame, &rxLen, ISO15693_WRITE_TIME);
    if(a_CheckStateBit(flag, ISO15693_REQ_FLAG_OPT))
    {
        CET632_SetRegBit(CET632_REG_CODERCTRL, CET632_BIT_SENDEOF);
        error = CET632_Command(CET632_CMD_TRANCEIVE, 0, 0, g_sCET632Frame.rxFrame, &rxLen, ISO15693_INVENTORY_TIME);
        CET632_ClearRegBit(CET632_REG_CODERCTRL, CET632_BIT_SENDEOF);
    }
    if(error == CET632_ERR_SUCCESS)
    {
        u8 reg = 0;
        reg = CET632_ReadReg(CET632_REG_ERR) & (~CET632_BIT_KEYERR);
        if(reg)
        {
            error = CET632_CheckErr(reg);
        }
        else
        {
            u8 rcvFlag = 0;
            rcvFlag = g_sCET632Frame.rxFrame[ISO15693_RSP_FRAME_POS_FLAG];
            if(rcvFlag != 0)
            {
                error = CET632_ERR_NAK;
            }
        }
    }

    return error;
}

u8 ISO15693_LockDSFID(u8 *pUID)
{
    u8 error = CET632_ERR_SUCCESS;;
    u8 flag = 0;
    //u8 buffer[ISO15693_MAX_FRAME_LEN] = {0};
    u8 txLen = 0;
    u8 rxLen = 0;

    a_SetStateBit(flag, ISO15693_REQ_FLAG_ADDRM);       // inventory command
    a_SetStateBit(flag, ISO15693_REQ_FLAG_HRATE);       // hight data rate is used
    if(pUID[ISO15693_UID_TYPE_POS] == ISO15693_TAG_TYPE_TI)
    {
        a_SetStateBit(flag, ISO15693_REQ_FLAG_OPT);       // addr mode
    }

    //frame
    g_sCET632Frame.txFrame[txLen++] = flag;
    g_sCET632Frame.txFrame[txLen++] = ISO15693_CMD_LOCKDSFID;
    a_memcpy(pUID, g_sCET632Frame.txFrame + txLen, ISO15693_SIZE_UID);
    txLen += ISO15693_SIZE_UID;

    //������û����Ӧ֡
    error = CET632_Command(CET632_CMD_TRANCEIVE, g_sCET632Frame.txFrame, txLen, g_sCET632Frame.rxFrame, &rxLen, ISO15693_WRITE_TIME);
    if(a_CheckStateBit(flag, ISO15693_REQ_FLAG_OPT))
    {
        CET632_SetRegBit(CET632_REG_CODERCTRL, CET632_BIT_SENDEOF);
        error = CET632_Command(CET632_CMD_TRANCEIVE, 0, 0, g_sCET632Frame.rxFrame, &rxLen, ISO15693_INVENTORY_TIME);
        CET632_ClearRegBit(CET632_REG_CODERCTRL, CET632_BIT_SENDEOF);
    }
    if(error == CET632_ERR_SUCCESS)
    {
        u8 reg = 0;
        reg = CET632_ReadReg(CET632_REG_ERR) & (~CET632_BIT_KEYERR);
        if(reg)
        {
            error = CET632_CheckErr(reg);
        }
        else
        {
            u8 rcvFlag = 0;
            rcvFlag = g_sCET632Frame.rxFrame[ISO15693_RSP_FRAME_POS_FLAG];
            if(rcvFlag != 0)
            {
                error = CET632_ERR_NAK;
            }
        }
    }

    return error;
}


u8 ISO15693_GetTagInf(u8 *pUID)
{
    u8 error = CET632_ERR_SUCCESS;
    u8 flag = 0;
    //u8 buffer[ISO15693_MAX_FRAME_LEN] = {0};
    u8 txLen = 0;
    u8 rxLen = 0;

    a_SetStateBit(flag, ISO15693_REQ_FLAG_ADDRM);       // inventory command
    a_SetStateBit(flag, ISO15693_REQ_FLAG_HRATE);       // hight data rate is used

    //frame
    g_sCET632Frame.txFrame[txLen++] = flag;
    g_sCET632Frame.txFrame[txLen++] = ISO15693_CMD_GETSYSINFO;
    a_memcpy(pUID, g_sCET632Frame.txFrame + txLen, ISO15693_SIZE_UID);
    txLen += ISO15693_SIZE_UID;

    //������û����Ӧ֡
    error = CET632_Command(CET632_CMD_TRANCEIVE, g_sCET632Frame.txFrame, txLen, g_sCET632Frame.rxFrame, &rxLen, ISO15693_READ_TIME);
    if(error == CET632_ERR_SUCCESS)
    {
        u8 reg = 0;
        reg = CET632_ReadReg(CET632_REG_ERR) & (~CET632_BIT_KEYERR);
        if(reg)
        {
            error = CET632_CheckErr(reg);
        }
        else
        {
            u8 rcvFlag = 0;
            rcvFlag = g_sCET632Frame.rxFrame[ISO15693_RSP_FRAME_POS_FLAG];
            if(rcvFlag != 0)
            {
                error = CET632_ERR_NAK;
            }
        }
    }

    return error;
}


u8 ISO15693_Dtu(u8 *pTxFrame, u8 txLen, u32 timeout)
{
    u8 error = CET632_ERR_SUCCESS;

    if(txLen > 0)
    {
        a_memcpy(pTxFrame, g_sCET632Frame.txFrame, txLen);
    }
    else
    {
        CET632_SetRegBit(CET632_REG_CODERCTRL, CET632_BIT_SENDEOF); //����EOF
    }


    //������û����Ӧ֡
    g_sCET632Frame.rxLen = 0;
    error = CET632_Command(CET632_CMD_TRANCEIVE, pTxFrame, txLen, g_sCET632Frame.rxFrame, &(g_sCET632Frame.rxLen), timeout);
    if(error == CET632_ERR_SUCCESS)
    {
        u8 reg = 0;
        reg = CET632_ReadReg(CET632_REG_ERR) & (~CET632_BIT_KEYERR);
        if(reg)
        {
            error = CET632_CheckErr(reg);
        }
    }

    return error;
}


