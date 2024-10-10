/********************************************************************************
* @File  	: key.h
* @Author	: Liuzc
* @Version	: V4.0
* @Date		: Oct07-24
* @Description: 1、此文件定义了按键所需相关类型。
				2、使用宏定义了按键所需相关时间变量。
				3、声明了按键的应用层函数。
				
********************************************************************************/
#ifndef __KEY_H
#define __KEY_H
 
#include "stm32f10x.h"

// 按键按下下的状态
#define KEY_ON 0	// 按键按下为低电平，设置 KEY_ON=0，KEY_OFF=1
#define KEY_OFF 1   // 按键按下为高电平，设置 KEY_ON=1，KEY_OFF=0

// 按键的时间变量
#define KEY_CONFIRM_TIME	1		// 消抖时间
#define KEY_THREE_TIME		10		// 三击时间间隔
#define KEY_CLICK_TIME		15		// 单击时间间隔
#define KEY_LONGPRESS_TIM	30		// 长按时间

// 计算多少个按键
#define KEY_NUM_MAX (sizeof(GPIO_Init_List) / sizeof(GPIO_Init_List[0]))

// 定义初始化结构体方便初始化
typedef struct
{
	uint32_t RCC_APB2Perip;
	GPIO_TypeDef *GPIOx;
	uint16_t Pin;
	GPIOMode_TypeDef GPIO_Mode;
	GPIOSpeed_TypeDef GPIO_Speed;
}Key_GPIO_t;	

// 按键初始化配置结构体数组
static Key_GPIO_t GPIO_Init_List[] = 
{
	{RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_12, GPIO_Mode_IPU, GPIO_Speed_50MHz},  // key1
	{RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_13, GPIO_Mode_IPU, GPIO_Speed_50MHz},  // key2
	{RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_14, GPIO_Mode_IPU, GPIO_Speed_50MHz},  // key3
	{RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_15, GPIO_Mode_IPU, GPIO_Speed_50MHz}   // key4
};

// 定义按键的状态
typedef enum 
{
    KEY_RELEASE = 0,    		// 释放 0
    KEY_CLICK,     				// 短按 1
	KEY_DOUBLE,					// 双击 2
	KEY_THREECLICK,				// 三击 3
    KEY_LONGPRESS,      		// 长按 4
	KEY_CONFIRM         		// 消抖 5
}KEY_STATE;

// 定义按键信息
typedef struct
{
	KEY_STATE Key_State;		// 按键当前状态
	KEY_STATE Key_State_Tem;	// 按键临时状态
	
	uint8_t Is_Press_Pre;		// 前一个状态是否按压
	uint8_t Is_Press;			// ==1 当前按键按压
	uint8_t Click_Flag;			// 单击标志位 ==0空闲状态  ==1第一次单击 ==2在第一次单击期间内又有按键按下

	uint8_t Double_Timers;		// 双击时间间隔, 单击响应时间
	uint8_t Three_Timers;		// 三击时间间隔, 双击后时间内有按键按下三击
    uint8_t Press_Timers;		// 按键按下了多长时间
	
} KEY_INFO_t;

// 声明按键  --> 定义在Key.c中
extern KEY_INFO_t Key_Info_List[KEY_NUM_MAX];  
 

void Key_Use(void);
void Key_Init(void);

#endif /* __KEY_H */
