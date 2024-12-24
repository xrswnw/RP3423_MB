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
}


void Lan_ConfigInt(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = LAN_INT_CHANNEL;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_UART_RX >> 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_UART_RX & 0x03;
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
    n *= 0x5400;
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




