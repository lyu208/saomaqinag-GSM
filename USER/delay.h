#ifndef _DELAY_H_
#define _DELAY_H_
#include <stm32f10x.h>

void Delay_init(void);
void Delay_us(uint32_t nus);
void Delay_ms(uint16_t nms);

#endif
