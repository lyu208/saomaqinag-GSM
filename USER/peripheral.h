#ifndef __PERIPHERAL_H__
#define __PERIPHERAL_H__


#include <stm32f10x.h>


/************************************************
*             对外开放-变量                     *
*************************************************/


/************************************************
*             对外开放-函数                     *
*************************************************/
 

void  USART1_Config(void);
void  SendDataBy_USART1(int8_t data);
void  SendStringBy_USART1(char *data);





void  TIM4_Config(void);

void  LED_Config(void);
 

void  GPIO_Config(void);

void  KeyPadConfig(void);

void  USART2_Config(void);



#endif

