#ifndef _SIM900_H_
#define _SIM900_H_
#include <stm32f10x.h>

extern    uint16_t   overTimeCnt;
extern    char     GSM_recvBuff[];
extern   uint8_t     recvDataCnt;


void  TCP_ConnectServer(void);
void  SendByGprs(void);

uint8_t  Send_AT_Cmd(char *cmd, const char *ref,
                     uint16_t  time);
#endif
