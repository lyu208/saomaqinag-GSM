#ifndef _HX711_H_
#define _HX711_H_


#include <stm32f10x.h>
#include "sys.h"
 
 
#define  ADSK   PBout(7)
#define  ADDO   PBin(6)



extern unsigned long ReadCount(void);
extern void HX711_Config(void);

 




 
extern void GetMaopi(void);
extern void GetWeight(void);

extern u32 HX711_Buffer;
extern u32 Weight_Maopi;
extern s32 Weight_Shiwu;
extern u8 Flag_Error;
 
#endif

