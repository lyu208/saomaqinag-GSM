#ifndef __TOOLS_H__
#define __TOOLS_H__


#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>








/*********************************
             ¡¾º¯ÊýÉùÃ÷¡¿
*********************************/
 
int   fputc(int ch, FILE *f); 
 
void  CheckId(uint32_t  *u32p_Unique_ID);

uint16_t  CRC16(uint8_t *updata, uint8_t len);

unsigned char  NumberTo_BCD(unsigned char dat);

unsigned char  BCD_ToNumber(unsigned char dat);

void IWDG_Configuration(void);

void FeedDog(void);

void  EXTI_Cmd(uint32_t EXTI_Line, FunctionalState NewState);

uint32_t  GetBetterNum(uint32_t * dat,const char length);

void  SystemReset(void);
uchar NumLength(uint32_t num);
#endif

