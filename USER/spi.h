#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"

//����7��,�������AT45DB161.c�ļ���,��spi.c  spi.h ֻ�䵱���ߴ���ʹ�ã���
#define HL      1
#define LL      0

#define AT45DB161_CS_Control(f)        if (f)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_8);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_8)



/************************* SPI1���ܺ������� *****************************/
void SPI1_Config(void);
void SPI1_GPIO_Config(void);
void SPI1_Struct_Config(void);
uint8_t SPI1_ReadWrite_Byte(uint8_t data);

/*************************  END  *****************************/


/************************* SPI2���ܺ�������  *****************************/
void SPI2_Config(void);
uint8_t SPI2_ReadWrite_Byte(uint8_t data);

/*************************  END  *****************************/

#endif



