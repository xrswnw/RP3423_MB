#include "AnyID_Uart_Receive.h"

void Uart_ReceiveFrame(u8 byte, UART_RCVFRAME *pRcvFrame)
{
    static u16 specialLen = 0;
    switch(pRcvFrame->state)
    {
        case UART_STAT_IDLE:
            if(byte == UART_FRAME_FLAG_HEAD1)
            {
                pRcvFrame->state = UART_STAT_HEAD1;
                pRcvFrame->buffer[0] = UART_FRAME_FLAG_HEAD1;
            }
            break;
        case UART_STAT_HEAD1:
            if(byte == UART_FRAME_FLAG_HEAD2)
            {
                pRcvFrame->state = UART_STAT_HEAD2;
                pRcvFrame->buffer[1] = UART_FRAME_FLAG_HEAD2;
            }
            else if(byte == UART_FRAME_FLAG_HEAD1)
            {
                pRcvFrame->state = UART_STAT_HEAD1;
            }
            else
            {
                pRcvFrame->state = UART_STAT_IDLE;
            }
            break;
        case UART_STAT_HEAD2:
            if(UART_FRAME_DATA_MIN_LEN <= byte && byte <= UART_FRAME_DATA_MAX_LEN)
            {
                pRcvFrame->state = UART_STAT_DATA;
                pRcvFrame->dataLen = byte + 3;
                pRcvFrame->buffer[2] = byte;
                pRcvFrame->index = 3;
            }
            else if(byte == 0) //��?��a??��?
            {
                pRcvFrame->state = UART_STAT_DATA;
                pRcvFrame->dataLen = 0;
                pRcvFrame->buffer[2] = byte;
                pRcvFrame->index = 3;
            }
            else
            {
                pRcvFrame->state = UART_STAT_IDLE;
            }
            break;
        case UART_STAT_DATA:
            pRcvFrame->buffer[pRcvFrame->index++] = byte;

            if(pRcvFrame->dataLen == 0)
            {
                if(pRcvFrame->index == 10)
                {
                    specialLen = byte;
                }
                else if(pRcvFrame->index == 11)
                {
                    specialLen += byte * 256;
                    pRcvFrame->dataLen = specialLen + 2 + 11;
                    if(pRcvFrame->dataLen > UART_BUFFER_MAX_LEN)
                    {
                        pRcvFrame->state = UART_STAT_IDLE;
                    }
                }

            }
            else if(pRcvFrame->index >= pRcvFrame->dataLen)
            {
                pRcvFrame->state = UART_STAT_END;
            }
            break;
    }
    /*pRcvFrame->state = UART_STAT_DATA;
    pRcvFrame->buffer[pRcvFrame->index++] = byte;
    if(pRcvFrame->index >= UART_BUFFER_MAX_LEN)
    {
        pRcvFrame->state = UART_STAT_END;
    }*/
    pRcvFrame->idleTime = 0;
}

BOOL Uart_IsRcvFrameOver(UART_RCVFRAME *pRcvFrame)
{
    BOOL bOver = FALSE;
    if(pRcvFrame->state == UART_STAT_TO)
    {
        if(pRcvFrame->index > UART_FRAME_DATA_MIN_LEN)
        {
            bOver = TRUE;
        }
        else
        {
            pRcvFrame->state = UART_STAT_IDLE;
        }
    }
    else if(pRcvFrame->state == UART_STAT_END)
    {
        bOver = TRUE;
    }

    return bOver;
}
