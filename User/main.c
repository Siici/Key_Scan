#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Timer.h"
#include "KEY.H"
#include "LED.H"

uint16_t Num, Num1, Num2;

uint8_t keyVal;
int main(void)
{
	OLED_Init();
	LED_Init();
	Timer_Init();
	Key_Init();
	OLED_ShowString(1, 1, "K1:");
	OLED_ShowString(2, 1, "K2:");
	OLED_ShowString(3, 1, "K3:");
	OLED_ShowString(4, 1, "K4:");
	while (1)
	{
		if (Num1)
		{
			Num1 = 0;
			for (uint8_t i= 0; i < 4; i++)
			{
				OLED_ShowNum(i + 1, 4,  Key_Info_List[i].Press_Timers,  2);		// 按压时间
				OLED_ShowNum(i + 1, 7,  Key_Info_List[i].Click_Flag,    1);		// 点击 双击标志
				OLED_ShowNum(i + 1, 9,  Key_Info_List[i].Double_Timers, 2);		// 双击时间
				OLED_ShowNum(i + 1, 12, Key_Info_List[i].Three_Timers,  2);		// 三击时间
				OLED_ShowNum(i + 1, 15, Key_Info_List[i].Key_State_Tem, 1);		// 按键状态
			}
			Key_Use();	
		}
	}
}

// 10ms 的定时频率
void TIM2_IRQHandler(void)		
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Num++;
		Num1 = 1;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
