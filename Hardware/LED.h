#ifndef __LED_H
#define __LED_H

#define LED_ON  1
#define LED_OFF 0
typedef enum 
{
	LED1 = 0,
	LED2,
	LED3,
	LED4
}LED_Num;


void LED_Init(void);
void LED_All_ON(void);
void LED_All_OFF(void);
void LED_ON_OFF(LED_Num LED_X, uint8_t ON_OFF);

void LED1_Turn(void);
void LED2_Turn(void);

#endif
