#include "AnyID_ST95HF.h"

void ST95HF_WriteFrame(ST95HF_FRAME *pFrame)
{
    u8 i = 0;

    ST95HF_CS_Low();

    //send control
    ST95HF_WriteByte(ST95HF_COMM_CMD_WRITE);

    //����
    ST95HF_WriteByte(pFrame->parms.cmd);
    ST95HF_WriteByte(pFrame->len);
    for(i = 0; i < pFrame->len; i++)
    {
        ST95HF_WriteByte(pFrame->frame[i]);
    }

    ST95HF_CS_High();
}

u8 ST95HF_Polling(u8 flag, u32 timeout)
{
    u8 reg = 0;
    u32 delay = 0;
    u8 rlt;

    flag = flag & (ST95HF_POLLING_WRITE_FLAG | ST95HF_POLLING_READ_FLAG);

    timeout = timeout >> 3; //ÿ����Ҫ10usʱ��
    if(timeout == 0)
    {
        timeout = 2;
    }

    //��ISO14443AЭ���У���һ��FDT�趨��ʱ�䣬Ӧ�ó�����ʱ��ʱ��timeoutһ��Ҫ����FDTֵ
    //��������ʹ����ʱʱ�����10,���Ǽ��㻹����10us����
    //�Ϳ��Ա�֤FDT<timeout
    ST95HF_CS_Low();
    while(((reg & flag) == 0x00) && (delay < timeout))//
    {
        reg = ST95HF_WriteByte(ST95HF_COMM_CMD_POLLING);        //�����1.1M��ʱ�ӽ���ͨ�ţ��ò�������10usʱ�䲻��
        delay++;
        //ST95HF_SpecDelay10us();                                 //���ﲹ����ʱʹ��ʱ��ʱ�䳬��10usһ�㣬��Լ��12.5us
    }
    ST95HF_CS_High();

    if(delay == timeout)
    {
        rlt = ST95HF_ERR_TIMEOUT;
    }
    else
    {
        rlt = ST95HF_ERR_SUCCESS;
    }

    return rlt;
}

u8 ST95HF_ReadFrame(ST95HF_FRAME *pFrame)
{
    u8 i = 0;
    u8 state = 0;

    state = ST95HF_ERR_SUCCESS;

    ST95HF_CS_Low();

    ST95HF_WriteByte(ST95HF_COMM_CMD_READ);
    pFrame->parms.code = ST95HF_WriteByte(ST95HF_DUMMY_BYTE);
    pFrame->len = ST95HF_WriteByte(ST95HF_DUMMY_BYTE);
    if(pFrame->len > ST95HF_FRAME_LEN || pFrame->len == 0)
    {
        state = ST95HF_ERR_LENGTH;
    }
    else
    {
        for(i = 0; i < pFrame->len; i++)
        {
            pFrame->frame[i] = ST95HF_WriteByte(ST95HF_DUMMY_BYTE);
        }
    }
    ST95HF_CS_High();

    return state;
}
void ST95HF_Reset(void)
{
    //��λST95HF
    ST95HF_CS_Low();

    //send control
    ST95HF_WriteByte(ST95HF_COMM_CMD_RESET);

    ST95HF_CS_High();

    //ѡ��SPI�ӿ�
    ST95HF_IRQIN_High();
    ST95HF_Delayms(1);   //1ms

    ST95HF_IRQIN_Low();
    ST95HF_Delayms(1);   //1ms

    ST95HF_IRQIN_High();
    ST95HF_Delayms(10);  //10ms
}

void ST95HF_CloseRf(ST95HF_FRAME *pFrame)
{
    pFrame->parms.cmd = ST95HF_CMD_SEL_PROTOCOL;
    pFrame->len = 0x02;
    pFrame->frame[0] = 0x00;
    pFrame->frame[1] = 0x00;
    pFrame->to = ST95HF_POLLING_DELAY;
    ST95HF_Command(&g_sST95HFFrame);
}

void ST95HF_OpenRf(ST95HF_FRAME *pFrame)
{
    pFrame->parms.cmd = ST95HF_CMD_SEL_PROTOCOL;
    pFrame->len = 0x02;
    pFrame->frame[0] = 0x01;
    pFrame->frame[1] = 0x00;
    pFrame->to = ST95HF_POLLING_DELAY;
    ST95HF_Command(&g_sST95HFFrame);
}

u8 ST95HF_Receive(ST95HF_FRAME *pFrame)
{
    u8 err = 0x00;

    err = ST95HF_Polling(ST95HF_POLLING_READ_FLAG, pFrame->to);
    if(err == ST95HF_ERR_SUCCESS)
    {
        err = ST95HF_ReadFrame(pFrame);
        if(err != ST95HF_ERR_SUCCESS)
        {
            err = pFrame->parms.code;
        }
    }

    return err;
}

u8 ST95HF_Transmit(ST95HF_FRAME *pFrame)
{
    u8 err = 0x00;

    err = ST95HF_Polling(ST95HF_POLLING_WRITE_FLAG, pFrame->to);
    if(err == ST95HF_ERR_SUCCESS)
    {
        ST95HF_WriteFrame(pFrame);
    }

    return err;
}

u8 ST95HF_Command(ST95HF_FRAME *pFrame)
{
    u8 err = 0x00;

    err = ST95HF_Polling(ST95HF_POLLING_READ_FLAG, 0);
    if(err == ST95HF_ERR_SUCCESS)
    {
        //��ȡ�ϴ�û�ж�ȡ���������ݣ������ϴγ�����
        u8 len = 0;
        u8 i = 0;

        ST95HF_CS_Low();

        ST95HF_WriteByte(ST95HF_COMM_CMD_READ);
        ST95HF_WriteByte(ST95HF_DUMMY_BYTE);// rspcode
        len = ST95HF_WriteByte(ST95HF_DUMMY_BYTE);
        for(i = 0; i < len; i++)
        {
            ST95HF_WriteByte(ST95HF_DUMMY_BYTE);
        }
        ST95HF_CS_High();
    }

    err = ST95HF_Polling(ST95HF_POLLING_WRITE_FLAG, pFrame->to);
    if(err == ST95HF_ERR_SUCCESS)
    {
        ST95HF_WriteFrame(pFrame);
        err = ST95HF_Polling(ST95HF_POLLING_READ_FLAG, pFrame->to);
        if(err == ST95HF_ERR_SUCCESS)
        {
            err = ST95HF_ReadFrame(pFrame);
            if(err != ST95HF_ERR_SUCCESS)
            {
                err = pFrame->parms.code;
            }
        }
    }

    return err;
}

u8 ST95HF_DeviceCheckResponse(ST95HF_FRAME *pFrame)
{
    u8 state = ST95HF_ERR_RSPCODE;
    if(pFrame->parms.code == ST95HF_RSP_CODE_OK)
    {
        state = ST95HF_ERR_SUCCESS;
    }

    return state;
}


u8 ST95HF_Iso14443ACheckResponse(ST95HF_FRAME *pFrame)
{
    u8 flag = 0;
    u8 state = ST95HF_ERR_SUCCESS;

    if(pFrame->parms.code == ST95HF_SENDRCV_RSPCODE_DAT)
    {
        if(g_sST95HFFrame.len >= ST95HF_SENDRCV_14443A_RSPTRIL_LEN)
        {
            flag = g_sST95HFFrame.frame[g_sST95HFFrame.len - ST95HF_SENDRCV_14443A_RSPTRIL_LEN];
            if(flag & ST95HF_SENDRCV_14443A_ERR_COL)
            {
                state = ST95HF_ERR_COLL;
            }
            else if(flag & ST95HF_SENDRCV_14443A_ERR_CRC)
            {
                state = ST95HF_ERR_CRC;
            }
            else if(flag & ST95HF_SENDRCV_14443A_ERR_PRITY)
            {
                state = ST95HF_ERR_PARITY;
            }
            else
            {
                state = ST95HF_ERR_SUCCESS;
            }
        }
        else
        {
            state = ST95HF_ERR_LENGTH;
        }
    }
    else if(pFrame->parms.code == ST95HF_SENDRCV_RSPCODE_ERR)
    {
        state = ST95HF_ERR_RSPCODE;
    }
    else
    {
        state = pFrame->parms.code;
    }

    return state;
}


u8 ST95HF_Iso15693CheckResponse(ST95HF_FRAME *pFrame)
{
    u8 flag = 0;
    u8 state = ST95HF_ERR_SUCCESS;

    if(pFrame->parms.code == ST95HF_SENDRCV_RSPCODE_DAT)
    {
        if(pFrame->len > 0)
        {
            //���һ���ֽڱ�ʾ���յ�����֡�Ƿ���Ч
            flag = pFrame->frame[pFrame->len - 1];
            //��ͻλ�е����⣬��ʱ��������ȷ��������ʵ��ͻλ
            if(flag & ST95HF_SENDRCV_15693_ERR_COL)
            {
                state = ST95HF_ERR_15693_COL;
            }
            else if(flag & ST95HF_SENDRCV_15693_ERR_CRC)
            {
                state = ST95HF_ERR_CRC;
            }
            else
            {
                u8 vccRspFlag = 0;
                vccRspFlag = pFrame->frame[0x00];
                if(vccRspFlag & 0x01)
                {
                    state = ST95HF_ERR_TAG;
                }
            }
        }
        else
        {
            state = ST95HF_ERR_LENGTH;
        }
    }
    else if(pFrame->parms.code == ST95HF_SENDRCV_RSPCODE_ERR)
    {
        state = ST95HF_ERR_RSPCODE;
    }
    else
    {
        state = pFrame->parms.code;
    }

    return state;
}

u8 ST95HF_Iso14443BCheckResponse(ST95HF_FRAME *pFrame)
{
    u8 flag = 0;
    u8 state = ST95HF_ERR_SUCCESS;

    if(pFrame->parms.code == ST95HF_SENDRCV_RSPCODE_DAT)
    {
        if(pFrame->len > 0)
        {
            //���һ���ֽڱ�ʾ���յ�����֡�Ƿ���Ч
            flag = pFrame->frame[pFrame->len - 1];
            //��ͻλ�е����⣬��ʱ��������ȷ��������ʵ��ͻλ
            if(flag & ST95HF_SENDRCV_14443B_ERR_CRC)
            {
                state = ST95HF_ERR_CRC;
            }
        }
        else
        {
            state = ST95HF_ERR_LENGTH;
        }
    }
    else if(pFrame->parms.code == ST95HF_SENDRCV_RSPCODE_ERR)
    {
        state = ST95HF_ERR_RSPCODE;
    }
    else
    {
        state = pFrame->parms.code;
    }

    return state;
}

u8 ST95HF_Iso18092CheckResponse(ST95HF_FRAME *pFrame)
{
    u8 flag = 0;
    u8 state = ST95HF_ERR_SUCCESS;

    if(pFrame->parms.code == ST95HF_SENDRCV_RSPCODE_DAT)
    {
        if(pFrame->len > 0)
        {
            //���һ���ֽڱ�ʾ���յ�����֡�Ƿ���Ч
            flag = pFrame->frame[pFrame->len - 1];
            //��ͻλ�е����⣬��ʱ��������ȷ��������ʵ��ͻλ
            if(flag & ST95HF_SENDRCV_18092_ERR_CRC)
            {
                state = ST95HF_ERR_CRC;
            }
        }
        else
        {
            state = ST95HF_ERR_LENGTH;
        }
    }
    else if(pFrame->parms.code == ST95HF_SENDRCV_RSPCODE_ERR)
    {
        state = ST95HF_ERR_RSPCODE;
    }
    else
    {
        state = pFrame->parms.code;
    }

    return state;
}



