#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"

//以下7行,建议放在AT45DB161.c文件中,而spi.c  spi.h 只充当工具代码使用！！
#define HL      1
#define LL      0

#define AT45DB161_CS_Control(f)        if (f)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_8);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_8)



/************************* SPI1功能函数声明 *****************************/
void SPI1_Config(void);
void SPI1_GPIO_Config(void);
void SPI1_Struct_Config(void);
uint8_t SPI1_ReadWrite_Byte(uint8_t data);

/*************************  END  *****************************/


/************************* SPI2功能函数声明  *****************************/
void SPI2_Config(void);
uint8_t SPI2_ReadWrite_Byte(uint8_t data);

/*************************  END  *****************************/

#endif



