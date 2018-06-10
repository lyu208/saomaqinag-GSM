#include "sim900a.h"
#include "tools.h"
#include "peripheral.h"
#include "user.h"



uint16_t       overTimeCnt = 0;
uint8_t     recvDataCnt = 0; //串口中断，接受数据计数
char     GSM_recvBuff[253] = {0}; //用于接受GSM模块发送回来的数据


//char   GPRS_ServerIp[16] = {"115.159.50.191"}; //长度一定要为16 还有一个  /
char   GPRS_ServerIp[16] = {"139.199.61.36"};
char   GPRS_ServerPort[5] = {"2347"};


void Clear_GSM_BUff()
{
//    uint8_t j;

//  for (j = 0; j < GSM_BUFF_LENGTH; j++)      //将缓存内容清零
//  {
//      GSM_recvBuff[j] = 0;
//  }
    memset( GSM_recvBuff, 0x00, sizeof(GSM_recvBuff) ); //清空数组


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
                     uint16_t  time)  //time如果等于3 表示等待时间为2*3=6ms
{
    static  uint8_t   step = 1;
    static  uint8_t   askCnt = 0;

    switch (step)
    {
        case 1:
            Clear_GSM_BUff(); //清空一下GSM接收缓存数组
            if (askCnt >= 3) //发了三次，都没有返回正确的数据，则表示有错误，退出！
            {
                askCnt = 0;
                return 3; //失败返回3
            }
           
            askCnt++;
						printf(cmd);
            overTimeCnt =
                time; //检测间隔的时间 即发送指令后，等这么长时间，检测数据有没有收到！！
            step = 2; //切换到下一步骤
            break;
        case 2:
            if (overTimeCnt == 0)
            {

                if (FindChar(GSM_recvBuff,
                             ref)) //在GSM_recvBuff[]数组里，查找有没有含有 ref这么个字符
                {
                    Clear_GSM_BUff(); //清空一下GSM接收缓存数组
                    askCnt = 0; //就因为这里没有清零，多弄了一个多小时。。。。。 程序真是奇妙非常，变幻无穷啊！
                    step = 1;
                    return 2; //成功，返回2
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
        //如果当前连接着服务器  执行后，才返回想得到的数据，如果没有连接着，执行返回ERROR
        temp = Send_AT_Cmd("AT+CIPCLOSE=1\r\n", "OK",
                           150); //

        if (temp == 2) //如果发送指令后?返回的数据有OK表示成功?此时temp=2
        {
					  SendStringBy_USART1("OK2\r\n");
            break;
        }
        if (temp == 3) //连续三次都错误
        {
					  SendStringBy_USART1("Err2\r\n");
            break;
        }
    }


    while (1)
    {
        //关闭移动场景
        temp = Send_AT_Cmd("AT+CIPSHUT\r\n", "SHUT OK",
                          150); //

        if (temp == 2) //如果发送指令后?返回的数据有OK表示成功?此时temp=2
        {
					  SendStringBy_USART1("OK3\r\n");
            break;
        }
        if (temp == 3) //连续三次都错误
        {
					  SendStringBy_USART1("Err3\r\n");
            break;
        }
    }
    while (1)
    {
        //设置GPRS移动台类别为B,支持包交换和数据交换
        temp = Send_AT_Cmd("AT+CGCLASS=\"B\"\r\n", "OK",
                           150); //

        if (temp == 2) //如果发送指令后?返回的数据有OK表示成功?此时temp=2
        {
					  SendStringBy_USART1("OK4\r\n");
            break;
        }
        if (temp == 3) //连续三次都错误
        {
					  SendStringBy_USART1("Err4\r\n");
            break;
        }
    }
    while (1)
    {
        //设置PDP上下文,互联网接协议,接入点等信息
        temp = Send_AT_Cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n", "OK",
                           150); //

        if (temp == 2) //如果发送指令后?返回的数据有OK表示成功?此时temp=2
        {
					  SendStringBy_USART1("OK5\r\n");
            break;
        }
        if (temp == 3) //连续三次都错误
        {
					  SendStringBy_USART1("Err5\r\n");
            break;
        }
    }
    while (1)
    {
        //附着GPRS业务
        temp = Send_AT_Cmd("AT+CGATT=1\r\n", "OK",
                           1000); //

         if (temp == 2) //如果发送指令后?返回的数据有OK表示成功?此时temp=2
        {
					  SendStringBy_USART1("OK6\r\n");
            break;
        }
        if (temp == 3) //连续三次都错误
        {
					  SendStringBy_USART1("Err6\r\n");
            break;
        }
    }
    while (1)
    {
        //设置为GPRS连接模式
        temp = Send_AT_Cmd("AT+CIPCSGP=1,\"CMNET\"\r\n", "OK",
                           1000); //

        if (temp == 2) //如果发送指令后?返回的数据有OK表示成功?此时temp=2
        {
					  SendStringBy_USART1("OK7\r\n");
            break;
        }
        if (temp == 3) //连续三次都错误
        {
					  SendStringBy_USART1("Err7\r\n");
            break;
        }
    }
    while (1)
    {
        //设置接收数据显示IP头(方便判断数据来源,仅在单路连接有效)
        temp = Send_AT_Cmd("AT+CIPHEAD=1\r\n", "OK",
                           150); //

        if (temp == 2) //如果发送指令后?返回的数据有OK表示成功?此时temp=2
        {
					  SendStringBy_USART1("OK8\r\n");
            break;
        }
        if (temp == 3) //连续三次都错误
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
        //设置接收数据显示IP头(方便判断数据来源,仅在单路连接有效)
        temp = Send_AT_Cmd(conString, "OK",
                           1000); //这个需要很长时间才返回来，建立需要的时间很长

        if (temp == 2) //如果发送指令后?返回的数据有OK表示成功?此时temp=2
        {
					  SendStringBy_USART1("Connect Success\r\n");
					
				  	//SendStringBy_USART1(conString);
					  ledFlickFlag=1;
					  cunnectFlag=1;//1成功  2 是失败
            break;
        }
        if (temp == 3) //连续三次都错误
        {
					  SendStringBy_USART1("Err\r\n");
					  
            break;
					  
        }
    }

}
