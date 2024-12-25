#include "AnyID_CanFestival_Periph_HL.h"

const PORT_INF PERIPH_LED_1_PORT = {GPIOC, GPIO_Pin_8};
const PORT_INF PERIPH_LED_2_PORT = {GPIOC, GPIO_Pin_9};
const PORT_INF PERIPH_LED_3_PORT = {GPIOC, GPIO_Pin_7};
const PORT_INF PERIPH_LED_4_PORT = {GPIOC, GPIO_Pin_6};
const PORT_INF PERIPH_LED_5_PORT = {GPIOB, GPIO_Pin_14};
const PORT_INF PERIPH_LED_6_PORT = {GPIOB, GPIO_Pin_15};

const PORT_INF PERIPH_LED[PERIPH_LED_NUM] = {PERIPH_LED1, PERIPH_LED2, PERIPH_LED3, PERIPH_LED4, PERIPH_LED5, PERIPH_LED6};
u8 g_nPeriphLedStatus = 0;



void Periph_Delayms(u32 n)
{
    n *= 0x6000;
    n++;
    while(n--);
}

void Periph_InitInterface()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    for(u8 i = 0; i < PERIPH_LED_NUM; i++)
    {
        GPIO_InitStructure.GPIO_Pin = PERIPH_LED[i].Pin;
        GPIO_Init(PERIPH_LED[i].Port, &GPIO_InitStructure);
    }
}


void Periph_Test()
{
    u8 index = 0;

    index  =  rand() % (PERIPH_LED_NUM);
    if((1 << index) & g_nPeriphLedStatus)
    {
        Periph_Ledoff(index);
    }
    else
    {
        Periph_Ledon(index);
    }
}