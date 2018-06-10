#ifndef _ADS1230_H_
#define _ADS1230_H_

#include <stm32f10x.h>
#include "sys.h"


#define  DOUT_1           PCout(14)
#define  SCLK_1           PCout(15)
#define  PDWN_1           PCout(0)

#define  DOUT_1_IN           PCin(14)


/***************************************************/

#define  ADS1230_DOUT     DOUT_1
#define  ADS1230_SCLK     SCLK_1
#define  ADS1230_PDWN     PDWN_1 

#define  ADS1230_DOUT_IN  DOUT_1_IN




void  ADS1230_Init(void);
void  ADS1230_Config(void);
void  EnterStandbyMode(void);
void  ExitStandbyMode(void);
void OffsetCalibration(void);
uint32_t ReadDataFro_ADS1230(void);

void  EnterStandbyMode(void);
 
void  ExitStandbyMode(void);

uint32_t ReadDataFro_ADS1230(void);
#endif
