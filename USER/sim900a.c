#include "sim900a.h"
#include "tools.h"
#include "peripheral.h"
#include "user.h"



uint16_t       overTimeCnt = 0;
uint8_t     recvDataCnt = 0; //�����жϣ��������ݼ���
char     GSM_recvBuff[253] = {0}; //���ڽ���GSMģ�鷢�ͻ���������


//char   GPRS_ServerIp[16] = {"115.159.50.191"}; //����һ��ҪΪ16 ����һ��  /
char   GPRS_ServerIp[16] = {"139.199.61.36"};
char   GPRS_ServerPort[5] = {"2347"};


void Clear_GSM_BUff()
{
//    uint8_t j;

//  for (j = 0; j < GSM_BUFF_LENGTH; j++)      //��������������
//  {
//      GSM_recvBuff[j] = 0;
//  }
    memset( GSM_recvBuff, 0x00, sizeof(GSM_recvBuff) ); //�������


    recvDataCnt = 0;
}



uint8_t  FindChar(const char *checkBuff, const char *ch)
{
    if (strstr(checkBuff, ch) != NULL)
    {
        return  1;
    }
    else
    {
        return  0;
    }
}




uint8_t  Send_AT_Cmd(char *cmd, const char *ref,
                     uint16_t  time)  //time�������3 ��ʾ�ȴ�ʱ��Ϊ2*3=6ms
{
    static  uint8_t   step = 1;
    static  uint8_t   askCnt = 0;

    switch (step)
    {
        case 1:
            Clear_GSM_BUff(); //���һ��GSM���ջ�������
            if (askCnt >= 3) //�������Σ���û�з�����ȷ�����ݣ����ʾ�д����˳���
            {
                askCnt = 0;
                return 3; //ʧ�ܷ���3
            }
           
            askCnt++;
						printf(cmd);
            overTimeCnt =
                time; //�������ʱ�� ������ָ��󣬵���ô��ʱ�䣬���������û���յ�����
            step = 2; //�л�����һ����
            break;
        case 2:
            if (overTimeCnt == 0)
            {

                if (FindChar(GSM_recvBuff,
                             ref)) //��GSM_recvBuff[]�����������û�к��� ref��ô���ַ�
                {
                    Clear_GSM_BUff(); //���һ��GSM���ջ�������
                    askCnt = 0; //����Ϊ����û�����㣬��Ū��һ����Сʱ���������� ������������ǳ�����������
                    step = 1;
                    return 2; //�ɹ�������2
                }

                step = 1;
            }
            break;
    }
    return  1;
}



void  TCP_ConnectServer()
{
    uchar temp = 0;
    char conString[100] = {0};
		
    while (1)
    {
        //�����ǰ�����ŷ�����  ִ�к󣬲ŷ�����õ������ݣ����û�������ţ�ִ�з���ERROR
        temp = Send_AT_Cmd("AT+CIPCLOSE=1\r\n", "OK",
                           150); //

        if (temp == 2) //�������ָ���?���ص�������OK��ʾ�ɹ�?��ʱtemp=2
        {
					  SendStringBy_USART1("OK2\r\n");
            break;
        }
        if (temp == 3) //�������ζ�����
        {
					  SendStringBy_USART1("Err2\r\n");
            break;
        }
    }


    while (1)
    {
        //�ر��ƶ�����
        temp = Send_AT_Cmd("AT+CIPSHUT\r\n", "SHUT OK",
                          150); //

        if (temp == 2) //�������ָ���?���ص�������OK��ʾ�ɹ�?��ʱtemp=2
        {
					  SendStringBy_USART1("OK3\r\n");
            break;
        }
        if (temp == 3) //�������ζ�����
        {
					  SendStringBy_USART1("Err3\r\n");
            break;
        }
    }
    while (1)
    {
        //����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ���
        temp = Send_AT_Cmd("AT+CGCLASS=\"B\"\r\n", "OK",
                           150); //

        if (temp == 2) //�������ָ���?���ص�������OK��ʾ�ɹ�?��ʱtemp=2
        {
					  SendStringBy_USART1("OK4\r\n");
            break;
        }
        if (temp == 3) //�������ζ�����
        {
					  SendStringBy_USART1("Err4\r\n");
            break;
        }
    }
    while (1)
    {
        //����PDP������,��������Э��,��������Ϣ
        temp = Send_AT_Cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n", "OK",
                           150); //

        if (temp == 2) //�������ָ���?���ص�������OK��ʾ�ɹ�?��ʱtemp=2
        {
					  SendStringBy_USART1("OK5\r\n");
            break;
        }
        if (temp == 3) //�������ζ�����
        {
					  SendStringBy_USART1("Err5\r\n");
            break;
        }
    }
    while (1)
    {
        //����GPRSҵ��
        temp = Send_AT_Cmd("AT+CGATT=1\r\n", "OK",
                           1000); //

         if (temp == 2) //�������ָ���?���ص�������OK��ʾ�ɹ�?��ʱtemp=2
        {
					  SendStringBy_USART1("OK6\r\n");
            break;
        }
        if (temp == 3) //�������ζ�����
        {
					  SendStringBy_USART1("Err6\r\n");
            break;
        }
    }
    while (1)
    {
        //����ΪGPRS����ģʽ
        temp = Send_AT_Cmd("AT+CIPCSGP=1,\"CMNET\"\r\n", "OK",
                           1000); //

        if (temp == 2) //�������ָ���?���ص�������OK��ʾ�ɹ�?��ʱtemp=2
        {
					  SendStringBy_USART1("OK7\r\n");
            break;
        }
        if (temp == 3) //�������ζ�����
        {
					  SendStringBy_USART1("Err7\r\n");
            break;
        }
    }
    while (1)
    {
        //���ý���������ʾIPͷ(�����ж�������Դ,���ڵ�·������Ч)
        temp = Send_AT_Cmd("AT+CIPHEAD=1\r\n", "OK",
                           150); //

        if (temp == 2) //�������ָ���?���ص�������OK��ʾ�ɹ�?��ʱtemp=2
        {
					  SendStringBy_USART1("OK8\r\n");
            break;
        }
        if (temp == 3) //�������ζ�����
        {
					  SendStringBy_USART1("Err8\r\n");
            break;
        }
    }

    //AT+CIPSTART="TCP","121.250.123.38","8086"
    sprintf((char*)conString, "AT+CIPSTART=\"TCP\",\"%s\",\"%s\"\r\n",
            GPRS_ServerIp, GPRS_ServerPort);

    while (1)
    {
        //���ý���������ʾIPͷ(�����ж�������Դ,���ڵ�·������Ч)
        temp = Send_AT_Cmd(conString, "OK",
                           1000); //�����Ҫ�ܳ�ʱ��ŷ�������������Ҫ��ʱ��ܳ�

        if (temp == 2) //�������ָ���?���ص�������OK��ʾ�ɹ�?��ʱtemp=2
        {
					  SendStringBy_USART1("Connect Success\r\n");
					
				  	//SendStringBy_USART1(conString);
					  ledFlickFlag=1;
					  cunnectFlag=1;//1�ɹ�  2 ��ʧ��
            break;
        }
        if (temp == 3) //�������ζ�����
        {
					  SendStringBy_USART1("Err\r\n");
					  
            break;
					  
        }
    }

}