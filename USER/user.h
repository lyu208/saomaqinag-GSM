#ifndef _USER_H_
#define _USER_H_

#include <stm32f10x.h>
#include "sys.h"
#include <stdint.h>


extern uint16_t DS0_TimeCnt;



#define ON   0
#define OFF  1
 
/* ע�� ���з����� �������пո� */
#define Beep_Control(x)       if(x)  \
						GPIO_ResetBits(GPIOC, GPIO_Pin_11); \
						else \
						GPIO_SetBits(GPIOC, GPIO_Pin_11)



 
 
/****************************************************/
#define LED0   PBout(5) 
#define LED1   PEout(5)
 
#define LED0_IN   PBin(5) 
#define LED1_IN   PEin(5)
  
 
#define  DS0                LED0            
#define  DS1                LED1             
 
#define  DS0_IN             LED0_IN 
#define  DS1_IN             LED1_IN 
 
  
/****************************************************/
#define  KEY_Row1             PAin(1) //�� row   �ϵ��ǵ͵�ƽ  
#define  KEY_Row2             PAin(4)
#define  KEY_Row3             PAin(5)
#define  KEY_Row4             PAin(6)

#define  KEY_Col1             PAout(7)//�� column  
#define  KEY_Col2             PAout(8)
#define  KEY_Col3             PAout(0)
#define  KEY_Col4             PAout(12)

#define  KEY_Col1_IN             PAin(7)//�� column  
#define  KEY_Col2_IN             PFin(7)
#define  KEY_Col3_IN             PFin(6)
#define  KEY_Col4_IN             PAin(12)




/************************************************
*             ���⿪��-����                     *
*************************************************/ 
extern uchar getWeightService;
extern uchar getWeightCnt;
extern   uchar gprsService;
extern  uint16_t  heartbeat;
extern uchar  ledFlickFlag;
extern uchar cunnectFlag;
extern uchar heartbeatService;
/************************************************
*             ���⿪��-����                     *
*************************************************/
void  HeadFirst(void);
 
void  LedFlick(void); 

void  KeyPadScan(void);
void  KeyPadService(void);
void  GetWeightService(void);
void  GPRS_Service(void);
 
#endif
