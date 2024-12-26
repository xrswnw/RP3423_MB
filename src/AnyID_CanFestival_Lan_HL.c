#include "AnyID_CanFestival_Lan_HL.h"

#define LAN_INT_CHANNEL        USART3_IRQn
const PORT_INF LAN_PORT_TX = {GPIOB, GPIO_Pin_10};
const PORT_INF LAN_PORT_RX = {GPIOB, GPIO_Pin_11};
const PORT_INF LAN_PORT_RST = {GPIOB, GPIO_Pin_0};
const PORT_INF LAN_PORT_CFG = {GPIOB, GPIO_Pin_1};
void Lan_InitInterface(u32 baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    USART_DeInit(LAN_PORT);

    GPIO_InitStructure.GPIO_Pin = LAN_PORT_TX.Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(LAN_PORT_TX.Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LAN_PORT_RX.Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(LAN_PORT_RX.Port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    
    GPIO_InitStructure.GPIO_Pin = LAN_PORT_RST.Pin;
    GPIO_Init(LAN_PORT_RST.Port, &GPIO_InitStructure);
    LAN_PORT_RST.Port->BSRR = LAN_PORT_RST.Pin;
    
    GPIO_InitStructure.GPIO_Pin = LAN_PORT_CFG.Pin;
    GPIO_Init(LAN_PORT_CFG.Port, &GPIO_InitStructure);
    LAN_PORT_CFG.Port->BSRR = LAN_PORT_CFG.Pin;

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    //Configure USART
    USART_Init(LAN_PORT, &USART_InitStructure);
    // Enable the USART
    USART_Cmd(LAN_PORT, ENABLE);
    USART_ITConfig(LAN_PORT, USART_IT_IDLE, ENABLE);
    USART_DMACmd(LAN_PORT, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(LAN_PORT, USART_DMAReq_Rx, ENABLE);
}


void Lan_ConfigInt(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = LAN_INT_CHANNEL;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_FREERTOS_UART;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_FREERTOS_UART;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void Lan_EnableInt(FunctionalState rxState, FunctionalState txState)
{
    USART_ClearITPendingBit(LAN_PORT, USART_IT_RXNE | USART_IT_TC);

    USART_ITConfig(LAN_PORT, USART_IT_RXNE, rxState);
    USART_ITConfig(LAN_PORT, USART_IT_TC, txState);
}

void Lan_WriteByte(u8 ch)
{
    while(((LAN_PORT)->SR & USART_FLAG_TXE) == (u16)RESET);
	(LAN_PORT)->DR = (ch & (u16)0x01FF);
}

void Lan_Delayms(u32 n)
{
    n *= 0x6000;
    n++;
    while(n--);
}

void Lan_WriteBuffer(u8 *pFrame, u16 len)
{
    u16 i = 0;

    for(i = 0; i < len; i++)
    {
		Lan_WriteByte(pFrame[i]);
    }
    if(len)
    {
        Lan_ChkTxOver();
    }
}

void Lan_WriteStr(char *str)
{
    Lan_WriteBuffer((u8 *)str, strlen(str));
    Lan_WriteByte('\r');
    Lan_WriteByte('\n');
} 


void Lan_InitTxDma(u8 *pTxBuffer, u32 len)
{
    NVIC_InitTypeDef  NVIC_InitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};
    
    //DMA1通道5配置
    DMA_DeInit(LAN_TXDMA_CH);
    //外设地址
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(LAN_PORT->DR));
    //内存地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)pTxBuffer;
    //dma传输方向单向
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    //设置DMA在传输时缓冲区的长度
    DMA_InitStructure.DMA_BufferSize = len;
    //设置DMA的外设递增模式，一个外设
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //设置DMA的内存递增模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //外设数据字长
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    //内存数据字长
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    //设置DMA的传输模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    //设置DMA的优先级别
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    //设置DMA的2个memory中的变量互相访问
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(LAN_TXDMA_CH, &DMA_InitStructure);

    DMA_ITConfig(LAN_TXDMA_CH, DMA_IT_TC, ENABLE);  //open DMA send inttrupt
    Lan_DisableTxDma();// 关闭DMA
    
    //dma interrupt
    NVIC_InitStructure.NVIC_IRQChannel = LAN_TXDMA_INT;    //

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_FREERTOS_UART;    //
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_FREERTOS_UART;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void Lan_InitRxDma(u8 *pRxBuffer, u32 len)
{
    NVIC_InitTypeDef  NVIC_InitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};

    DMA_DeInit(LAN_RXDMA_CH);
    //外设地址
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(LAN_PORT->DR));
        
    //内存地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)pRxBuffer;
    //dma传输方向单向
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //设置DMA在传输时缓冲区的长度
    DMA_InitStructure.DMA_BufferSize = len;
    //设置DMA的外设递增模式，一个外设
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //设置DMA的内存递增模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //外设数据字长
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    //内存数据字长
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    //设置DMA的传输模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    //设置DMA的优先级别
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    //设置DMA的2个memory中的变量互相访问
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    
    DMA_Init(LAN_RXDMA_CH, &DMA_InitStructure);
    DMA_ITConfig(LAN_RXDMA_CH, DMA_IT_TC, ENABLE);  //open DMA send inttrupt
    Lan_DisableRxDma();// 关闭DMA
    
    //dma interrupt
    NVIC_InitStructure.NVIC_IRQChannel = LAN_RXDMA_INT;   

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_FREERTOS_UART;    //
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_FREERTOS_UART;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}



