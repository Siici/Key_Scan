#include "key.h"
#include "LED.H"
/********************************************************************************
* @File  	: key.c
* @Author	: Liuzc
* @Version	: V4.0
* @Date		: Oct07-24
* @Description: 1、此程序采用状态机的编程思想，实现独立按键的**单击**，**双击**，**三击**，**长按**。
				2、通过结构体定义了按键的相关属性再通过结构体数组统一管理存储所有的按键的相关状态。
				3、按键的计时通过对主函数的**​计数**判断，本次主函数采用定时器2进行10ms一次的循环。
				4、全键无冲支持多键同时按下。
********************************************************************************/

// 声明按键 
KEY_INFO_t Key_Info_List[KEY_NUM_MAX];   // 全局的静态变量自动赋初值 0


/**
 * @authors	LZC		@timer 7_Oct_24  
 * @brief 	初始化所有按键
 * @param	void
 * @retval 	void
 * @note	None
 */
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	for (uint8_t i = 0; i < KEY_NUM_MAX; i++)
	{
		RCC_APB2PeriphClockCmd(GPIO_Init_List[i].RCC_APB2Perip,ENABLE);
		GPIO_InitStructure.GPIO_Mode  = GPIO_Init_List[i].GPIO_Mode;
		GPIO_InitStructure.GPIO_Speed = GPIO_Init_List[i].GPIO_Speed;
		GPIO_InitStructure.GPIO_Pin   = GPIO_Init_List[i].Pin;
		GPIO_Init(GPIO_Init_List[i].GPIOx,&GPIO_InitStructure);
	}
}


/**
 * @authors	LZC		@timer 7_Oct_24  
 * @brief 	按键扫描判断按键是否按下 
 * 			-->若按下则在相应按键的结构体（Key_Info_List[index].Is_Press）中记录
 * @param	index:
 * 				  按键索引
 * @retval 	void
 * @note	1: 若移植到其它单片机需更改读取引脚的判断。
 */
static void Key_Scan(uint8_t index)
{	
	if (GPIO_ReadInputDataBit(GPIO_Init_List[index].GPIOx, GPIO_Init_List[index].Pin) == KEY_ON)
	{
		Key_Info_List[index].Is_Press = KEY_ON;
	}
	else 
	{
		Key_Info_List[index].Is_Press = KEY_OFF;
		Key_Info_List[index].Key_State = KEY_RELEASE;
	}
}


/**
 * @authors	LZC		@timer 7_Oct_24 
 * @brief 	按键单击双击三击长按的逻辑判断 
 * 			--> 判断的结果记录在相应按键的结构体（Key_Info_List[index].Key_State）中记录
 * @param	index:
 * 				  按键索引
 * @retval 	void
 * @note	1: 此函数不依赖硬件资源。
 * 			2: 此函数中计时部分，采用的是对程序的计数，1个Timer等于一个主程序周期。
 */
static void Key_Begin(uint8_t index)
{
	// 保存上次的按键值 
	Key_Info_List[index].Is_Press_Pre = Key_Info_List[index].Is_Press;
	// 扫描所有按键 更新按键值
	Key_Scan(index);

	/************************************************************************************************************/
	// 如果是从释放到按下 且 前面有一次按下 则是 第二次按下 
	if ((Key_Info_List[index].Is_Press != Key_Info_List[index].Is_Press_Pre) && (Key_Info_List[index].Is_Press == KEY_ON)) 
	{
		if ((Key_Info_List[index].Click_Flag == 1) )
		{
			// 第二次按下 且上次单击还未响应 --> 双击
			if ( (Key_Info_List[index].Double_Timers > 0) && (Key_Info_List[index].Double_Timers <= KEY_CLICK_TIME)) 
			{
				Key_Info_List[index].Click_Flag = 2;			// 双击了 
				Key_Info_List[index].Press_Timers = 0;
				Key_Info_List[index].Double_Timers = 0;
			}
		}
		else
		if ((Key_Info_List[index].Click_Flag == 2) )
		{
			// 第三次按下 且上次双击击还未响应 --> 三击
			if ( (Key_Info_List[index].Three_Timers > 0) && (Key_Info_List[index].Three_Timers <= KEY_CLICK_TIME)) 
			{
				Key_Info_List[index].Click_Flag = 0;
				Key_Info_List[index].Three_Timers = 0;
				Key_Info_List[index].Press_Timers = 0;
				Key_Info_List[index].Key_State_Tem = KEY_THREECLICK;
			}
		}
	}	
	/************************************************************************************************************/

	// 等待时间没有三击发生则执行双击
	if (Key_Info_List[index].Three_Timers >= KEY_THREE_TIME)
		Key_Info_List[index].Key_State_Tem = KEY_DOUBLE;
		

	// 首次单击 开始双击间隔计时
	if ((Key_Info_List[index].Click_Flag == 1))		
	{
		if (Key_Info_List[index].Double_Timers >= KEY_CLICK_TIME)	
		{
			Key_Info_List[index].Click_Flag = 0;
			Key_Info_List[index].Double_Timers = 0;
		}
		else	
			Key_Info_List[index].Double_Timers++;
	}
	else// 双击了 开启三击计时
	if ((Key_Info_List[index].Click_Flag == 2))		
	{
		if (Key_Info_List[index].Three_Timers >= KEY_THREE_TIME)		
		{
			Key_Info_List[index].Click_Flag = 0;
			Key_Info_List[index].Three_Timers = 0;
		}
		else	
			Key_Info_List[index].Three_Timers++;
	}
	/************************************************************************************************************/
	/************************************************************************************************************/
	// 按键状态转换
	/************************************************************************************************************/
	switch (Key_Info_List[index].Key_State_Tem)
	{
		case KEY_RELEASE:											
			if (Key_Info_List[index].Is_Press == KEY_OFF)
			{
				Key_Info_List[index].Key_State_Tem = KEY_RELEASE;
				Key_Info_List[index].Key_State = KEY_RELEASE;
			}
			else										// 释放状态 如果有按键按下 则转为消抖状态
			{
				Key_Info_List[index].Press_Timers = 0;	// 时间重置 开启消抖计时
				Key_Info_List[index].Key_State_Tem = KEY_CONFIRM;
			}
			break;
		/****************************************************************************************************/	
		/****************************************************************************************************/		
		case KEY_CONFIRM:											
			if (Key_Info_List[index].Is_Press == KEY_OFF)		// 消抖状态 如果按键释放 --> 释放状态 
			{
				Key_Info_List[index].Press_Timers = 0;
				Key_Info_List[index].Key_State_Tem = KEY_RELEASE;
				Key_Info_List[index].Key_State = Key_Info_List[index].Key_State_Tem;
			}
			else												// 消抖状态 如果有按键按下 且 大于消抖时间 --> 短按状态
			{
				if (Key_Info_List[index].Press_Timers >= KEY_CONFIRM_TIME)
				{
					Key_Info_List[index].Click_Flag = 1;		// 第一次按下	
					// Key_Info_List[index].Press_Timers = 0;		// 时间重置 开始短按计时
					Key_Info_List[index].Double_Timers = 0;		// 时间重置 开始短按计时
					Key_Info_List[index].Key_State_Tem = KEY_CLICK;
				}
				else
					Key_Info_List[index].Press_Timers++;
			}
			break;

		/****************************************************************************************************/	
		/****************************************************************************************************/	
		case KEY_CLICK:								
			if (Key_Info_List[index].Is_Press == KEY_OFF)	// 单击--> 释放后 判断
			{			
				// Double_Timers == KEY_CLICK_TIME  					--> 单击了并且释放了，且无单击发生，等待响应时间过去 
				// Double_Timers = 0，Press_Timers >= KEY_CLICK_TIME 	--> 一直按着按键，单击响应时间过去了
				if ((Key_Info_List[index].Double_Timers >= KEY_CLICK_TIME) || ((Key_Info_List[index].Double_Timers == 0) && (Key_Info_List[index].Press_Timers >= KEY_CLICK_TIME)))
				{
					Key_Info_List[index].Click_Flag = 0; 
					Key_Info_List[index].Press_Timers = 0;
					Key_Info_List[index].Double_Timers = 0;

					Key_Info_List[index].Key_State = Key_Info_List[index].Key_State_Tem;	
					Key_Info_List[index].Key_State_Tem = KEY_RELEASE;
				}
			}
			else											// 短按状态 如果有按键按下 且 大于长按时间 则转为长按状态
			{
				if (Key_Info_List[index].Press_Timers >= KEY_LONGPRESS_TIM)
				{
					Key_Info_List[index].Key_State_Tem = KEY_LONGPRESS;
				}
				else
					Key_Info_List[index].Press_Timers++;
				
			}
			break;
		/****************************************************************************************************/
		/****************************************************************************************************/
		case KEY_DOUBLE:
			if (Key_Info_List[index].Is_Press == KEY_OFF) // 双击状态 按键释放了 --> 双击状态
			{
				Key_Info_List[index].Click_Flag = 0; 
				Key_Info_List[index].Press_Timers = 0;
				Key_Info_List[index].Double_Timers = 0;

				Key_Info_List[index].Key_State = Key_Info_List[index].Key_State_Tem;
				Key_Info_List[index].Key_State_Tem = KEY_RELEASE;
			}
			else	// 双击状态 如果按键一直按下 且 大于长按时间 --> 长按状态
			{
				if (Key_Info_List[index].Press_Timers >= KEY_LONGPRESS_TIM)
				{
					Key_Info_List[index].Key_State_Tem = KEY_LONGPRESS;
				}
				else
					Key_Info_List[index].Press_Timers++;
			}
			break;

		/****************************************************************************************************/	
		/****************************************************************************************************/
		case KEY_THREECLICK:								
			if (Key_Info_List[index].Is_Press == KEY_OFF)	// 单击--> 释放后 判断
			{			
				Key_Info_List[index].Click_Flag = 0; 
				Key_Info_List[index].Press_Timers = 0;
				Key_Info_List[index].Double_Timers = 0;

				Key_Info_List[index].Key_State = Key_Info_List[index].Key_State_Tem;	
				Key_Info_List[index].Key_State_Tem = KEY_RELEASE;
				
			}
			else											// 短按状态 如果有按键按下 且 大于长按时间 则转为长按状态
			{
				if (Key_Info_List[index].Press_Timers >= KEY_LONGPRESS_TIM)
				{
					Key_Info_List[index].Key_State_Tem = KEY_LONGPRESS;
				}
				else
					Key_Info_List[index].Press_Timers++;		
			}
			break;		

		/****************************************************************************************************/					
		/****************************************************************************************************/	
		case KEY_LONGPRESS:	
				if (Key_Info_List[index].Is_Press == KEY_OFF)	
				{
					Key_Info_List[index].Click_Flag = 0; 
					Key_Info_List[index].Press_Timers = 0;
					Key_Info_List[index].Double_Timers = 0;
					Key_Info_List[index].Key_State = Key_Info_List[index].Key_State_Tem;
					Key_Info_List[index].Key_State_Tem = KEY_RELEASE;
				}
				else						// 长按状态等待释放
					Key_Info_List[index].Key_State_Tem = KEY_LONGPRESS;
			break;
		
		default:
				Key_Info_List[index].Key_State = Key_Info_List[index].Key_State_Tem;
				Key_Info_List[index].Key_State_Tem = KEY_RELEASE;
			break;
	}
}


/**
 * @authors	LZC		@timer 7_Oct_24 
 * @brief 	根据按键的状态控制LED灯:
 * 								 1、KEY_CLICK		点亮相应 LED
 * 								 2、KEY_DOUBLE		熄灭相应 LED
 * 								 3、KEY_THREECLICK	点亮所有 LED
 * 								 4、KEY_LONGPRESS	熄灭所有 LED
 * @param	void		 
 * @retval 	void
 * @note	None
 * 			
 */
void Key_Use(void)  
{  	
    for (uint8_t i = 0; i < KEY_NUM_MAX; i++)  
    {  
		Key_Begin(i);
        if (Key_Info_List[i].Key_State == KEY_CLICK)  
        {  
            LED_ON_OFF((LED_Num)i, LED_ON);
        }  
		else
		if (Key_Info_List[i].Key_State == KEY_DOUBLE)
		{
			LED_ON_OFF((LED_Num)i, LED_OFF);
		}
		else 
		if (Key_Info_List[i].Key_State == KEY_THREECLICK)  
			LED_All_ON();
		else 
		if (Key_Info_List[i].Key_State == KEY_LONGPRESS)  
			LED_All_OFF();
    }  
}
