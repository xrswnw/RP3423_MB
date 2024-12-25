#include "AnyID_CanFestival_Device.h"

const u8 DEVICE_VERSION[DEVICE_VERSION_SIZE]@0x08005000 = "RP3423_MB 24122500 G231200";

#define MASTER                  1

#ifdef MASTER
    #define I_AM_MASTER         MASTER
#else
    #define I_AM_MASTER         0
#endif

DEVICE_PARAMS g_sDeviceParams = {0};
DEVICE_SUBINFO g_sDeviceSubInfo = {0};
DEVICE_OP g_sDeviceOp = {0};

void Device_Init()
{
#if I_AM_MASTER
    if(Can_QueueCreatTask())
    {
        __NOP();
    }
#endif
}

void Device_ResetDeviceParamenter()
{
    memset(&g_sDeviceParams, 0, sizeof(DEVICE_PARAMS));
		
	//系统参数
	g_sDeviceParams.addr = 0x01;
    //--------------------------------
    g_sDeviceParams.rfParams.pemace.power = RF_PARAMS_PEMACE_POWER_5W;
    g_sDeviceParams.rfParams.pemace.immunity = RF_PARAMS_PEMACE_IMMUNITY_HIGH;              
    g_sDeviceParams.rfParams.pemace.sensitivity = RF_PARAMS_PEMACE_SENSITIVITY_HIGH;             
    g_sDeviceParams.rfParams.pemace.depth = RF_PARAMS_PEMACE_DEPTH_ASK100;                 
    g_sDeviceParams.rfParams.pemace.slot = RF_PARAMS_PEMACE_SLOT_16;       
    
    g_sDeviceParams.rfParams.amar.amarMode = RF_PARAMS_AMAR_MODE_AFI;
    g_sDeviceParams.rfParams.amar.op = RF_PARAMS_AMAR_OP_READ_UID;
    g_sDeviceParams.rfParams.amar.opAddr = RF_PARAMS_AMAR_OP_ADDR_DEFULAT;
    g_sDeviceParams.rfParams.amar.opLenth = RF_PARAMS_AMAR_OP_LENTH_DEFULAT;
    
    g_sDeviceParams.rfParams.afi.enable = RF_PARAMS_AFI_ENABLE_AFI0;
    g_sDeviceParams.rfParams.afi.afi0 = RF_PARAMS_AFI_VALUE_DEFULAT;
    g_sDeviceParams.rfParams.afi.afi1 = RF_PARAMS_AFI_VALUE_DEFULAT;
    g_sDeviceParams.rfParams.afi.afi2 = RF_PARAMS_AFI_VALUE_DEFULAT;
    g_sDeviceParams.rfParams.afi.afi3 = RF_PARAMS_AFI_VALUE_DEFULAT;
    
    g_sDeviceParams.rfParams.eas.enable = RF_PARAMS_EAS_ENABLE_EAS0;
    g_sDeviceParams.rfParams.eas.eas0 = RF_PARAMS_EAS_VALUE_DEFULAT;
    g_sDeviceParams.rfParams.eas.eas1 = RF_PARAMS_EAS_VALUE_DEFULAT;
    g_sDeviceParams.rfParams.eas.eas2 = RF_PARAMS_EAS_VALUE_DEFULAT;
    
    g_sDeviceParams.rfParams.infraed.enable = RF_PARAMS_INFRARED_ENABLE_MODE0;
    
    //--------------------------
    g_sDeviceParams.wokeParams.type = DEVICE_SUBDEVICE_TYPE_MB;
    g_sDeviceParams.wokeParams.tigger = DEVICE_WOKE_PARAMS_TIGGER_CYCLE;
    g_sDeviceParams.wokeParams.cycleTime = DEVICE_WOKE_PARAMS_CYCLETIME_500MS;
    g_sDeviceParams.wokeParams.flag = DEVICE_WOKE_PARAMS_FLAG_NULL;
    g_sDeviceParams.wokeParams.netDissTime = DEVICE_WOKE_PARAMS_NETDISSTIME_100S;
    g_sDeviceParams.wokeParams.linitTime = DEVICE_WOKE_PARAMS_LINITTIME_DEFAULT;
}

void Device_ReadDeviceParamenter(void)
{
	BOOL b = FALSE, bBackup = FALSE;
     
    b = FRam_ReadBuffer(FRAME_INFO_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    if(b)
    {
        u32 crc1 = 0, crc2 = 0;

        crc1 = a_GetCrc((u8 *)(&g_sDeviceParams), (sizeof(DEVICE_PARAMS)) - 4);
        crc2 = g_sDeviceParams.crc;

        //检测参数是否正确，如果不正确，使用默认参数操作
        if(crc1 != crc2)
        {
            b = FALSE;
        }
    }  
    bBackup = FRam_ReadBuffer(FRAME_INFO_BACKUP_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    if(bBackup)
    {
        u32 crc1 = 0, crc2 = 0;

        crc1 = a_GetCrc((u8 *)(&g_sDeviceParams), (sizeof(DEVICE_PARAMS)) - 4);
        crc2 = g_sDeviceParams.crc;

        //检测参数是否正确，如果不正确，使用默认参数操作                        .
        if(crc1 != crc2)
        {
            bBackup = FALSE;
        }
    }

    if(b == FALSE && bBackup == FALSE)
    {
		//参数如何恢复默认？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
		Device_ResetDeviceParamenter();
        Device_WriteDeviceParamenter();
    }
    else if(b == TRUE && bBackup == FALSE)
    {
        FRam_ReadBuffer(FRAME_INFO_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
        FRam_WriteBuffer(FRAME_INFO_BACKUP_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    }
    else if(b == FALSE && bBackup == TRUE)
    {
        FRam_ReadBuffer(FRAME_INFO_BACKUP_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
        FRam_WriteBuffer(FRAME_INFO_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    }
    
    Fram_ReadBootParamenter();
	if((g_sFramBootParamenter.appState != FRAM_BOOT_APP_OK) || 
	   (g_sFramBootParamenter.br ^ g_sDeviceParams.bud)	||
		(g_sFramBootParamenter.addr ^ g_sDeviceParams.addr))
    {
        g_sFramBootParamenter.appState = FRAM_BOOT_APP_OK;
		g_sFramBootParamenter.br = g_sDeviceParams.bud;
		g_sFramBootParamenter.addr = g_sDeviceParams.addr;
        Fram_WriteBootParamenter();
    }
}


BOOL Device_WriteDeviceParamenter(void)
{
    BOOL b = FALSE;

    g_sDeviceParams.crc = 0;
    g_sDeviceParams.crc = a_GetCrc((u8 *)(&g_sDeviceParams), (sizeof(DEVICE_PARAMS)) - 4);

    b = FRam_WriteBuffer(FRAME_INFO_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    b = FRam_WriteBuffer(FRAME_INFO_BACKUP_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    
    return b;
}


u16 Device_ResponseFrame(u8 *pParam, u16 len, UART_TXFRAME *pRspFrame)
{
    u16 pos = 0;
    u16 crc = 0;

    pRspFrame->buffer[pos++] = UART_FRAME_FLAG_HEAD1; // frame head first byte
    pRspFrame->buffer[pos++] = UART_FRAME_FLAG_HEAD2; // frame haed second byte
    pRspFrame->buffer[pos++] = 0x00;   // length
    pRspFrame->buffer[pos++] = (g_sDeviceParams.addr >> 0) & 0xFF;
    pRspFrame->buffer[pos++] = (g_sDeviceParams.addr >> 8) & 0xFF;
    pRspFrame->buffer[pos++] = (pRspFrame->destAddr >> 0) & 0xFF;
    pRspFrame->buffer[pos++] = (pRspFrame->destAddr >> 8) & 0xFF;
    pRspFrame->buffer[pos++] = UART_FRAME_RESPONSE_FLAG;
    pRspFrame->buffer[pos++] = pRspFrame->cmd;               // cmd
    pRspFrame->buffer[pos++] = UART_FRAME_PARAM_RFU;     // RFU

    if(len > UART_FRAME_DATA_MAX_LEN)
    {
        pRspFrame->buffer[pos++] = (len >> 0) & 0xFF;
        pRspFrame->buffer[pos++] = (len >> 8) & 0xFF;

        memcpy(pRspFrame->buffer + pos, pParam, len);
        pos += len;
    }
    else
    {
        memcpy(pRspFrame->buffer + pos, pParam, len);
        pos += len;
        
        pRspFrame->buffer[pos++] = pRspFrame->flag;
        pRspFrame->buffer[pos++] = pRspFrame->err;
        
        pRspFrame->buffer[UART_FRAME_POS_LEN] = pos - 3 + 2; //减去帧头7E 55 LEN 的三个字节，加上CRC的两个字节
    }

    crc = a_GetCrc(pRspFrame->buffer + UART_FRAME_POS_LEN, pos - UART_FRAME_POS_LEN); //从LEN开始计算crc
    pRspFrame->buffer[pos++] = crc & 0xFF;
    pRspFrame->buffer[pos++] = (crc >> 8) & 0xFF;

    pRspFrame->len = pos;

    return pos;
}

u16 Reader_ProcessUartFrame(u8 *pFrame, UART_TXFRAME *txFrame)
{
    u8 cmd = 0;
    u16 destAddr = 0;
    u16 paramsLen = 0;
    u8 *pParams = NULL;

    destAddr = *(u16 *)(pFrame + UART_FRAME_POS_DESTADDR);
    if((destAddr != DEVICE_FRAME_BROADCAST_ADDR) && (destAddr != g_sDeviceParams.addr) )
    {
        return 0;
    }

    txFrame->destAddr = *((u16 *)(pFrame + UART_FRAME_POS_SRCADDR));
    txFrame->len = 0;
    cmd = *(pFrame + UART_FRAME_POS_CMD);

    txFrame->cmd = cmd;
    txFrame->flag = UART_FRAME_FLAG_OK;
    txFrame->err = UART_FRAME_RSP_NOERR;

    if(pFrame[UART_FRAME_POS_LEN] == 0)
    {
        paramsLen = *((u16 *)(pFrame + UART_FRAME_POS_PAR));
        pParams = pFrame + UART_FRAME_POS_PAR + 2;
    }
    else
    {
        paramsLen = pFrame[UART_FRAME_POS_LEN] + 3 - UART_FRAME_MIN_LEN;;
        pParams = pFrame + UART_FRAME_POS_PAR;
    }
    
    switch(cmd)
    {
        case UART_COM_CMD_GET_VERSION:
          if(paramsLen == 0)
          {
                txFrame->result = UART_COM_RESULT_NEED_RSP;
                txFrame->len = Device_ResponseFrame((u8 *)&DEVICE_VERSION, DEVICE_VERSION_SIZE, txFrame);
          }
        break;
    case UART_COM_CMD_SUBDEVICE_DTU:
         if((paramsLen <= (CAN_FRAME_MAX_LENTH + 1)) && (paramsLen >= (3)))
          {
                CAN_FRAME canTxFrame = {0};

                if(*(pParams) == 0xFF)
                {
                    canTxFrame.coBid = DEVICE_COM_BASE_BOAD_COBID;
                    canTxFrame.flag = CAN_FRAME_FLAG_NO_RSP;
                    
                    txFrame->result = UART_COM_RESULT_NEED_RSP;
                    txFrame->len = Device_ResponseFrame(NULL, 0, txFrame);
                }
                else
                {
                    canTxFrame.coBid = DEVICE_COB_ID_MT_SDO + *(pParams);
                    canTxFrame.flag = CAN_FRAME_FLAG_NEED_RSP;
                    
                    txFrame->len = 1;
                    txFrame->result = UART_COM_RESULT_NO_RSP;
                }
                
                canTxFrame.len = paramsLen - 1;
                memcpy(&canTxFrame.buffer, pParams + 1, canTxFrame.len);
                canTxFrame.staus = CAN_FRAME_STATUS_REPORT;
                if(xQueueSend(g_hCanTxQueue, &canTxFrame, portMAX_DELAY))
                {
                    g_sUartTxFrame.cmd = UART_COM_CMD_SUBDEVICE_DTU;
                    g_sUartTxFrame.destAddr = txFrame->destAddr;
                }
          }
      break;
        default:
            break;
    }
    
    if(txFrame->len == 0)
    {
        txFrame->flag = UART_FRAME_FLAG_FAIL;
        txFrame->err = UART_FRAME_RSP_PARAMSERR;
        txFrame->len = Device_ResponseFrame(NULL, 0, txFrame);
    }
    
    return txFrame->len;
}


u8 Device_ProcessCobMTSdo(u32 addr, u8 cmd, u8 paramsLen, u8 *pParams)
{
    u8 frame[8] = {0};
    u8 pos = 0;
    u8 result = DEVICE_CAN_FRAME_RESULT_NO_RSP;
    u32 coBid = 0;
    
    if(addr != g_sDeviceParams.addr && addr != DEVICE_COM_BASE_BOAD_COBID)
    {
        return DEVICE_CAN_FRAME_RESULT_NO_RSP;
    }
    if(addr ^ DEVICE_COM_BASE_BOAD_COBID)
    {
        coBid = DEVICE_COB_ID_MR_SDO + addr;
    }
    else
    {
        coBid = DEVICE_COM_BASE_BOAD_RSP_COBID + g_sDeviceParams.addr;
    }
    switch(cmd)
    {
        case DEVICE_COM_CMD_RESET:
            if(paramsLen == DEVICE_CAN_FRAME_NOPARAMS_LEN)
            {
                result = DEVICE_CAN_FRAME_RESULT_RSP;
                Device_RspFormatCanFrame(coBid, cmd, DEVICE_CAN_FRAME_RSP_OK,  NULL, NULL);
            }
        break;
        case DEVICE_COM_CMD_GET_VERSION:
            if(paramsLen == DEVICE_CAN_FRAME_GETVERSION_LEN)
            {
                u8 step = 0;
                
                step = *(pParams + DEVICE_CAN_RSP_POS_PARAMS);
                if(step < (DEVICE_VERSION_SIZE / DEVICE_VERSION_STEP_LEN))
                {
                    frame[pos++] = step;
                    memcpy(frame + pos,(u8 *)&DEVICE_VERSION + step * DEVICE_VERSION_STEP_LEN, DEVICE_VERSION_STEP_LEN);
                    pos += DEVICE_VERSION_STEP_LEN;
                    Device_RspFormatCanFrame(coBid, cmd, DEVICE_CAN_FRAME_RSP_OK, pos, frame);
                }
                else
                {
                    Device_RspFormatCanFrame(coBid, cmd, DEVICE_CAN_FRAME_RSP_PARAS_ERR, NULL, NULL);
                }
                result = DEVICE_CAN_FRAME_RESULT_RSP;
            }
        break;
          break;
        default:
            //Device_RspFormatCanFrame(coBid, cmd, DEVICE_CAN_FRAME_RSP_NO_SUPP, NULL, NULL);
            //result = DEVICE_CAN_FRAME_RESULT_RSP;
            break;
    }
    
    return result;
}



u8 Device_ProcessCobMRSdo(u32 addr, u8 cmd, u8 paramsLen, u8 *pParams, CAN_FRAME *txFrame)
{
    u8 result = DEVICE_CAN_FRAME_RESULT_NO_RSP;

    switch(cmd)
    {
        case DEVICE_COM_CMD_HEART:
        if(paramsLen == DEVICE_CAN_FRAME_BOOT_UP_LEN)
        {   
            DEVICE_INFO *pSubInfo = NULL;
        
            if(*(pParams + DEVICE_CAN_RSP_POS_PARAMS) == DEVICE_SUBDEVICE_STATUS_HEART_LOADING)
            {
                pSubInfo = Device_SubDeviceGet(addr);
                if(pSubInfo)
                {
                    pSubInfo->status = *(pParams + DEVICE_CAN_RSP_POS_PARAMS + 1);
                    pSubInfo->heartTime = g_nSysTick;
                }
            }
        }
        break;
        case DEVICE_COM_CMD_RESET:
        if(paramsLen == DEVICE_CAN_FRAME_NOPARAMS_LEN)
        {   
             result = DEVICE_CAN_FRAME_RESULT_CHKOK;
        }
        break;
        case DEVICE_COM_CMD_GET_VERSION:
        if(paramsLen == DEVICE_CAN_FRAME_LIMITDATA_LEN)
        {
            u8 step = 0;
            DEVICE_INFO *pSunInfo = NULL;
            
            step = *(pParams + DEVICE_CAN_RSP_POS_PARAMS);
            if(addr == (txFrame->coBid & DEVICE_COB_ID_MASK_ADDR) && (step == txFrame->buffer[DEVICE_CAN_RSP_POS_PARAMS]))
            {
                pSunInfo = Device_SubDeviceGet(addr);
                if(pSunInfo)
                {
                    memcpy(pSunInfo->version + step * DEVICE_VERSION_STEP_LEN, pParams + 3, DEVICE_VERSION_STEP_LEN);
                }
                else
                {
                    step = NULL;
                }
                result = DEVICE_CAN_FRAME_RESULT_CHKOK;
            }
        }
        case DEVICE_COM_CMD_OUTPUT_CTR :
        if(paramsLen == DEVICE_CAN_FRAME_NOPARAMS_LEN)
        {
            if(addr == (txFrame->coBid & DEVICE_COB_ID_MASK_ADDR))
            {
                result = DEVICE_CAN_FRAME_RESULT_CHKOK;
            }
        }
        break;
		case DEVICE_COM_CMD_ANT_CTR :
        if(paramsLen == DEVICE_CAN_FRAME_NOPARAMS_LEN)
        {
            if(addr == (txFrame->coBid & DEVICE_COB_ID_MASK_ADDR))
            {
                result = DEVICE_CAN_FRAME_RESULT_CHKOK;
            }
        }
        break;
        default:
        break;
    }
    
    return result;
}

u8 Device_ProcessCobSTSdo(u32 addr, u8 cmd, u8 paramsLen, u8 *pParams)
{
    u8 result = DEVICE_CAN_FRAME_RESULT_NO_RSP;
    
    switch(cmd)
    {
        #if I_AM_MASTER
            case DEVICE_COM_CMD_HEART:
            if(paramsLen == DEVICE_CAN_FRAME_NOPARAMS_LEN + 1)
            {   
                DEVICE_INFO *pSubInfo = NULL;
            
                pSubInfo = Device_SubDeviceGet(addr);
                if(pSubInfo)
                {
                    pSubInfo->addr = addr;
                    pSubInfo->heartTime = g_nSysTick;
                    pSubInfo->err = *(pParams + DEVICE_CAN_RSP_POS_PARAMS);
                }
                
                Device_RspFormatCanFrame(DEVICE_COB_ID_SR_SDO + addr, cmd, DEVICE_CAN_FRAME_RSP_OK, NULL, NULL);
                result = DEVICE_CAN_FRAME_RESULT_RSP;
            }
            break;
            case DEVICE_COM_CMD_BOOT_UP:
            if(paramsLen == DEVICE_CAN_FRAME_BOOT_UP_LEN)
            {   
                DEVICE_INFO *pSubInfo = NULL;
            
                pSubInfo = Device_SubDeviceGet(addr);
                if(pSubInfo)
                {
                    pSubInfo->addr = addr;
                    pSubInfo->type = *(pParams + DEVICE_CAN_RSP_POS_PARAMS);
                    pSubInfo->err = pSubInfo->status = *(pParams + DEVICE_CAN_RSP_POS_PARAMS + 1);
                }
                else 
                {
                    if(g_sDeviceSubInfo.num < DEVICE_SUBDEVICE_MAX_NUM)
                    {
                        pSubInfo = g_sDeviceSubInfo.subInfo + g_sDeviceSubInfo.num,
                        
                        pSubInfo->addr = addr;
                        pSubInfo->type = *(pParams + DEVICE_CAN_RSP_POS_PARAMS);
                        pSubInfo->err = pSubInfo->status = *(pParams + DEVICE_CAN_RSP_POS_PARAMS + 1);
                        g_sDeviceSubInfo.num++;
                    }
                }

                Device_RspFormatCanFrame(DEVICE_COB_ID_SR_SDO + addr, cmd, DEVICE_CAN_FRAME_RSP_OK, NULL, NULL);
                result = DEVICE_CAN_FRAME_RESULT_RSP;
            }
            break;
            
			case DEVICE_COM_CMD_RF_AMAR_INFO:
            if(paramsLen == 6)
            {   
            	u8 result = 0, ant = 0, pos = 0;

				ant = *(pParams + DEVICE_CAN_RSP_POS_PARAMS + pos++);
				result = *(pParams + DEVICE_CAN_RSP_POS_PARAMS + pos++);
				if(ant == g_sDeviceOp.rfInfo.antPort)
				{
                	Device_RspFormatCanFrame(DEVICE_COB_ID_SR_SDO + addr, cmd, DEVICE_CAN_FRAME_RSP_OK, NULL, NULL);
                	result = DEVICE_CAN_FRAME_RESULT_RSP;

                    if(g_sDeviceOp.rfInfo.state == DEVICE_RF_OP_STATE_WAIT || g_sDeviceOp.rfInfo.state == DEVICE_RF_OP_STATE_OPEN_ANT_AND_OP)
                    {
                        if(result)
                        {//报警，拉取数据
                            g_sDeviceOp.rfInfo.state = DEVICE_RF_OP_STATE_GET_AMAR_DATA;    
                            g_sDeviceOp.rfInfo.amar = result;
                            g_sDeviceOp.rfInfo.amarValue = *(pParams + DEVICE_CAN_RSP_POS_PARAMS + pos++); 
                            g_sDeviceOp.rfInfo.opAddr = *(pParams + DEVICE_CAN_RSP_POS_PARAMS + pos++);
                            g_sDeviceOp.rfInfo.opLen = *(pParams + DEVICE_CAN_RSP_POS_PARAMS + pos++);
                        }
                        else
                        {
                            g_sDeviceOp.rfInfo.state = DEVICE_RF_OP_STATE_ANT_OP_OK;
                        }
                    }
				}
            }
            break;
        #else
            __NOP();
        #endif
        default:
        break;
    }
    
    return result;
}

u8 Device_ProcessCobSRSdo(u32 addr, u8 cmd, u8 paramsLen, u8 *pParams)
{
    u8 result = DEVICE_CAN_FRAME_RESULT_NO_RSP;
    u8 rspResult = 0;
    
    rspResult = *(pParams + 1);
    switch(cmd)
    {
        case DEVICE_COM_CMD_BOOT_UP:
            if(paramsLen == 4)
            {   
                if(rspResult == DEVICE_CAN_FRAME_RSP_OK)
                {
                    result = DEVICE_CAN_FRAME_RESULT_CHKER;
                }
            }
            break;
        default:
                break;
    }
    return result;
}

void Device_RspFormatCanFrame(u32 coBid, u8 cmd, u8 result, u8 len, u8 *pBuffer)
{
    CAN_FRAME *pTxInfo = {0};
    u8 pos = 0;
    
    pTxInfo = &g_sCanRspFrame; 
    pTxInfo->coBid = coBid;
    pTxInfo->cmd = cmd;
    if(len <= (CAN_FRAME_MAX_LEN - 2))
    {
        pTxInfo->buffer[pos++] = cmd;
        pTxInfo->buffer[pos++] = result;

        memcpy(pTxInfo->buffer + pos, pBuffer, len);
        pos += len;
        pTxInfo->len = pos;
    }
}


void Device_CanAutoTask()
{
    u8 frame[8] = {0}, pos = 0;
    u8 addr = 0;
    CAN_FRAME txFrame = {0};
    frame[pos ++] = DEVICE_COM_CMD_OUTPUT_CTR;
    frame[pos ++] = DEVICE_CAN_FRAME_RFU;
    
    frame[pos ++] = rand() % 0xFF;
    frame[pos ++] = rand() % 0x20;
    frame[pos ++] = rand() % 0x30;
    frame[pos ++] = rand() % 0x30;

    addr = rand() % 4;
    
    if(addr != 0x02 && addr != 0x03)
    {
        txFrame.coBid = DEVICE_COM_BASE_BOAD_COBID;
        txFrame.flag = CAN_FRAME_FLAG_NO_RSP;
    }
    else
    {
        txFrame.coBid = DEVICE_COB_ID_MT_SDO + addr;
        txFrame.flag = CAN_FRAME_FLAG_NEED_RSP;
    }
    
    memcpy(txFrame.buffer, frame, pos);
    txFrame.len = pos;
    xQueueSend(g_hCanTxQueue, &txFrame, portMAX_DELAY);
}

DEVICE_INFO *Device_SubDeviceGet(u8 addr)
{
    u8 j = 0;
    BOOL extiFlag = FALSE;
    DEVICE_INFO *pInfo = NULL;
    
    if(g_sDeviceSubInfo.num < DEVICE_SUBDEVICE_MAX_NUM)
    {
        if(g_sDeviceSubInfo.num)
        {
            for(j = 0; j < g_sDeviceSubInfo.num; j++)
            {
                if(addr == g_sDeviceSubInfo.subInfo[j].addr)
                {
                    extiFlag = TRUE;
                    break;
                }
            }
            
            if(extiFlag)
            {
                pInfo = g_sDeviceSubInfo.subInfo + j;
            }
        }
    }
    
    return pInfo;
}

void Device_CtrAnt(u8 addr, u8 antIndex)
{
    u8 frame[CAN_FRAME_MAX_LEN] = {0}, pos = 0;

    frame[pos++] = DEVICE_COM_CMD_ANT_CTR;
    frame[pos++] = DEVICE_CAN_FRAME_RFU;
    frame[pos++] = antIndex;
    Device_AddCanTxFrame(addr + DEVICE_COB_ID_MT_SDO, NULL, NULL, 3, frame);
}

void Device_OpTask()
{
    RF_OP *pROp = NULL;

    pROp = &g_sDeviceOp.rfInfo;

    if(pROp->state == DEVICE_RF_OP_STATE_IDLE)
    {   
        u8 rfDeviceNum = 0, mxDeviceNum = 0, mbDeviceNum = 0;
        u8 index = 0;
        for(index = 0; index < g_sDeviceSubInfo.num; index++)
        {
            if(g_sDeviceSubInfo.subInfo[index].status == DEVICE_SUBDEVICE_STATUS_STD_ONLINE)
            {
                if(g_sDeviceSubInfo.subInfo[index].type == DEVICE_SUBDEVICE_TYPE_MX)
                {
                    mxDeviceNum ++;
                }
                else if(g_sDeviceSubInfo.subInfo[index].type == DEVICE_SUBDEVICE_TYPE_RF)
                {
                    pROp->rfAddr[rfDeviceNum ++] = g_sDeviceSubInfo.subInfo[index].addr;
                }
                else if(g_sDeviceSubInfo.subInfo[index].type == DEVICE_SUBDEVICE_TYPE_MB)
                {
                    pROp->mxAddr[mbDeviceNum ++] = g_sDeviceSubInfo.subInfo[index].addr;
                }
            }
        }
        
        if(rfDeviceNum == 1)                //暂时仅处理一个RF的情况
        {
            pROp->antNum = ((mxDeviceNum + rfDeviceNum) * DEVICE_SUBDEVICE_ALONE_ANT_OP_NUM);
            pROp->antIndex = 0;
            pROp->state = DEVICE_RF_OP_STATE_START_OP;
        }
    }
    else if(pROp->state == DEVICE_RF_OP_STATE_START_OP)
    {
        pROp->state = DEVICE_RF_OP_STATE_OPEN_ANT_AND_OP;
        switch(pROp->antIndex)
        {
            case DEVICE_SUBDEVICE_ANT_0:
				pROp->antPort = DEVICE_SUBDEVICE_ANT_0;
                Device_CtrAnt(*(pROp->rfAddr + pROp->rfIndex), Device_SeltctAnt(DEVICE_SUBDEVICE_ANT_0));
            break;
            case DEVICE_SUBDEVICE_ANT_1:
				pROp->antPort = DEVICE_SUBDEVICE_ANT_1;
              Device_CtrAnt(*(pROp->rfAddr + pROp->rfIndex), Device_SeltctAnt(DEVICE_SUBDEVICE_ANT_1));
            break;
            case DEVICE_SUBDEVICE_ANT_2:
				pROp->antPort = DEVICE_SUBDEVICE_ANT_0 | DEVICE_SUBDEVICE_ANT_1;
              Device_CtrAnt(*(pROp->rfAddr + pROp->rfIndex), Device_SeltctAnt(DEVICE_SUBDEVICE_ANT_0) | Device_SeltctAnt(DEVICE_SUBDEVICE_ANT_1));
            break;
            case DEVICE_SUBDEVICE_ANT_3:
				pROp->antPort = DEVICE_SUBDEVICE_ANT_2;
              Device_CtrAnt(*(pROp->mxAddr + pROp->mxIndex), Device_SeltctAnt(DEVICE_SUBDEVICE_ANT_0));                     //先开启MX子端口
              Device_CtrAnt(*(pROp->rfAddr + pROp->rfIndex), Device_SeltctAnt(DEVICE_SUBDEVICE_ANT_2));                     //再开启RF母端口
            break;
            case DEVICE_SUBDEVICE_ANT_4:
				pROp->antPort = DEVICE_SUBDEVICE_ANT_2;
              Device_CtrAnt(*(pROp->mxAddr + pROp->mxIndex), Device_SeltctAnt(DEVICE_SUBDEVICE_ANT_1));
              Device_CtrAnt(*(pROp->rfAddr + pROp->rfIndex), Device_SeltctAnt(DEVICE_SUBDEVICE_ANT_2));
            break;
            case DEVICE_SUBDEVICE_ANT_5:
				pROp->antPort = DEVICE_SUBDEVICE_ANT_2;
              Device_CtrAnt(*(pROp->mxAddr + pROp->mxIndex), Device_SeltctAnt(DEVICE_SUBDEVICE_ANT_0) | Device_SeltctAnt(DEVICE_SUBDEVICE_ANT_1));
              Device_CtrAnt(*(pROp->rfAddr + pROp->rfIndex), Device_SeltctAnt(DEVICE_SUBDEVICE_ANT_2));
            break;
            default:
              pROp->state = DEVICE_RF_OP_STATE_IDLE;
            break;
        }
    }
    else if(pROp->state == DEVICE_RF_OP_STATE_OPEN_ANT_AND_OP)
    {
        pROp->opTime = g_nSysTick;
        pROp->state = DEVICE_RF_OP_STATE_WAIT;
    }
    else if(pROp->state == DEVICE_RF_OP_STATE_WAIT)
    {
        if(pROp->opTime + 100 <=  g_nSysTick)
        {
            pROp->state = DEVICE_RF_OP_STATE_ANT_OP_FAIL;
        }
    }
    else if(pROp->state == DEVICE_RF_OP_STATE_ANT_OP_OK ||
            pROp->state == DEVICE_RF_OP_STATE_ANT_OP_FAIL)
    {
        pROp->state = DEVICE_RF_OP_STATE_CHANGE_ANT;
    }
    else if(pROp->state == DEVICE_RF_OP_STATE_CHANGE_ANT)
    {
        switch(pROp->antIndex)
        {
            case DEVICE_SUBDEVICE_ANT_0:
            case DEVICE_SUBDEVICE_ANT_1:
            case DEVICE_SUBDEVICE_ANT_2:
            case DEVICE_SUBDEVICE_ANT_3:
            case DEVICE_SUBDEVICE_ANT_4:
                pROp->antIndex ++;
            break;
            case DEVICE_SUBDEVICE_ANT_5:
                pROp->antIndex ++;
                pROp->mxIndex ++;
            break;
            default:
              pROp->state = DEVICE_RF_OP_STATE_IDLE;
            break;
        }
        
        if(pROp->antIndex < pROp->antNum)
        {
            pROp->state = DEVICE_RF_OP_STATE_START_OP;
        }
        else
        {
            pROp->opTime = g_nSysTick;
            pROp->state = DEVICE_RF_OP_STATE_SLEEP;
        }
    }
    else if(pROp->state == DEVICE_RF_OP_STATE_SLEEP)
    {
        if(pROp->opTime + 100 <=  g_nSysTick)
        {
            pROp->state = DEVICE_RF_OP_STATE_IDLE;
        }
    }
    else if(pROp->state == DEVICE_RF_OP_STATE_GET_AMAR_DATA)
    {
        
    
    }
}

void Device_GetSubDeviceVersion(u8 addr, CAN_FRAME *pTxFrame)
{
    pTxFrame->coBid = DEVICE_COB_ID_MT_SDO + addr;
    pTxFrame->len = 3;
    pTxFrame->flag = CAN_FRAME_FLAG_NEED_RSP;
    pTxFrame->buffer[0] = DEVICE_COM_CMD_GET_VERSION;
    pTxFrame->buffer[1] = DEVICE_CAN_FRAME_RFU;
    
    pTxFrame->buffer[2] = 0x00;
    xQueueSend(g_hCanTxQueue, pTxFrame, portMAX_DELAY);                         //可能毒死？？？
    
    pTxFrame->buffer[2] = 0x01;
    xQueueSend(g_hCanTxQueue, pTxFrame, portMAX_DELAY);
    
    pTxFrame->buffer[2] = 0x02;
    xQueueSend(g_hCanTxQueue, pTxFrame, portMAX_DELAY);
    
    pTxFrame->buffer[2] = 0x03;
    xQueueSend(g_hCanTxQueue, pTxFrame, portMAX_DELAY);
    
    pTxFrame->buffer[2] = 0x04;
    xQueueSend(g_hCanTxQueue, pTxFrame, portMAX_DELAY);
    
    pTxFrame->buffer[2] = 0x05;
    xQueueSend(g_hCanTxQueue, pTxFrame, portMAX_DELAY);
}

//-----------------------------------------------------

//因数据包短，硬件层右冲突校验且为多主从方案，考虑弃用一问一答方案，采用数据固定位和心跳来确定设备在线
void Device_CanRxDispatch(void *p)
{
    CAN_FRAME canRxFrame = {0};   
    CAN_FRAME canTxFrame = {0};
    u8 result = DEVICE_CAN_FRAME_RESULT_NO_RSP;
    u32 coBid = 0, addr = 0;
    u8 *pParams = NULL, paramsLen = 0;
    u8 rCmd = 0; 
    
    while(1)
    {
        if(xQueueReceive(g_hCanRxQueue, &canRxFrame, 1000) == pdTRUE)
        {
            CAN_FRAME *pRxFrame = &canRxFrame;
            
            rCmd = pRxFrame->buffer[CAN_RCV_FRAME_POS_CMD];
            pParams = pRxFrame->buffer;
            paramsLen = pRxFrame->len;                  //cmd不计入

            if(pRxFrame->coBid ^ DEVICE_COM_BASE_BOAD_COBID)
            {
                coBid = pRxFrame->coBid & DEVICE_COB_ID_MASK_ID;
                addr = pRxFrame->coBid & DEVICE_COB_ID_MASK_ADDR;
            }
            else
            {
                addr = coBid = DEVICE_COM_BASE_BOAD_COBID;
            }
            
            xQueuePeek(g_hCanTxQueue, &canTxFrame, 0);
            switch(coBid)
            {
                case DEVICE_COB_ID_MT_SDO:
                case DEVICE_COM_BASE_BOAD_COBID:
                    result = Device_ProcessCobMTSdo(addr, rCmd, paramsLen, pParams);
                break;
                case DEVICE_COB_ID_MR_SDO:
                    result = Device_ProcessCobMRSdo(addr, rCmd, paramsLen, pParams, &canTxFrame);
                break;
                case DEVICE_COB_ID_ST_SDO:
                    result = Device_ProcessCobSTSdo(addr, rCmd, paramsLen, pParams);
                break;
                case DEVICE_COB_ID_SR_SDO:
                  result = Device_ProcessCobSRSdo(addr, rCmd, paramsLen, pParams);
                break;
                case DEVICE_COM_BASE_BOAD_RSP_COBID:
                  result = DEVICE_CAN_FRAME_RESULT_NO_RSP;      
                break;
                default:
                break;    
            }
            
            if(result == DEVICE_CAN_FRAME_RESULT_RSP)
            {
                if(Can_TxFrames(&g_sCanRspFrame))
                {
                    if(g_sCanRspFrame.cmd == DEVICE_COM_CMD_RESET)
                    {
                        vTaskDelay(20);
                        Sys_SoftReset();
                    }
                }
            }
            else if(result == DEVICE_CAN_FRAME_RESULT_CHKOK)
            {
                if(canTxFrame.staus == CAN_FRAME_STATUS_REPORT)     //此帧需要上报
                {
                    memcpy(&g_sRspCanFrame, pRxFrame, sizeof(CAN_FRAME));
                }
                xSemaphoreGive(g_hCanComStatus);            //校验成功
            }
        }
    }
}


void Device_CanTxDispatch(void *p)
{
    CAN_FRAME canTxFrame = {0};  
    BOOL result = FALSE;    
    u8 repat = 0;
    const u32 canTxDelayTime = pdMS_TO_TICKS(21);
    const u32 canTxRepatTime = pdMS_TO_TICKS(100);
    while(1)
    {
        if(xQueuePeek(g_hCanTxQueue, &canTxFrame, portMAX_DELAY) == pdTRUE)                     //可能堵塞死？？？
        {
            CAN_FRAME *pTxFrame = &canTxFrame;
            
            if(Can_TxFrames(pTxFrame))
            {
                result = FALSE;
                if(pTxFrame->flag == CAN_FRAME_FLAG_NEED_RSP)
                {
                    vTaskDelay(canTxDelayTime);                                                     //实际八字节以内交互时间相当短
                    if(xSemaphoreTake(g_hCanComStatus, canTxRepatTime) == pdTRUE)                   //别毒死
                    {
                        result = TRUE;
                        repat = 0;
                        if(canTxFrame.staus == CAN_FRAME_STATUS_REPORT)                             //此帧需要上报
                        {
                            Device_DtuCanOk();
                            xSemaphoreGive(g_hUartComStatus); 
                        }
                    }
                    else
                    {
                        repat++;
                        if(repat >= CAN_TX_REPET_TICK)
                        {
                            result = TRUE;
                            repat = 0;
                            if(canTxFrame.staus == CAN_FRAME_STATUS_REPORT)                         //此帧需要上报
                            {
                                Device_DtuCanErr();
                                xSemaphoreGive(g_hUartComStatus); 
                            }
                        }
                    }
                    
                    if(result)
                    {
                        if(xQueueReceive(g_hCanTxQueue, &canTxFrame, portMAX_DELAY) == pdFALSE)         //校验成功，清除队列数据。此处校验队列问题
                        {
                            result = TRUE;
                        }
                    }
                }
                else
                {
                    if(xQueueReceive(g_hCanTxQueue, &canTxFrame, portMAX_DELAY) == pdFALSE)
                    {
                        result = FALSE;
                    }
                }
            }
            else
            {
                result = FALSE;
            }
        }
    }
}

void Device_HeartDispatch(void *p)
{
    CAN_FRAME canTxFrame = {0};
    u8 index = 0;
    const u8 heartFrame[3] = {0x00, 0x00, 0x40};
    CAN_FRAME *pTxFrame = NULL;
    DEVICE_INFO *pDevInfo = NULL;
    const u32 heartLiveTime = pdMS_TO_TICKS(10000);
    const u32 heartChkTime = pdMS_TO_TICKS(5000);
    while(1)
    {
        if(g_sDeviceSubInfo.num)
        {
            pTxFrame = &canTxFrame;
            
            for(index = 0; index < g_sDeviceSubInfo.num; index++)
            {
                pDevInfo = g_sDeviceSubInfo.subInfo + index;
                if(pDevInfo->status == DEVICE_SUBDEVICE_STATUS_HEART_LOADING)
                {
                    if(pDevInfo->heartTime + heartLiveTime<= g_nSysTick)
                    {
                        pDevInfo->status = DEVICE_SUBDEVICE_STATUS_OFFLINE;
                    }
                }
                else if(pDevInfo->status == DEVICE_SUBDEVICE_STATUS_STD_ONLINE)
                {
                    Device_GetSubDeviceVersion(pDevInfo->addr, pTxFrame);
                    pDevInfo->status = DEVICE_SUBDEVICE_STATUS_HEART_LOADING;
                }
            }

            pTxFrame->flag = CAN_FRAME_FLAG_NO_RSP;
            pTxFrame->coBid = DEVICE_COM_BASE_BOAD_COBID;
            memcpy(pTxFrame->buffer, heartFrame, 3);
            pTxFrame->len = 3;
            xQueueSend(g_hCanTxQueue, &canTxFrame, portMAX_DELAY);
        }
        vTaskDelay(heartChkTime); 
    }
}
void Device_MainDispatch(void *p)
{
    while(1)
    {

        vTaskDelay(2000);
        Device_CanAutoTask();                   //test
    }
}

void Device_UartRxDispatch(void *p)
{
    UART_RCVFRAME uartRxFreme = {0};
    UART_TXFRAME uartTxFreme = {0};
    const u32 uartTxDelayTime = pdMS_TO_TICKS(100);
    u16 startPos = 0;
    BOOL repot = TRUE;
    while(1)
    {
        if(xQueuePeek(g_hUartRxQueue, &uartRxFreme, portMAX_DELAY) == pdTRUE)
        {
            if(uartRxFreme.length >= UART_FRAME_MIN_LEN)
            {
                if(Uart_UsrCheckFrame(uartRxFreme.buffer, uartRxFreme.length, &startPos))
                {
                    if(repot)
                    {
                        uartTxFreme.len = Reader_ProcessUartFrame(uartRxFreme.buffer + startPos, &uartTxFreme);
                        if(uartTxFreme.result == UART_COM_RESULT_NEED_RSP)
                        {
                            uartTxFreme.com = uartRxFreme.com;
                            xQueueSend(g_hUartTxQueue, &uartTxFreme, portMAX_DELAY);
                            xQueueReceive(g_hUartRxQueue, &uartRxFreme, portMAX_DELAY);
                        }
                        else if(uartTxFreme.result == UART_COM_RESULT_NO_RSP)
                        {
                            repot = FALSE;
                            Can_ClearRspInfo();
                        }
                    }
                    else
                    {
                        if(xSemaphoreTake(g_hUartComStatus, uartTxDelayTime) == pdTRUE)
                        {
                            repot = TRUE;
                            g_sUartTxFrame.com = uartRxFreme.com;
                            g_sUartTxFrame.len = Device_ResponseFrame(g_sRspCanFrame.buffer, g_sRspCanFrame.len, &g_sUartTxFrame);
                            xQueueReceive(g_hUartRxQueue, &uartRxFreme, portMAX_DELAY);
                            xQueueSend(g_hUartTxQueue, &g_sUartTxFrame, portMAX_DELAY);
                        }
                    }
                }
            }
        }
        else
        {
        
        }
    }
}

void Device_UartTxDispatch(void *p)
{
    UART_TXFRAME uartTxFreme = {0};

    while(1)
    {
        if(xQueueReceive(g_hUartTxQueue, &uartTxFreme, portMAX_DELAY) == pdTRUE)                //正常来说除非特例，否则不用特别毒死
        {
            if(uartTxFreme.com == UART_COM_232)
            {
                Uart_EnableTxDma(uartTxFreme.buffer, uartTxFreme.len);
            }
            else if(uartTxFreme.com == UART_COM_LAN)
            {
                Lan_EnableTxDma(uartTxFreme.buffer, uartTxFreme.len);
            }
        }
        else
        {
        
        }
    }
}

void Device_HLDispatch(void *p)
{
    while(1)
    {
        if(USART_GetFlagStatus(UART_PORT, USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE))
        {
            USART_ITConfig(UART_PORT, USART_IT_RXNE, DISABLE);
            USART_ClearFlag(UART_PORT, USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE);
            vTaskDelay(10);
            Uart_DisableRxDma();
            
            Uart_Init(UART_BAUDRARE);           //清除上电所创队列，否则栈空间一直压缩
        }
    
    
    }
}