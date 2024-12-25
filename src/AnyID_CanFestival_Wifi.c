#include "AnyID_CanFestival_Wifi.h"

UART_RCVFRAME g_sWifiRxFrame = {0};
UART_RCVFRAME g_sUartTempRcvFrame = {0};
UART_TXFRAME g_sWifiTxFrame = {0};
u32 g_nWifiStatus = 0;

void Wifi_Init()
{
    //Wifi_InitInterface(bud);
    //Wifi_ConfigInt();
  //  Wifi_InitTxDma(g_sWifiTxFrame.buffer, UART_BUFFER_MAX_LEN);
  //  Wifi_InitRxDma(g_sWifiRxFrame.buffer, UART_BUFFER_MAX_LEN);
}

void  Wifi_NetInit(void *p)
{
    while(1)
    {
        if(USART_GetFlagStatus(WIFI_PORT, USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE))
        {
            USART_ITConfig(WIFI_PORT, USART_IT_RXNE, DISABLE);
             
            //Sys_Delayms(2);
            Uart_ResetFrame(g_sWifiRxFrame);
            USART_ClearFlag(WIFI_PORT, USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE); 

            Wifi_Init();
            Wifi_Connect(&g_sWifiOpRegs, (WIFI_REGS *)p);
        }
        
        if(Uart_IsRcvFrame(g_sWifiRxFrame))  
        {
            memcpy(&g_sUartTempRcvFrame, &g_sWifiRxFrame, sizeof(UART_RCVFRAME));
            Uart_ResetFrame(g_sWifiRxFrame);
            
            if(g_sWifiOpRegs.log)
            {
                //R485_WriteBuffer(g_sUartTempRcvFrame.buffer, g_sUartTempRcvFrame.len);
            }
            if(g_sWifiOpRegs.mode == WIFI_OP_MODE_DTU)
            {

                
            }
            //RCV
            else
            {
                if(Wifi_ChkRcv(&g_sWifiOpRegs, g_sUartTempRcvFrame.buffer, g_sUartTempRcvFrame.length))
                {
                    if(a_CheckStateBit(g_sWifiOpRegs.state, WIFI_OP_STATE_WAIT))                //接收好发送对一个
                    {
                        g_sWifiOpRegs.state = WIFI_OP_STATE_DELAY;                              //接收正确，进入下一步，要延时一点时间
                        g_sWifiOpRegs.tick = g_nSysTick; 
                        g_sWifiOpRegs.repeat = 0;
                    }
                }
            }
        }
        
        //等待连接DTU
        if(g_sWifiOpRegs.mode == WIFI_OP_MODE_IDLE)
        {
            if(a_CheckStateBit(g_sWifiOpRegs.state, WIFI_OP_STATE_TX))
            {
                a_ClearStateBit(g_sWifiOpRegs.state, WIFI_OP_STATE_TX);
                if(Wifi_QueneChk(&g_sWifiOpRegs))
                {
                    Uart_ResetFrame(g_sWifiRxFrame);                                               //清空接收区
                    Wifi_Tranceive(&g_sWifiOpRegs, g_nSysTick);
                    a_SetStateBit(g_sWifiOpRegs.state, WIFI_OP_STATE_WAIT | WIFI_OP_STATE_RCV);
                    if(g_sWifiOpRegs.log)
                    {
                       // R485_WriteBuffer((u8 *)g_aWifiTmpBuf, strlen(g_aWifiTmpBuf));
                       // R485_WriteBuffer("\r\n", 2);
                    }
                }
            }
            else if(a_CheckStateBit(g_sWifiOpRegs.state, WIFI_OP_STATE_WAIT))
            {
                if(g_sWifiOpRegs.tick + g_sWifiOpRegs.to[g_sWifiOpRegs.index] < g_nSysTick)
                {
                    g_sWifiOpRegs.repeat++;
                    if(g_sWifiOpRegs.repeat <= WIFI_OP_REPAT_TICK)                                  //超过重复次数，直接重来
                    {
                        a_SetState(g_sWifiOpRegs.state, WIFI_OP_STATE_TX);
                    }
                    else
                    {
                        if(g_sWifiOpRegs.mode == WIFI_OP_MODE_IDLE)
                        {
                            Wifi_Connect(&g_sWifiOpRegs, (WIFI_REGS *)p);
                        }
                        else
                        {
                            a_SetState(g_sWifiOpRegs.state, WIFI_OP_STATE_STEP);
                        }
                    }
                }
            }
            else if(a_CheckStateBit(g_sWifiOpRegs.state, WIFI_OP_STATE_DELAY))
            {
                if(g_sWifiOpRegs.tick + WIFI_OP_DELAY_TIME < g_nSysTick)
                {
                    a_SetState(g_sWifiOpRegs.state, WIFI_OP_STATE_STEP);
                }
            }
            else if(a_CheckStateBit(g_sWifiOpRegs.state, WIFI_OP_STATE_STEP))
            {
                a_ClearStateBit(g_sWifiOpRegs.state, WIFI_OP_STATE_STEP);
                
                g_sWifiOpRegs.repeat = 0;
                if(Wifi_Step(&g_sWifiOpRegs))
                {
                    if(g_sWifiOpRegs.index >= g_sWifiOpRegs.num)
                    {
                        if(g_sWifiOpRegs.mode == WIFI_OP_MODE_IDLE)
                        {
                            g_sWifiOpRegs.mode = WIFI_OP_MODE_DTU;
                            g_sWifiOpRegs.state = WIFI_OP_STATE_IDEL;
                        }
                    }
                    else
                    {
                        a_SetState(g_sWifiOpRegs.state, WIFI_OP_STATE_TX);
                    }
                }
            }
        }
    }
}

void Wifi_Connect(WIFI_OPREGS *pOpRegs, WIFI_REGS *pConRegs)
{   
    WIFI_RstLow();
    Wifi_Delayms(10);
    WIFI_RstHigh();
    Wifi_Delayms(50);

    Wifi_UpdateTimerPeriod(WIFI_RCV_TIM_WIFI);

    memset(pOpRegs, 0, sizeof(WIFI_OPREGS));
    
    pOpRegs->tcpMode = pConRegs->stationRegs.config & WIFI_STATION_CONFIG_TCPMASK;
    pOpRegs->log = pConRegs->netRegs.rfu & 0x01;
    
    memcpy(&pOpRegs->conRegs, pConRegs, sizeof(WIFI_REGS));
    
	pOpRegs->mode = WIFI_OP_MODE_IDLE;
	Wifi_ConnectWifi(pOpRegs);
}


void Wifi_QueneEnter(WIFI_OPREGS *pOpRegs, u8 op, u16 to)
{
    if(pOpRegs->num < WIFI_OP_NUM)
    {
        pOpRegs->op[pOpRegs->num] = op; 
        pOpRegs->to[pOpRegs->num] = to;
        pOpRegs->num++;
    }
}

void Wifi_ConnectWifi(WIFI_OPREGS *pOpRegs)
{
    pOpRegs->index = 0;
    pOpRegs->num = 0;
    
    Wifi_QueneEnter(pOpRegs, WIFI_CMD_AT_ATE, WIFI_TO_AT_NORMAL);                               //禁止回声
    Wifi_QueneEnter(pOpRegs, WIFI_CMD_AT_WMODE, WIFI_TO_AT_NORMAL);                             //设置工作模式 STA
    Wifi_QueneEnter(pOpRegs, WIFI_CMD_AT_WSDHCP, WIFI_TO_AT_NORMAL);                            //设置DHCP
    Wifi_QueneEnter(pOpRegs, WIFI_CMD_AT_WJAP, WIFI_TO_AT_LINK);                                //连接wifi
    Wifi_QueneEnter(pOpRegs, WIFI_CMD_AT_WJAP_QUERY, WIFI_TO_AT_NORMAL);                        //查询wifi连接状态
    Wifi_QueneEnter(pOpRegs, WIFI_CMD_AT_SKT_RECVCFG, WIFI_TO_AT_NORMAL);                       //设置skt数据上报模式
    Wifi_QueneEnter(pOpRegs, WIFI_CMD_AT_SOCKET, WIFI_TO_AT_LINK);                              //创建socket
    if(pOpRegs->tcpMode == WIFI_STATION_CONFIG_TCPSERVER)                                       //
    {
        Wifi_QueneEnter(pOpRegs, WIFI_CMD_AT_SKT_LISTEN, WIFI_TO_AT_NORMAL);                       
    }
    Wifi_QueneEnter(pOpRegs, WIFI_CMD_AT_SKT_ENTTT, WIFI_TO_AT_LINK);                            

    pOpRegs->state = WIFI_OP_STATE_TX;
}


char g_aWifiTmpBuf[WIFI_FEAME_TX_LEN] = {0};
void Wifi_Tranceive(WIFI_OPREGS *pOpRegs, u32 tick)
{
    u8 op = 0;

	op = pOpRegs->op[pOpRegs->index];
	switch (op)
    {
        case WIFI_CMD_AT_WAIT:                      
            pOpRegs->repeat = WIFI_OP_REPAT_TICK;
			break;
        case WIFI_CMD_AT_ATE:
            sprintf(g_aWifiTmpBuf, "%s", "ATE0");
			Wifi_WriteAtCmd(g_aWifiTmpBuf);
			break;
		case WIFI_CMD_AT_WMODE:
            sprintf(g_aWifiTmpBuf, "%s", "AT+WMODE=1,0");
            Wifi_WriteAtCmd(g_aWifiTmpBuf);             //固定STA模式
			break;
		case WIFI_CMD_AT_WSDHCP:
            if((pOpRegs->conRegs.stationRegs.config & WIFI_STATION_CONFIG_ENDHCP) == WIFI_STATION_CONFIG_ENDHCP)       //DHCP使能
            {
                sprintf(g_aWifiTmpBuf, "%s", "AT+WSDHCP=1");
                Wifi_WriteAtCmd(g_aWifiTmpBuf);
            }
            else
            {
                sprintf(g_aWifiTmpBuf, "AT+WSDHCP=0,%d.%d.%d.%d,%d.%d.%d.%d,%d.%d.%d.%d", pOpRegs->conRegs.stationRegs.localIp[0],  pOpRegs->conRegs.stationRegs.localIp[1],  
                                                                                          pOpRegs->conRegs.stationRegs.localIp[2],  pOpRegs->conRegs.stationRegs.localIp[3],
					 																      pOpRegs->conRegs.stationRegs.localMask[0],  pOpRegs->conRegs.stationRegs.localMask[1],  
                                                                                          pOpRegs->conRegs.stationRegs.localMask[2],  pOpRegs->conRegs.stationRegs.localMask[3], 
					 																      pOpRegs->conRegs.stationRegs.localGateWay[0],  pOpRegs->conRegs.stationRegs.localGateWay[1],  
                                                                                          pOpRegs->conRegs.stationRegs.localGateWay[2],  pOpRegs->conRegs.stationRegs.localGateWay[3]);
				Wifi_WriteAtCmd(g_aWifiTmpBuf);
            }
			break;
		case WIFI_CMD_AT_WJAP:
            if((pOpRegs->conRegs.netRegs.ssidLen != 0x00) && (pOpRegs->conRegs.netRegs.pwdLen != 0))
            {
                pOpRegs->conRegs.netRegs.ssid[pOpRegs->conRegs.netRegs.ssidLen] = 0;                                    //防止后面拼接异常 
                pOpRegs->conRegs.netRegs.pwd[pOpRegs->conRegs.netRegs.pwdLen] = 0;
                sprintf(g_aWifiTmpBuf, "AT+WJAP=%s,%s", pOpRegs->conRegs.netRegs.ssid, pOpRegs->conRegs.netRegs.pwd);
                Wifi_WriteAtCmd(g_aWifiTmpBuf);
            }
			break;
		case WIFI_CMD_AT_WJAP_QUERY:
            sprintf(g_aWifiTmpBuf, "%s", "AT+WJAP?");
            Wifi_WriteAtCmd(g_aWifiTmpBuf);
			break;
		case WIFI_CMD_AT_SKT_RECVCFG:
            sprintf(g_aWifiTmpBuf, "%s", "AT+SOCKETRECVCFG=0");
            Wifi_WriteAtCmd(g_aWifiTmpBuf);
			break;
		case WIFI_CMD_AT_SKT_ENTTT:
            sprintf(g_aWifiTmpBuf, "%s", "AT+SOCKETTT");
            Wifi_WriteAtCmd(g_aWifiTmpBuf);
			break;
		case WIFI_CMD_AT_SOCKET:
            if(pOpRegs->tcpMode == WIFI_STATION_CONFIG_TCPSERVER)
            {
                sprintf(g_aWifiTmpBuf, "AT+SOCKET=3,%d", pOpRegs->conRegs.stationRegs.localPort);
            }
            else
            {
                sprintf(g_aWifiTmpBuf, "AT+SOCKET=4,%d.%d.%d.%d,%d,0", pOpRegs->conRegs.stationRegs.remoteIp[0], pOpRegs->conRegs.stationRegs.remoteIp[1], 
                                                                       pOpRegs->conRegs.stationRegs.remoteIp[2], pOpRegs->conRegs.stationRegs.remoteIp[3], 
                                                                       pOpRegs->conRegs.stationRegs.remotePort);
            }
			Wifi_WriteAtCmd(g_aWifiTmpBuf);
			break;
        case WIFI_CMD_AT_SKT_LISTEN:
            sprintf(g_aWifiTmpBuf, "%s", "AT+SOCKET?");
            Wifi_WriteAtCmd(g_aWifiTmpBuf);
			break;
		default:
			break;
			
    }
    pOpRegs->tick = tick;
}

BOOL Wifi_ChkRcv(WIFI_OPREGS *pOpRegs, u8 *pRxFrame, u16 rxLen)
{
	u8 op = 0;
	BOOL bOk = FALSE;

	op = pOpRegs->op[pOpRegs->index];
	switch (op)
    {
        case WIFI_CMD_AT_WAIT:
            /*if(strstr((char const *)pRxFrame, "DISCONNECT") || 
               strstr((char const *)pRxFrame, "ERROR") || 
               strstr((char const *)pRxFrame, "Error"))    //wifi鏂烽枊
            {
                bOk = TRUE;
            }
            else if(strstr((char const *)pRxFrame, "SocketDisconnect"))    //wifi鏂烽枊
            {
                bOk = TRUE;
            }
            else if(strstr((char const *)pRxFrame, "cketDown"))    
            {
                if(Wifi_GetSocketDownInfo((char *)pRxFrame, pOpRegs))
                {
                    pOpRegs->dtuState = WIFI_DTU_STAT_CON;
                }
                bOk = TRUE;
            }
            else*/
            {
                bOk = TRUE;
            }
            break;
        case WIFI_CMD_AT_WJAP:
           if(strstr((char const *)pRxFrame, "CONNECT"))        //
            {
                bOk = TRUE;
            }
            break;
        case WIFI_CMD_AT_WJAP_QUERY:
            if( strstr((char const *)pRxFrame, "WJAP:3"))       //查询wifi连接状态
            {
                pOpRegs->dtuState = WIFI_DTU_STAT_CONWIFI;
                bOk = TRUE;
            }
			break;
        case WIFI_CMD_AT_ATE:
        case WIFI_CMD_AT_WMODE:
        case WIFI_CMD_AT_WSDHCP:
		case WIFI_CMD_AT_SKT_RECVCFG:
            if(strstr((char const *)pRxFrame, "OK\r\n"))
            {
                bOk = TRUE;
            }
			break;
        case WIFI_CMD_AT_SKT_ENTTT:
            if(strstr((char const *)pRxFrame, ">"))
            {
                pOpRegs->dtuState = WIFI_DTU_STAT_DTU;
                bOk = TRUE;
            }
            else if(strstr((char const *)pRxFrame, "SOCKETTT:"))        //鍏朵粬鍏跺疄淇℃伅buyon绠?
            {
                pOpRegs->repeat = 0;
                bOk = TRUE;
            }
            else if(Wifi_CheckEvent((char *)pRxFrame, pOpRegs))
            {
                //pOpRegs->dtuState = WIFI_DTU_STAT_ERR;
            }
			break;
        case WIFI_CMD_AT_SOCKET:
            if(strstr((char const *)pRxFrame, "success"))
            {
                if((pOpRegs->conRegs.stationRegs.config & WIFI_STATION_CONFIG_TCPMASK) == WIFI_STATION_CONFIG_TCPCLIENT)
                {
                    pOpRegs->dtuState = WIFI_DTU_STAT_DTU;
                }
                bOk = TRUE;
            }
            else
            {
                if(pOpRegs->tcpMode == WIFI_STATION_CONFIG_TCPCLIENT)
                {
                    pOpRegs->repeat = 0;                                //client模式需要一直连接主机
                }
            }
			break;
        case WIFI_CMD_AT_SKT_LISTEN:
            if(strstr((char const *)pRxFrame, "OK"))
            {
                if(Wifi_CheckEvent((char *)pRxFrame, pOpRegs))                                              //检查是否有错误
                {
                    pOpRegs->repeat = WIFI_OP_REPAT_TICK;                                                   //如果出错，不需要重复
                }
                else
                {
                    if(pOpRegs->tcpMode == WIFI_STATION_CONFIG_TCPSERVER)                                   //server模式，需要监听client的是否连接
                    {
                        pOpRegs->repeat = 0;                                                                //检验是否有连接, listen需要一直执行
                        if(Wifi_CheckClientConnected((char *)pRxFrame, pOpRegs))                           
                        {
                            pOpRegs->dtuState = WIFI_DTU_STAT_DTU;
                            bOk= TRUE;
                        }
                    }
                    else
                    {
                        
                    }
                }
            }
			break;
		default:
			break;
    }
    
    if(bOk)
    {
        pOpRegs->repeat = 0;
    }
    return bOk;
}

u8 Wifi_CheckEvent(char *pEvtStr, WIFI_OPREGS *pOpRegs)
{
    u8 err = WIFI_ERR_NO;
    
    if(strstr((char const *)pEvtStr, "DISCONNECT"))                 //wifi disconnect
    {
        pOpRegs->dtuState = WIFI_DTU_STAT_DIS;
        err = WIFI_ERR_WIFI;
    }
    else if(strstr((char const *)pEvtStr, "SocketDisconnect"))      //socket disconnect
    {
        pOpRegs->dtuState = WIFI_DTU_STAT_CONWIFI;
        err = WIFI_ERR_SOCKET;
    }
    else if(strstr((char const *)pEvtStr, "SocketSeed"))            //socket connect
    {
        if(pOpRegs->mode == WIFI_OP_MODE_DTU)                       //如果是DTU模式了，只要有socket连接上，就可以认为再次进入DTU
        {
            pOpRegs->dtuState = WIFI_DTU_STAT_DTU;
        }
        err = WIFI_ERR_NO;
    }
    else if(strstr((char const *)pEvtStr, "ERROR")) 
    {
        err = WIFI_ERR_OTHER;
    }
    
    return err;
}

BOOL Wifi_Step(WIFI_OPREGS *pOpRegs)
{
	//u8 op = 0;
	BOOL bOk = TRUE;
    
    pOpRegs->index++;

    return bOk;
}

u32 Wifi_SubDecStr(char *str, char *subStr, char c, u32 bufSize)                                //摘取数字字符串
{
    u32 i = 0;
    u32 len = 0;
    len = strlen(str);
    for(i = 0; i < len && i < bufSize - 1; i++)
    {
        if((str[i] != c) && (str[i] != '\0') && (str[i] >= '0') && (str[i] <= '9'))
        {
            subStr[i] = str[i];
        }
        else
        {
            break;
        }
    }
    
    return i;
}
#define WIFI_STR_DAT_BUF_SIZE               5


BOOL Wifi_GetSocketReadInfo(char *str, WIFI_OPREGS *pOpRegs, u16 *pDatStart)
{
    char *p = NULL;
    char conStr[WIFI_STR_DAT_BUF_SIZE] = "";
    char lenStr[WIFI_STR_DAT_BUF_SIZE] = "";
    //u32 dtuConID = 0;
    
    u32 len = 0;
    BOOL b = FALSE;
    
    p = strstr(str, "CKETREAD,");
    if(p != NULL)
    {
       p += strlen("CKETREAD,");
       len = Wifi_SubDecStr(p, conStr, ',', WIFI_STR_DAT_BUF_SIZE);
       if(len > 0)
       {
           // dtuConID = a_atoi((const u8 *)conStr, (const u8)len, STD_LIB_FMT_DEC);
            //if(dtuConID == pOpRegs->dtuConID)
            {
                p += len + 1; 
                len = Wifi_SubDecStr(p, lenStr, ',', WIFI_STR_DAT_BUF_SIZE);
                if(len > 0)
                {
                    *pDatStart = (p + len + 1) - str;
                    //pOpRegs->dtuRxLen = a_atoi((const u8 *)lenStr, (const u8)len, STD_LIB_FMT_DEC);
                    b = TRUE;
                }
            }
            
       }
    }
    
    return b;
}

BOOL Wifi_GetSocketDownInfo(char *str, WIFI_OPREGS *pOpRegs)
{
    char *p = NULL;
    char conStr[WIFI_STR_DAT_BUF_SIZE] = "";
    char lenStr[WIFI_STR_DAT_BUF_SIZE] = "";
    
    u32 len = 0;
    BOOL b = FALSE;
    
    p = strstr(str, "cketDown,");
    if(p != NULL)
    {
       p += strlen("cketDown,");
       len = Wifi_SubDecStr(p, conStr, ',', WIFI_STR_DAT_BUF_SIZE);
       if(len > 0)
       {
            //pOpRegs->dtuConID = a_atoi((const u8 *)conStr, (const u8)len, STD_LIB_FMT_DEC);
           
            p += len + 1; 
            len = Wifi_SubDecStr(p, lenStr, ',', WIFI_STR_DAT_BUF_SIZE);
            if(len > 0)
            {
                //pOpRegs->dtuRxLen = a_atoi((const u8 *)lenStr, (const u8)len, STD_LIB_FMT_DEC);
                b = TRUE;
            }
       }
    }
    
    if(b == FALSE)
    {
        //pOpRegs->dtuRxLen = 0;
        //pOpRegs->dtuConID = 0;
    }
    
    return b;
}

BOOL Wifi_CheckClientConnected(char *str, WIFI_OPREGS *pOpRegs)
{
    char *p = NULL;
    char conStr[WIFI_STR_DAT_BUF_SIZE] = "";
    char statusStr[WIFI_STR_DAT_BUF_SIZE] = "";
    u32 len = 0, pos = 0;
    BOOL b = FALSE;

    do{
        p = strstr(str + pos, "\r\n");
        if(p != NULL)
        {
            pos = (p - str);
            p += 2; pos += 2;
            len = Wifi_SubDecStr(p, conStr, ',', WIFI_STR_DAT_BUF_SIZE);
            p += len + 1; pos += len + 1;
            len = Wifi_SubDecStr(p, statusStr, ',', WIFI_STR_DAT_BUF_SIZE);       //鐒℃晥
            p += len + 1; pos += len + 1;
            len = Wifi_SubDecStr(p, statusStr, ',', WIFI_STR_DAT_BUF_SIZE);
            if(len == 1)
            {
                if(statusStr[0] == '3')
                {
                    b = TRUE;
                    break;
                }
            }
            pos += len + 1;
        }
    }while(p != NULL);

    return b;
}

BOOL Wifi_DelSocket(char *str)
{
    char *p = NULL;
    char conStr[WIFI_STR_DAT_BUF_SIZE] = "";
    
    u32 len = 0;
    BOOL b = FALSE;
    
    p = strstr(str, "cketDisconnect,");
    if(p != NULL)
    {
       p += strlen("cketDisconnect,");
       len = Wifi_SubDecStr(p, conStr, ',', WIFI_STR_DAT_BUF_SIZE);
       if(len > 0)
       {
            u32 conID = a_atoi((const u8 *)conStr, (const u8)len, STD_LIB_FMT_DEC);
            
            sprintf(g_aWifiTmpBuf, "AT+SOCKETDEL=%d", conID);
            Wifi_WriteAtCmd(g_aWifiTmpBuf);
            
            b = TRUE;
            
       }
    }
    
    return b;
}

u8 *SeachStartIndex(char const *str, u8 len, u8 *startIndex)
{
    //鏀寔鏈�澶у洓涓瓧绗?
    u8 pos = 0;
    
    for(pos = 0; pos <= 128; pos++)
    {
        if(*(startIndex + pos) == 0x2C)
        {
            if(*(startIndex + pos + 1) == 0x22)             //,""
            {
                break;
            }
        }
    }

    return startIndex + pos;        
}

#define BW12_DATA_DELIMITER                         0x2C

void Bw12_PortDTStr(u16 port, u8 *str)
{
    if(port <= 10)
    {
        sprintf((char *)str, "%d", port % 10 / 1);
    }
    else if(port <= 100)
    {
        sprintf((char *)str, "%d%d", port / 10, port % 10 / 1);
    }
    else if(port <= 1000)
    {
       sprintf((char *)str, "%d%d%d", port / 100, (port % 100) / 10, port % 10 / 1);
    }
    else if(port <= 10000)
    {
        sprintf((char *)str, "%d%d%d%d", port / 1000, (port % 1000) / 100, (port % 100) / 10, port % 10 / 1);
    }
    else
    {  
        sprintf((char *)str, "%d%d%d%d%d", port / 10000, (port % 10000) / 1000, (port % 1000) / 100, (port % 100) / 10, port % 10 / 1);
    }
}

BOOL Bw12_ChkSerVer(u8 *port)
{
    BOOL bOk = FALSE;
    /*WIFI_OPREGS *pOpRegs = NULL;
    u8 *startIndex = NULL, *endIndex = NULL;
    
    pOpRegs = &g_sWifiOpRegs;
    
    if(strstr((char const *)pRxFrame, (char *)(port)))
    {
        startIndex =  (u8 *)(strstr((char const *)pRxFrame, (char *)(port)) + strlen((char const *)port) + 4); 
        endIndex = startIndex + 1;
        if(*startIndex != STR_HEX_CR && *endIndex != STR_HEX_LF)
        {
            //startIndex = 1;     
            pOpRegs->opTcpCReg.id =  a_atoi(startIndex, endIndex - startIndex, STD_LIB_FMT_DEC);
            
            startIndex = endIndex + 1 ;
            endIndex = startIndex + 1;
            pOpRegs->opTcpCReg.type =  a_atoi(startIndex, endIndex - startIndex, STD_LIB_FMT_DEC);
            
            startIndex = endIndex + 1 ;
            endIndex = startIndex + 1;
            pOpRegs->opTcpCReg.status =  a_atoi(startIndex, endIndex - startIndex, STD_LIB_FMT_DEC);
            
            startIndex = (u8 *)(strstr((char const *)endIndex, ",") + 1); 
            endIndex = (u8 *)strstr((char const *)startIndex, ".");
            pOpRegs->opTcpCReg.ip[0] =  a_atoi(startIndex, endIndex - startIndex, STD_LIB_FMT_DEC);
            
            startIndex = (u8 *)(strstr((char const *)endIndex, ".") + 1); 
            endIndex = (u8 *)strstr((char const *)startIndex, ".");
            pOpRegs->opTcpCReg.ip[1] =  a_atoi(startIndex, endIndex - startIndex, STD_LIB_FMT_DEC);
            
            startIndex = (u8 *)(strstr((char const *)endIndex, ".") + 1); 
            endIndex = (u8 *)strstr((char const *)startIndex, ".");
            pOpRegs->opTcpCReg.ip[2]  =  a_atoi(startIndex, endIndex - startIndex, STD_LIB_FMT_DEC);
            
            startIndex = (u8 *)(strstr((char const *)endIndex, ".") + 1); 
            endIndex = (u8 *)strstr((char const *)startIndex, ",");
            pOpRegs->opTcpCReg.ip[3] =  a_atoi(startIndex, endIndex - startIndex, STD_LIB_FMT_DEC);
            
            startIndex = (u8 *)(strstr((char const *)endIndex, ",") + 1); 
            endIndex = (u8 *)strstr((char const *)startIndex, ",");
            pOpRegs->opTcpCReg.port =  a_atoi(startIndex, endIndex - startIndex, STD_LIB_FMT_DEC);

            endIndex += 5;                         //闂鍊硷紝startIndex姝ゅ鏃犳硶澧炲姞锛宔ndIndex鍙互锛屽悗缁洿鏀?

            if(memcmp(endIndex, "\r\n\r\nOK\r\n", 8) == 0)
            {
                bOk =  TRUE;
            }
        }
    }*/
    return bOk;

}


u8 Wifi_16To8(u8 len, u16 *pFrame, u8 *pBuffer)
{
    u8 index = 0;
    
    for(index = 0; index < len; index ++)
    {
        *(pBuffer + index) = *(pFrame + index) & 0x00FF;
    }
    
    return index;
}

u8 Wifi_8To16(u8 len, u8 *pFrame, u16 *pBuffer)
{
    u8 index = 0;
    
    for(index = 0; index < len; index ++)
    {
        *(pBuffer + index) = *(pFrame + index);
    }
    
    return index;
}