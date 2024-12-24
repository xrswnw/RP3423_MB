#include "AnyID_CanFestival_RS485_HL.h"

#define RS485_INT_CHANNEL        UART4_IRQn
const PORT_INF RS485_PORT_TX = {GPIOC, GPIO_Pin_10};
const PORT_INF RS485_PORT_RX = {GPIOC, GPIO_Pin_11};
const PORT_INF UART_485_CTRL = {GPIOC, GPIO_Pin_12};
void Rs485_InitInterface(u32 baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
	
    //GPIO_DeInit(RS485_PORT_TX.Port);
    USART_DeInit(RS485_PORT);

    GPIO_InitStructure.GPIO_Pin = RS485_PORT_TX.Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(RS485_PORT_TX.Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = RS485_PORT_RX.Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(RS485_PORT_RX.Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = UART_485_CTRL.Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(UART_485_CTRL.Port, &GPIO_InitStructure);
    Rs485_EnableTx();

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    //Configure USART
    USART_Init(RS485_PORT, &USART_InitStructure);
    // Enable the USART
    USART_Cmd(RS485_PORT, ENABLE);
}


void Rs485_InitFloating()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = RS485_PORT_TX.Pin | RS485_PORT_RX.Pin;// | UART_485_CTRL.Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    
    GPIO_Init(RS485_PORT_RX.Port, &GPIO_InitStructure);
    Rs485_EnableTx();
}


void Rs485_ConfigInt(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = RS485_INT_CHANNEL;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_UART_RX >> 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_UART_RX & 0x03;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void Rs485_EnableInt(FunctionalState rxState, FunctionalState txState)
{
    USART_ClearITPendingBit(RS485_PORT, USART_IT_RXNE | USART_IT_TC);

    USART_ITConfig(RS485_PORT, USART_IT_RXNE, rxState);
    USART_ITConfig(RS485_PORT, USART_IT_TC, txState);
}
void Rs485_Delayms(u32 n)
{
    n *= 0x5400;
    n++;
    while(n--);
}
void Rs485_WriteByte(u8 ch)
{
	
    while(((RS485_PORT)->SR & USART_FLAG_TXE) == (u16)RESET);
	(RS485_PORT)->DR = (ch & (u16)0x01FF);
}

void Rs485_WriteBuffer(u8 *pFrame, u16 len)
{
    u16 i = 0;
    Rs485_EnableTx();
    Rs485_Delayms(1);
    for(i = 0; i < len; i++)
    {
		Rs485_WriteByte(pFrame[i]);
    }
    Rs485_ChkTxOver();
    Rs485_Delayms(1);
    Rs485_EnableRx();
}

void Rs485_WriteErrStr(char *str)
{
    Rs485_WriteBuffer((u8 *)str, strlen(str));
    Rs485_WriteByte('\r');
    Rs485_WriteByte('\n');
}

void Rs485_WriteWarStr(char *str)
{
    Rs485_WriteBuffer((u8 *)str, strlen(str));
    Rs485_WriteByte('\r');
    Rs485_WriteByte('\n');
    
}



