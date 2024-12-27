#include "AnyID_CanFestival_Can_HL.h"

const PORT_INF CAN_RX_PORT = {GPIOA, GPIO_Pin_11};
const PORT_INF CAN_TX_PORT = {GPIOA, GPIO_Pin_12};

CAN_FRAME g_sCanRspFrame = {0};
BOOL Can_InitInterface(u8 psc)
{
	BOOL bOk = FALSE;
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	CAN_InitTypeDef  CAN_InitStructure = {0};
	CAN_FilterInitTypeDef CAN_FilterInitStructure = {0};
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PORT.Pin;
	
	GPIO_Init(CAN_RX_PORT.Port, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = CAN_TX_PORT.Pin;
    
	GPIO_Init(CAN_TX_PORT.Port, &GPIO_InitStructure);	
	//can硬件参数
	//v 典型值：1000K、800k、500K、250k、125k、50k、20k、10k
	//v = PLCK1 / (psc * (tbs1 + 1 + tbs2 + 1 + 1)) = 60/(psc *(1 + 1 + 1 + 1+ 1)) 。 默认 500k
    CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
	CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;
	CAN_InitStructure.CAN_Prescaler = psc;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_ABOM = ENABLE;   								        // 硬件层自动复位
	CAN_InitStructure.CAN_AWUM = DISABLE;   								// 睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART = DISABLE;   								    // 使用报文自动传送 
	CAN_InitStructure.CAN_RFLM = DISABLE;   								// 报文不锁定,新的覆盖旧的 
	CAN_InitStructure.CAN_TTCM = DISABLE;									// 非时间触发通信模式  
	CAN_InitStructure.CAN_TXFP = DISABLE;   								// 优先级由报文标识符决定 
	
	if(CAN_Init(CAN_HARDPORT, &CAN_InitStructure) == CANINITOK)		
	{ 	
        CAN_FilterInitStructure.CAN_FilterNumber = 0;                               // 过滤器0	
        CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;                          // 32位ID
        CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
        CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;                      // 32位MASK
        CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;   
        CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;             // 掩码模式
        CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;            // 32位 
        CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;         // 过滤器0关联到FIFO0
        CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;				        // 激活过滤器0

		CAN_FilterInit(&CAN_FilterInitStructure);  	
		
		CAN_ITConfig(CAN_HARDPORT, CAN_IT_FMP0 | CAN_IT_ERR, ENABLE);                            //清除上电置位FLAG
		
		NVIC_InitStructure.NVIC_IRQChannel = CAN_INT_CHANNEL;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	else
	{
		bOk = TRUE;
	}
	
	return bOk;
}


BOOL Can_TxBuffer(CanTxMsg *pTxInfo)
{
    BOOL result = FALSE;
    u32 ot = 0;
    u8 mailBox = 0;

    mailBox = CAN_Transmit(CAN_HARDPORT, pTxInfo);
    while(CAN_TransmitStatus(CAN_HARDPORT, mailBox) != CANTXOK && ot < CAN_SEND_TIMEOUT)
    {
        ot++;
    }
    
    if(ot <= CAN_SEND_TIMEOUT)
    {
        result = TRUE;
    }

    return result;
}

BOOL CanTxTransm(CanTxMsg *pFrame)
{
	BOOL result = FALSE;
    u32 ot = 0;
    u8 mailBox = 0;
   
    mailBox = CAN_Transmit(CAN_HARDPORT, pFrame);
    while(CAN_TransmitStatus(CAN_HARDPORT, mailBox) != CANTXOK && ot < CAN_SEND_TIMEOUT)
    {
        ot++;
    }
    
    if(ot <= CAN_SEND_TIMEOUT)
    {
        result = TRUE;
    }
	
    return result;
}


void Can_Delay100us(u32 t)
{
    t *= 0x600; 
    t++;
    while(t--);
}

BOOL Can_TxFrames(CAN_FRAME *pFrame)
{
    BOOL result = FALSE;
    u32 errTick = 0;
    u32 ot = 0;
    u8 mailBox = 0;
    CanTxMsg txInfo = {0};

    txInfo.StdId = pFrame->coBid;
    txInfo.DLC = pFrame->len;
    memcpy(txInfo.Data, pFrame->buffer, txInfo.DLC);
    
    mailBox = CAN_Transmit(CAN_HARDPORT, &txInfo);
    while(CAN_TransmitStatus(CAN_HARDPORT, mailBox) != CANTXOK && ot <= CAN_SEND_TIMEOUT)
    {
        ot++;
    }

    if(ot >= CAN_SEND_TIMEOUT)
    {//也可拉取错误位单独处理，暂时不采用
        do{
                vTaskDelay(2);
                result = Can_TxFramesRepat(pFrame);
                errTick++;
            }while(errTick <= 3 && !result);
    }
    else
    {
         result = TRUE;
    }

    return result;
}


BOOL Can_TxFramesRepat(CAN_FRAME *pFrame)
{
    BOOL result = FALSE;
    u32 ot = 0;
    u8 mailBox = 0;
    CanTxMsg txInfo = {0};
    
    txInfo.StdId = pFrame->coBid;
    txInfo.DLC = pFrame->len;
    memcpy(txInfo.Data, pFrame->buffer, txInfo.DLC);

    mailBox = CAN_Transmit(CAN_HARDPORT, &txInfo);
    while(CAN_TransmitStatus(CAN_HARDPORT, mailBox) != CANTXOK && ot < CAN_SEND_TIMEOUT)
    {
        ot++;
    }

    if(ot < CAN_SEND_TIMEOUT)
    {
        result = TRUE;
    }

    return result;
}

QueueHandle_t g_hCanRxQueue = NULL;
QueueHandle_t g_hCanTxQueue = NULL;
QueueHandle_t g_hCanComStatus = NULL;

CAN_FRAME g_sRspCanFrame = {0};
BOOL Can_QueueCreatTask()
{
    BOOL bOk = FALSE;
    
    g_hCanRxQueue = xQueueCreate(CAN_RX_MAX_SIZE, sizeof(CAN_FRAME));
    g_hCanTxQueue = xQueueCreate(CAN_TX_MAX_SIZE, sizeof(CAN_FRAME)); 
    g_hCanComStatus = xSemaphoreCreateBinary();
    if(g_hCanRxQueue && g_hCanTxQueue && g_hCanComStatus)
    {
        bOk = TRUE;
        //xSemaphoreGive(g_hCanComStatus);
    }

    return bOk;
}