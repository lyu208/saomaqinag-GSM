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


//extern  char  userPhone[];  //用户手机号，现在用的是死的，到时候可以用户输入！！
//extern  char  userPhoneBack[]; //手机号，取反后存储在此！！ 这个是中文发送时需要用到的！






extern char  userPhone_1[ ] ;
extern char  userPhone_2[ ] ;
 
extern char  userPhone_1Back[];
extern char  userPhone_2Back[];

extern  char  callInPhone[]; 
extern  char  callInPhoneTemp[];
extern char  callInPhoneBack[ ];

 


//char  centreNum[]="+8613800536500";	
extern  char  centreNum[];	//短信中心号
extern  char  centreNumBack[];



extern  uchar     regSuccessFlag;  //检测到注册网络成功，则为1 
extern  uchar     checkCenterNumFlag;

extern  uint16_t     messageLength; //发送中文短信时，有个AT指令， AT+CMGS=xx  需要设置短信的长度


extern  char  messageBuff_CH[];//究极体！！！ 这就是发送中文时，最后发射出去的东西！！！



 
extern  char  PDU_Content[]; //024F60 PDU模式短信存储内容  --- 中文短信

extern  char  centreNumBuff[]; //短信中心号组合数组
extern  char  phoneNumBuff[]; //用户手机号组合数组

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

