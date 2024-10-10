#include "stm32f10x.h"                  // Device header
#include "LED.H"


void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
}


void LED_ON_OFF(LED_Num LEDx, uint8_t ON_OFF)
{
	if (ON_OFF)
	{
		switch (LEDx)
		{
			case LED1:
				GPIO_ResetBits(GPIOA, GPIO_Pin_4);
				break;
			case LED2:
				GPIO_ResetBits(GPIOA, GPIO_Pin_5);
				break;
			case LED3:
				GPIO_ResetBits(GPIOA, GPIO_Pin_6);
				break;
			case LED4:
				GPIO_ResetBits(GPIOA, GPIO_Pin_7);
				break;
		}
	}
	else
	{
		switch (LEDx)
		{
			case LED1:
				GPIO_SetBits(GPIOA, GPIO_Pin_4);
				break;
			case LED2:
				GPIO_SetBits(GPIOA, GPIO_Pin_5);
				break;
			case LED3:
				GPIO_SetBits(GPIOA, GPIO_Pin_6);
				break;
			case LED4:
				GPIO_SetBits(GPIOA, GPIO_Pin_7);
				break;
		}
	}
}


void LED_All_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	GPIO_SetBits(GPIOA, GPIO_Pin_6);
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
}
void LED_All_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	GPIO_ResetBits(GPIOA, GPIO_Pin_7);
}


void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_4) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	}
}

void LED2_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_5) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	}
}
