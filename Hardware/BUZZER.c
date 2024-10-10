#include "stm32f10x.h"                  // Device header
#include "DELAY.H"
#include "BUZZER.H"

void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

void Buzzer_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void Buzzer_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

void Buzzer_Alarm(void)
{
		Buzzer_ON();
		Delay_ms(15);
		Buzzer_OFF();
		
}

void Buzzer_Alarm1(void)
{
	Buzzer_ON();
	Delay_ms(100);
	Buzzer_OFF();
		

}
