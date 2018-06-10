#ifndef _ADC_H_
#define _ADC_H_
#include <stm32f10x.h>

 
extern volatile  uint16_t  u16AdcValue;
extern   uint16_t  u16AD_Value[];



uint16_t Get_ADC1_Value(void);
void ADC_Config(void);
void ADC_ValueFilter(void);
 


#endif









