#include "AnyID_CanFestival_Uart_HL.h"

#define UART_INT_CHANNEL        USART1_IRQn
const PORT_INF UART_PORT_TX = {GPIOA, GPIO_Pin_9};
const PORT_INF UART_PORT_RX = {GPIOA, GPIO_Pin_10};

void Uart_InitInterface(u32 baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    USART_DeInit(UART_PORT);

    GPIO_InitStructure.GPIO_Pin = UART_PORT_TX.Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(UART_PORT_TX.Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = UART_PORT_RX.Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(UART_PORT_RX.Port, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    //Configure USART
    USART_Init(UART_PORT, &USART_InitStructure);
    // Enable the USART
    USART_Cmd(UART_PORT, ENABLE);
    
    //DMA
    USART_ITConfig(UART_PORT, USART_IT_IDLE, ENABLE);
    USART_DMACmd(UART_PORT, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(UART_PORT, USART_DMAReq_Rx, ENABLE);
}


void Uart_ConfigInt(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = UART_INT_CHANNEL;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_FREERTOS_UART;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_FREERTOS_UART;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void Uart_EnableInt(FunctionalState rxState, FunctionalState txState)
{
    USART_ClearITPendingBit(UART_PORT, USART_IT_RXNE | USART_IT_TC);

    USART_ITConfig(UART_PORT, USART_IT_RXNE, rxState);
    USART_ITConfig(UART_PORT, USART_IT_TC, txState);
}

void Uart_WriteByte(u8 ch)
{
    while(((UART_PORT)->SR & USART_FLAG_TXE) == (u16)RESET);
	(UART_PORT)->DR = (ch & (u16)0x01FF);
}

void Uart_Delayms(u32 n)
{
    n *= 0x3800;
    n++;
    while(n--);
}

void Uart_WriteBuffer(u8 *pFrame, u16 len)
{
    u16 i = 0;

    for(i = 0; i < len; i++)
    {
		Uart_WriteByte(pFrame[i]);
    }
    Uart_ChkTxOver();
}


void Uart_InitTxDma(u8 *pTxBuffer, u32 len)
{
    NVIC_InitTypeDef  NVIC_InitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};
    
    //DMA1ͨ��5����
    DMA_DeInit(UART_TXDMA_CH);
    //�����ַ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(UART_PORT->DR));
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

    DMA_Init(UART_TXDMA_CH, &DMA_InitStructure);

    DMA_ITConfig(UART_TXDMA_CH, DMA_IT_TC, ENABLE);  //open DMA send inttrupt
    Uart_DisableTxDma();// �ر�DMA
    
    //dma interrupt
    NVIC_InitStructure.NVIC_IRQChannel = UART_TXDMA_INT;    //

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_FREERTOS_UART;    //
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_FREERTOS_UART;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void Uart_InitRxDma(u8 *pRxBuffer, u32 len)
{
    NVIC_InitTypeDef  NVIC_InitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};

    DMA_DeInit(UART_RXDMA_CH);
    //�����ַ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(UART_PORT->DR));
        
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
    
    DMA_Init(UART_RXDMA_CH, &DMA_InitStructure);
    DMA_ITConfig(UART_RXDMA_CH, DMA_IT_TC, ENABLE);  //open DMA send inttrupt
    Uart_DisableRxDma();// �ر�DMA
    
    //dma interrupt
    NVIC_InitStructure.NVIC_IRQChannel = UART_RXDMA_INT;   

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_FREERTOS_UART;    //
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_FREERTOS_UART;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}



