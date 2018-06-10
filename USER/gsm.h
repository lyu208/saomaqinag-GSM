#ifndef  __GSM_H__
#define  __GSM_H__

#include <stm32f10x.h>


#define   uint8_t     uchar 

#define   GSM_BUFF_LENGTH   253
/*

#define   Send_LR() SendData(0X0D);\
											 SendData(0X0A)
#define UART1_SendLR() UART1_SendData(0X0D);\
											 UART1_SendData(0X0A)


*/

extern  char   GPRS_ServerIp[];
extern  char   GPRS_ServerPort[];



extern  char    GSM_recvBuff[];
extern  char    GSM_recvBuff2[];

extern  uint8_t     recvDataCnt;


//extern  char  userPhone[];  //�û��ֻ��ţ������õ������ģ���ʱ������û����룡��
//extern  char  userPhoneBack[]; //�ֻ��ţ�ȡ����洢�ڴˣ��� ��������ķ���ʱ��Ҫ�õ��ģ�






extern char  userPhone_1[ ] ;
extern char  userPhone_2[ ] ;
 
extern char  userPhone_1Back[];
extern char  userPhone_2Back[];

extern  char  callInPhone[]; 
extern  char  callInPhoneTemp[];
extern char  callInPhoneBack[ ];

 


//char  centreNum[]="+8613800536500";	
extern  char  centreNum[];	//�������ĺ�
extern  char  centreNumBack[];



extern  uchar     regSuccessFlag;  //��⵽ע������ɹ�����Ϊ1 
extern  uchar     checkCenterNumFlag;

extern  uint16_t     messageLength; //�������Ķ���ʱ���и�ATָ� AT+CMGS=xx  ��Ҫ���ö��ŵĳ���


extern  char  messageBuff_CH[];//�����壡���� ����Ƿ�������ʱ��������ȥ�Ķ���������



 
extern  char  PDU_Content[]; //024F60 PDUģʽ���Ŵ洢����  --- ���Ķ���

extern  char  centreNumBuff[]; //�������ĺ��������
extern  char  phoneNumBuff[]; //�û��ֻ����������

extern  char  callInPhone[];

extern  uint16_t        overTimeCnt;

extern  uint8_t         u8CheckPhoneDownCnt;
extern  uint16_t      uiPhoneRestartCnt;

extern  uchar  ucRestartCnt;






void  CheckRegState(void);
void Clear_GSM_BUff(void);

void Clear_GSM_BUff2(void);

uint8_t  Send_AT_Cmd(char *cmd,const char *ref,uint16_t  time); 
uint8_t  FindChar(const char *checkBuff, const char  *ch);
 

void  GetCenterNum(void);
void  getBackNum(char *num,char *numBack);

void   CheckPhoneMode(void);
void  PhoneRestart(void);

void  TCP_ConnectServer(void);

#endif

