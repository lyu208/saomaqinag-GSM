#ifndef _PCF8563_H_
#define _PCF8563_H_

#include "IIC.h"

extern uchar Guint8_t_Year, Guint8_t_Month, Guint8_t_Day;
extern uchar Guint8_t_Week;
extern uchar Guint8_t_Hour, Guint8_t_Minute, Guint8_t_Second;


void PCF8563_Init(void);


uint8_t PCF8563_ReaDAdress(uint8_t Adress);

//在CLKOUT上定时1S输出一个下降沿脉冲
void PCF8563_CLKOUT_1s(void);

 

void WriteDataTo_PCF8563(uint8_t Adress, uint8_t DataTX);//取回7个字节的数据:秒，分，时，天，星期，月份，年份。
 

void ReadTimesFro_PCF8563(void);

void WriteTimesTo_PCF8563(void);

void  PCF8563_Config(void);

#endif




