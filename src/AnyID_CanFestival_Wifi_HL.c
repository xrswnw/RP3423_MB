#include "AnyID_CanFestival_Wifi_HL.h"

#define WIFI_INT_CHANNEL        USART2_IRQn
const PORT_INF WIFI_PORT_TX = {GPIOA, GPIO_Pin_2};
const PORT_INF WIFI_PORT_RX = {GPIOA, GPIO_Pin_3};
const PORT_INF WIFI_PORT_EN = {GPIOB, GPIO_Pin_5};

void Wifi_InitInterface(u32 baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    USART_DeInit(WIFI_PORT);

    GPIO_InitStructure.GPIO_Pin = WIFI_PORT_TX.Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(WIFI_PORT_TX.Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = WIFI_PORT_RX.Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(WIFI_PORT_RX.Port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    
    GPIO_InitStructure.GPIO_Pin = WIFI_PORT_EN.Pin;
    GPIO_Init(WIFI_PORT_EN.Port, &GPIO_InitStructure);
    WIFI_PORT_EN.Port->BSRR = WIFI_PORT_EN.Pin;
    
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    //Configure USART
    USART_Init(WIFI_PORT, &USART_InitStructure);
    // Enable the USART
    USART_Cmd(WIFI_PORT, ENABLE);

    USART_ITConfig(WIFI_PORT, USART_IT_IDLE, ENABLE);
    USART_DMACmd(WIFI_PORT, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(WIFI_PORT, USART_DMAReq_Rx, ENABLE);
}


void Wifi_ConfigInt(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = WIFI_INT_CHANNEL;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_UART_RX >> 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_UART_RX & 0x03;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void Wifi_EnableInt(FunctionalState rxState, FunctionalState txState)
{
    USART_ClearITPendingBit(WIFI_PORT, USART_IT_RXNE | USART_IT_TC);

    USART_ITConfig(WIFI_PORT, USART_IT_RXNE, rxState);
    USART_ITConfig(WIFI_PORT, USART_IT_TC, txState);
}

void Wifi_InitTxDma(u8 *pTxBuffer, u32 len)
{
    NVIC_InitTypeDef  NVIC_InitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};
    
    //DMA1ͨ��5����
    DMA_DeInit(WIFI_TXDMA_CH);
    //�����ַ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(WIFI_PORT->DR));

    //�ڴ��ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)pTxBuffer;
    //dma���䷽����
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    //����DMA�ڴ���ʱ�������ĳ���
    DMA_InitStructure.DMA_BufferSize = len;
    //����DMA���������ģʽ��һ������
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //����DMA���ڴ����ģʽ
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //���������ֳ�
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    //�ڴ������ֳ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    //����DMA�Ĵ���ģʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    //����DMA�����ȼ���
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    //����DMA��2��memory�еı����������
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(WIFI_TXDMA_CH, &DMA_InitStructure);

    DMA_ITConfig(WIFI_TXDMA_CH, DMA_IT_TC, ENABLE);  //open DMA send inttrupt
    Wifi_DisableTxDma();// �ر�DMA
    
    //dma interrupt
    NVIC_InitStructure.NVIC_IRQChannel = WIFI_TXDMA_INT;    //

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_UART_DMA >> 2;    //
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_UART_DMA & 0x03;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void Wifi_InitRxDma(u8 *pRxBuffer, u32 len)
{
    NVIC_InitTypeDef  NVIC_InitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};

    DMA_DeInit(WIFI_RXDMA_CH);
    //�����ַ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(WIFI_PORT->DR));
        
    //�ڴ��ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)pRxBuffer;
    //dma���䷽����
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //����DMA�ڴ���ʱ�������ĳ���
    DMA_InitStructure.DMA_BufferSize = len;
    //����DMA���������ģʽ��һ������
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //����DMA���ڴ����ģʽ
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //���������ֳ�
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    //�ڴ������ֳ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    //����DMA�Ĵ���ģʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    //����DMA�����ȼ���
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    //����DMA��2��memory�еı����������
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    
    DMA_Init(WIFI_RXDMA_CH, &DMA_InitStructure);
    DMA_ITConfig(WIFI_RXDMA_CH, DMA_IT_TC, ENABLE);  //open DMA send inttrupt
    Wifi_DisableRxDma();// �ر�DMA
    
    //dma interrupt
    NVIC_InitStructure.NVIC_IRQChannel = WIFI_RXDMA_INT;   

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_UART_DMA >> 2;    //
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_UART_DMA & 0x03;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);

}

void Wifi_WriteByte(u8 ch)
{
    while(((WIFI_PORT)->SR & USART_FLAG_TXE) == (u16)RESET);
	(WIFI_PORT)->DR = (ch & (u16)0x01FF);
}

void Wifi_Delayms(u32 n)
{
    n *= 0x5400;
    n++;
    while(n--);
}

void Wifi_WriteBuffer(u8 *pFrame, u16 len)
{
    u16 i = 0;

    for(i = 0; i < len; i++)
    {
		Wifi_WriteByte(pFrame[i]);
    }
    Wifi_ChkTxOver();
}

void Wifi_WriteStr(char *str)
{
    Wifi_WriteBuffer((u8 *)str, strlen(str));
    Wifi_WriteByte('\r');
    Wifi_WriteByte('\n');
} 




