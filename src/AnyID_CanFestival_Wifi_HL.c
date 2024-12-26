#include "AnyID_CanFestival_Wifi_HL.h"

#define WIFI_INT_CHANNEL        USART2_IRQn
const PORT_INF WIFI_PORT_TX = {GPIOA, GPIO_Pin_2};
const PORT_INF WIFI_PORT_RX = {GPIOA, GPIO_Pin_3}; 
const PORT_INF WIFI_PORT_RST = {GPIOB, GPIO_Pin_5};   
void Wifi_InitInterface(u32 baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin = WIFI_PORT_RST.Pin;
    GPIO_Init(WIFI_PORT_RST.Port, &GPIO_InitStructure);
	WIFI_RstLow();

    USART_DeInit(WIFI_PORT);

    GPIO_InitStructure.GPIO_Pin = WIFI_PORT_TX.Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(WIFI_PORT_TX.Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = WIFI_PORT_RX.Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(WIFI_PORT_RX.Port, &GPIO_InitStructure);

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
}

void Wifi_Delayms(u32 n)             //ÏµÍ³ÑÓÊ±nºÁÃë
{
    n *= 0x6000;
    n++;
    while(n--);
}

void Wifi_InitTimer(u32 baudrate)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    if(baudrate == 9600)
    {
        TIM_TimeBaseStructure.TIM_Period = WIFI_RCV_TIM_9600;
    }
    else
    {
        TIM_TimeBaseStructure.TIM_Period = WIFI_RCV_TIM_9600M;
    }
    TIM_TimeBaseStructure.TIM_Prescaler = WIFI_RCV_TIM_PRESCALER;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(WIFI_RCV_TIMER, &TIM_TimeBaseStructure);

    TIM_ITConfig(WIFI_RCV_TIMER, TIM_IT_Update, ENABLE);
    TIM_Cmd(WIFI_RCV_TIMER, DISABLE);

    NVIC_InitStructure.NVIC_IRQChannel = WIFI_RCV_TIMER_INT;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_FREERTOS_UART;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_FREERTOS_UART;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  	NVIC_Init(&NVIC_InitStructure);
}

void Wifi_ConfigInt(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    NVIC_InitStructure.NVIC_IRQChannel = WIFI_INT_CHANNEL;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_FREERTOS_UART;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_FREERTOS_UART;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void Wifi_EnableInt(FunctionalState rxState, FunctionalState txState)
{
    USART_ClearITPendingBit(WIFI_PORT, USART_IT_RXNE | USART_IT_TC);

    USART_ITConfig(WIFI_PORT, USART_IT_RXNE, rxState);
    USART_ITConfig(WIFI_PORT, USART_IT_TC, txState);
}

void Wifi_WriteByte(u8 ch)
{
    while(((WIFI_PORT)->SR & USART_FLAG_TXE) == (u16)RESET);
	(WIFI_PORT)->DR = (ch & (u16)0x01FF);
}

void Wifi_WriteBuffer(u8 *pBuffer, u32 len)
{
    u32 i = 0;
    for(i = 0; i < len; i++)
    {
        Wifi_WriteByte(pBuffer[i]);
    }
}


void Wifi_WriteAtCmd(char *str)
{
	Wifi_WriteBuffer((u8 *)str, strlen(str));
	Wifi_WriteByte(STR_HEX_CR);
	Wifi_WriteByte(STR_HEX_LF);
}

