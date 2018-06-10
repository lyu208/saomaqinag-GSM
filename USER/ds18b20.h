#ifndef _DS18B20_H_
#define _DS18B20_H_


#include <stm32f10x.h>
#include "sys.h"

#define DS18B20_ROM_LENGTH  8


#define DQ_1   PAout(3) 
#define DQ_2   PCout(3)

 
#define DQ_1_IN   PAin(3)
#define DQ_2_IN   PCin(3)


/********************************************
*              開放变量聲明                 *
*********************************************/

extern uchar   romEncode[DS18B20_ROM_LENGTH];




/********************************************
*              開放函數聲明                  *
*********************************************/

BOOL DS18B20_Reset(uchar select);
void WriteByteTo_DS18B20(uchar dat, uchar select);
uchar  ReadByteFr_DS18B20(uchar select);
void Read_DS18B20_ROM(void);
void  DS18B20_Config(void);


#endif
