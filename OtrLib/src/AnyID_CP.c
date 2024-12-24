#include "AnyID_CP.h"


const u8 CP_Version[] = "AnyID_CP V1.1.2";

u16 g_nCPFrameIdleTime = 0;
u8 g_nCPFrameRcvFlag = CP_FRAME_FLAG_IDLE;
u16 g_nCPFrameRcvPos = 0;

u8 g_aCPRcvBuffer[CP_FRAME_BUF_MAX_LEN] = {0};

BOOL CP_CheckCrc(u8 *pFrame, u8 len)
{
    BOOL b = FALSE;
    u16 crc = 0x0000;

    //��Դ��ַ��ʼ���������������CRC
    crc = a_GetCrc(pFrame + CP_FRAME_SCRADDR_POS, len - CP_FRAME_TRAIL_LEN - 2);
    if(crc == (*(u16 *)(pFrame + len - CP_FRAME_TRAIL_LEN)))
    {
        b = TRUE;
    }

    return b;
}


void CP_Init(u16 scrAddr, u8 ctrl)
{
    /*CP_HEAD.uHead.SHEAD.head1 = 0x7E;
    CP_HEAD.uHead.SHEAD.head2 = 0x55;
    CP_SetScrAddr(scrAddr);
    CP_SetDesAddr(CP_FRAME_BROADCAST_ADDR);
    CP_HEAD.ctrl = ctrl;
    CP_HEAD.cmd = 0x00;
    CP_HEAD.state = 0x00;

    CP_TRAIL.trail = 0xAA;*/
}

u8 CP_GetFrame(u16 scrAddr, u16 desAddr, u8 ctrl, u8 cmd, u8 state, u8 *pFrame, u8 dataLen)
{
    u16 crc = 0x0000;
    u8 len = CP_FRAME_HEAD_LEN + dataLen + CP_FRAME_TRAIL_LEN;

    CP_FRAME_HEAD *pHead = NULL;
    CP_FRAME_TRAIL *pTrail = NULL;

    pHead = (CP_FRAME_HEAD *)(pFrame);
    pTrail = (CP_FRAME_TRAIL *)(pFrame + CP_FRAME_HEAD_LEN + dataLen);

    //֡ͷ
    pHead->uHead.sHead.head1 = 0x7E;
    pHead->uHead.sHead.head2 = 0x55;
    CP_SetScrAddr(pHead, scrAddr);
    CP_SetDesAddr(pHead, desAddr);
    pHead->ctrl = ctrl;
    pHead->cmd = cmd;
    pHead->state = state;
    pHead->dataLen = dataLen;


    //��������֡ͷ�ֽڣ�����CRC
    crc = a_GetCrc(pFrame + CP_FRAME_SCRADDR_POS, dataLen + CP_FRAME_HEAD_LEN - 2);
    CP_SetCrc(pTrail, crc);
    pTrail->trail = 0xAA;

    return (len);
}

BOOL CP_CheckFrame(u8 *pInFrame, u16 inLen, u8 **pOutFrame, u16 *pOutLen)
{
    BOOL b = FALSE;
    u16 startPos = 0;
    CP_FRAME_HEAD *pHead = NULL;
    CP_FRAME_TRAIL *pTrail = NULL;

    //�ҵ�֡ͷ
    startPos = 0;
    while((pInFrame[startPos] != CP_FRAME_HEAD1_FLAG) || (pInFrame[startPos + 1] != CP_FRAME_HEAD2_FLAG))
    {
        if(startPos < inLen)
        {
            startPos++;
        }
        else
        {
            break;
        }
    }
    inLen -= startPos;

    //֡�����㹻֡ͷ
    if(inLen > CP_FRAME_HEAD_LEN)
    {
        pHead = (CP_FRAME_HEAD *)(pInFrame + startPos);
    }
    else
    {
        return b;
    }



    //֡�����㹻����֡����
    if(inLen >= (CP_FRAME_HEAD_LEN + pHead->dataLen))
    {
        pTrail = (CP_FRAME_TRAIL *)(inLen + startPos + CP_FRAME_HEAD_LEN + pHead->dataLen);
    }
    else
    {
        return b;
    }

    *pOutFrame = pInFrame + inLen;
    //֡���� = ���ݳ��� + ֡ͷ���� + ֡β����
    *pOutLen = pHead->dataLen + CP_FRAME_HEAD_LEN + CP_FRAME_TRAIL_LEN;

    //�ж�֡β
    if(pTrail->trail == CP_FRAME_TRAIL_FLAG)
    {
        //�����򳤶��ж�(<= 200)
        if(pHead->dataLen <= CP_FRAME_DATA_MAX_LEN)
        {
            if(CP_CheckCrc(pInFrame + startPos, *pOutLen))
            {
                b = TRUE;
            }
        }
    }

    return b;
}


void CP_Task(void (*process)(u8 *pFrame, u16 len), u8 delayms)
{
    if(CP_IsRcvFrame(delayms))
    {
        u8 rcvFrame[CP_FRAME_BUF_MAX_LEN] = {0};
        u16 frameLen = 0;
        u8 startPos = 0;
        CP_FRAME_HEAD *pHead = NULL;
        CP_FRAME_TRAIL *pTrail = NULL;
        CP_SetRcvFlag(CP_FRAME_FLAG_COPY);


        //��ȡbuf
        frameLen = CP_GetFrameLen();
        CP_SetFrameRcvPos(0);
        memcpy(rcvFrame, (const void *)g_aCPRcvBuffer, frameLen);

        //�����־λ
        CP_SetRcvFlag(CP_FRAME_FLAG_IDLE);
        CP_ClearFrameTime();

        //�ҵ�֡ͷ
        startPos = 0;
        while((rcvFrame[startPos] != CP_FRAME_HEAD1_FLAG) || (rcvFrame[startPos + 1] != CP_FRAME_HEAD2_FLAG))
        {
            if(startPos < frameLen)
            {
                startPos++;
            }
            else
            {
                break;
            }
        }
        frameLen -= startPos;

        //֡�����㹻֡ͷ
        if(frameLen > CP_FRAME_HEAD_LEN)
        {
            pHead = (CP_FRAME_HEAD *)(rcvFrame + startPos);
        }
        else
        {
            return;
        }

        //֡�����㹻����֡����
        if(frameLen >= (CP_FRAME_HEAD_LEN + pHead->dataLen))
        {
            pTrail = (CP_FRAME_TRAIL *)(rcvFrame + startPos + CP_FRAME_HEAD_LEN + pHead->dataLen);
        }
        else
        {
            return;
        }

        //֡���� = ���ݳ��� + ֡ͷ���� + ֡β����
        frameLen = pHead->dataLen + CP_FRAME_HEAD_LEN + CP_FRAME_TRAIL_LEN;

        //�ж�֡β
        if(pTrail->trail == CP_FRAME_TRAIL_FLAG)
        {
            //�����򳤶��ж�(<= 200)
            if(pHead->dataLen <= CP_FRAME_DATA_MAX_LEN)
            {
                if(CP_CheckCrc(rcvFrame + startPos, frameLen))
                {
                    if(process != 0)
                    {
                        process(rcvFrame + startPos, frameLen);
                    }
                }
            }
        }
    }
}



#if 0
//ϵͳ����
u8 CP_SysTestReqFrame(u8 *pFrame, u8 testItem)
{
    u8 len = 1;

    *(pFrame + CP_FRAME_DSTART_POS) = testItem;

    len = CP_GetFrame(CP_FRAME_BROADCAST_ADDR, CP_FRAME_BROADCAST_ADDR, CP_FRAME_CTRL_REQUEST | CP_FRAME_CTRL_OVER,
                      CP_FRAME_CMD_TEST, CP_FRAME_STATE_REQUEST, pFrame, len);

    return len;
}

u8 CP_SysTestRspFrame(u8 *pFrame, u8 result)
{
    u8 len = 1;
    *(pFrame + CP_FRAME_DSTART_POS) = result;

    len = CP_GetFrame(CP_FRAME_BROADCAST_ADDR, CP_FRAME_BROADCAST_ADDR, CP_FRAME_CTRL_RESPONSE | CP_FRAME_CTRL_OVER,
                      CP_FRAME_CMD_TEST, CP_FRAME_STATE_RESPONE_OK, pFrame, len);


    return len;
}
#endif
