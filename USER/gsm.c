#include "gsm.h"
#include "tools.h"
#include "delay.h"
#include "user.h"
#include  "string.h"
/**
 *
 * 1号机温度：31.6℃ 32.2℃;
 *
 * 常规显示
 * x号机温度：xx.x℃ xx.x℃
 *
 * 1号机区温度：31.3℃ 32.6℃；2号机区温度：33.1℃ 36.4℃；3号机区温度：35.5℃ 29.5℃；
 *
 */

/**  0     1     2    3    4    5     6    7    8    9
 *  0030 0031 0032 0033 0034  0035  0036 0037 0038 0039
 *
 */

/**
 *  .     空格      ：       ℃
 *  002E  0020    FF1A     2103
 */


/**
 * 号机区温度
 * 53F7 673A 533A 6E29 5EA6
 */

/**
 * 风口全部打开
 * 98CE 53E3 5168 90E8 6253 5F00
 */

/**
* 风口全部关闭
* 98CE 53E3 5168 90E8 5173 95ED
*/

/**
* 恢复自动控制
* 6062 590D 81EA 52A8 63A7 5236
*/
/**
 *
 *
 */
/*********************************
         变量定义部分
**********************************/
char   GPRS_ServerIp[16]={"124.134.239.29"}; //长度一定要为16 还有一个  /
char   GPRS_ServerPort[5]={"6666"};


uint8_t     regSuccessFlag = 0;  //检测到注册网络成功，则为1
uchar      checkCenterNumFlag =
    0; //检测到注册网络成功，后检查短信中心号码，也成功则为1

//uint8_t     GSM_recvBuff[200]= {"CMT:   8615866550271 ,user3 15953332029"}; // \"+8615866550271\", ,\"15/09/23,15:21:07\"
char     GSM_recvBuff[GSM_BUFF_LENGTH] = {0}; //用于接受GSM模块发送回来的数据
char     GSM_recvBuff2[200] = {"+CMT: \"+8615866550271\", ,\"15/09/06,11:12:05\"\r\n752862370031002000310035003800360036003500350030003200370031"};

uint8_t     recvDataCnt = 0; //串口中断，接受数据计数


char  userPhone_1[13] = {"15866550271"};//18706522160
//char  userPhone_2[13] = {"15866550271"}; //为什么长度为13  因为反转函数 会加一个F 然后加上/0 正好13个


char  userPhone_1Back[13] = {0};
//char  userPhone_2Back[13] = {0};

char  callInPhone[12] = { 0 }; //char  callInPhone[]="-----------";
char  callInPhoneTemp[12] = { 0 };
char  callInPhoneBack[13] = { 0 };

//char  userPhone[12] =
//    "15866550271";  //15854989578用户手机号，现在用的是死的，到时候可以用户输入！！   发送英文短信用
//char  userPhoneBack[12] = { 0 }; //手机号，取反后存储在此！！ 这个是中文发送时需要用到的！          发送中文短信用



//char  centreNum[]="+8613800536500";
char  centreNum[15] = { 0 };  //短信中心号    发送英文短信用
char  centreNumBack[15] = { 0 }; //取反后的短信中心号存储在此！！ 发送中文短信时用！！



uint16_t  messageLength =
    0; //发送中文短信时，有个AT指令， AT+CMGS=xx  需要设置短信的长度


char  messageBuff_CH[500] = { 0 };




char  PDU_Content[280] = {0}; //"6D4B8BD55DF27ECF5B8C6210测试已经完成"  024F60 PDU模式短信存储内容  --- 中文短信

char  centreNumBuff[19] = {0}; //18短信中心号组合数组    中文用
char  phoneNumBuff[15] = {0}; //14用户手机号组合数组     中文用







uint16_t       overTimeCnt = 0;
uint8_t        u8CheckPhoneDownCnt = 0;
uint16_t         uiPhoneRestartCnt = 0;


uint8_t  phoneRestartFlag = 0;
uchar  ucRestartCnt = 0;
/*********************************
             函数部分
**********************************/

void  getBackNum(char *num, char *numBack)
{
    char temp;
    unsigned char length = 0, i = 0;

    length = strlen(num);
    if ((length % 2) == 1) //如果是奇数  在末尾加 ‘F’
    {
        num[length] = 'F';
        num[length + 1] = '\0';
    }

    for (i = 0; i < length;) //俩俩翻转
    {
        temp = num[i];
        numBack[i] = num[i + 1];
        numBack[i + 1] = temp;
        i += 2;
    }

    num[length] = '\0'; //测试发现 需要加上，不然会和下一次打电话有耦合，多带一个F
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
            printf(cmd);
            askCnt++;
            
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



void  GetCenterNum()
{
    uint8_t overTime = 0, i, j;
    uint8_t index = 0;
    char  centreNumTemp[15] = { '0' };

    if (regSuccessFlag == 0) return;  //注册成功，才执行短信中心号码的获取。

    while (1)
    {
         FeedDog(); //喂狗
        Clear_GSM_BUff();

        printf("AT+CSCA?\r\n");


        Delay_ms(300);
   


        for (j = 0; j < GSM_BUFF_LENGTH; j++)
        {
            if (GSM_recvBuff[j] == 'A' && GSM_recvBuff[j + 1] == ':')
            {
                index = j;
                checkCenterNumFlag = 1;
                break;
            }
        }

        if (checkCenterNumFlag == 1) //如果成功检测到符合条件的返回的数据
        {
 
            for (i = 0; i < 14; i++)
            {
                centreNum[i] = GSM_recvBuff[index + 4]; //短信中心号取出来，包括'+'

                index++;
            }
            for (i = 0; i < 13; i++) //8613800536500
            {
                centreNumTemp[i] = centreNum[i + 1]; //把'+'符号去掉
            }
            getBackNum(centreNumTemp,
                       centreNumBack); //顺便把短信中心号 也 取反出来  但不要包括‘+’
 
            return; //break 亦可
        }

        overTime++;
        if (overTime > 3)
        {
            overTime = 0;
            checkCenterNumFlag = 0;  //失败
            return;   //break 亦可
        }
    }
}




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
//void Clear_GSM_BUff2()
//{
//    uint8_t j;

//    for (j = 0; j < 200; j++)      //将缓存内容清零
//    {
//        GSM_recvBuff2[j] = 0;
//    }

//}

 



void  CheckRegState()
{
    uint8_t overTime = 0, j = 0;
    Clear_GSM_BUff();

    while (1)
    {
        FeedDog(); //喂狗
        Clear_GSM_BUff();
        printf("AT+CREG?\r\n");

        Delay_ms(500);
   
        for (j = 0; j < GSM_BUFF_LENGTH; j++)
        {
            if (GSM_recvBuff[j] == 'G' && GSM_recvBuff[j + 1] == ':')
            {
                if (GSM_recvBuff[j + 5] == '1' || GSM_recvBuff[j + 5] == '5')
                {
                    regSuccessFlag = 1;                 
//                                       break;//用break 只是退出了for循环   并没有退出while(1)
                    return; // 用return  记住了！！！
                }
            }
        }

        overTime++;
        if (overTime >=
            40) //如果20s了，那么就是表示 没有放置手机卡，或手机卡不符合规则
        {
            regSuccessFlag = 0;
            break; //这个的直系循环 是 while(1)  所以可以用break 跳出
        }
    }
}



/**
* 此函数只是用来检查 当上电手机检测成功，但是在运行当中，掉线等。。 只有重新模块上电才能和sim卡进行通信 自动注册网络！
*  
* 15分钟检测掉线等情况 重启一次   如果连续5次重启都注册不上网络 
* 不再检测额  除非第二天早上7点 再恢复检测 
*/
void   CheckPhoneMode(void)
{
    uchar  j = 0;
    static uchar  step = 0; 
    static uint16_t  minuteCnt=0;
   
    if (phoneRestartFlag) //手机正在重启 还没启动起来 不检测
    {
        return;
    }
    if (checkCenterNumFlag == 0 || MG_CHECK == 1 || ucRestartCnt >= 5) return; //没插手机模块 或者 插着 初次没检查成功了，则退出不执行

    if(sendMessageService == 1 ) //有正在发送短信的服务函数 则退出
    {
        step = 0;
        return;
    }

    switch(step)
    {
    case 0:
            /*0.5s检测一次是否掉线  在中断中进行2ms自加  但没有采取中断锁的安全方式*/
            if(u8CheckPhoneDownCnt >= 250)
            {
                if (++minuteCnt>=1800) /*1800 静态变量 当十五分钟检测一次是否掉线*/
                {
                Clear_GSM_BUff();
                printf("AT+CREG?\r\n"); 
                minuteCnt=0;
                step = 1;
                }
                u8CheckPhoneDownCnt = 0;
            }
            break;
        case 1: //发送了检测指令后，等200ms 检测返回的数据！
            if(u8CheckPhoneDownCnt >= 100) //200ms检测返回的数据
            {
                for (j = 0; j < GSM_BUFF_LENGTH; j++)
                {
                    if (GSM_recvBuff[j] == 'G' && GSM_recvBuff[j + 1] == ':')
                    {
                         /*注册了本地网络 或 漫游网络 检测成功*/
                         if (GSM_recvBuff[j + 5] == '1' || GSM_recvBuff[j + 5] == '5' ) 
                         {
                             ucRestartCnt=0;  //只有连续重启5次都失败 才不再执行
                         }else /*其他情况 均进行掉电重启手机模块*/
                         {
                             phoneRestartFlag = 1;
                             ucRestartCnt++; 
                         }
                    }
                }
                u8CheckPhoneDownCnt = 0;
                step = 0;
            }
            break;

        default:
            break;

    }


}
void  PhoneRestart(void)
{
    static uchar step = 0;

    if(phoneRestartFlag == 0) return;
	//  LED_1=~LED_1;
    switch(step)
    {
        case 0:
            if(uiPhoneRestartCnt >= 1000)
            {
                _PWR = 1;  //手机模块控制开关机的pwr引脚
                uiPhoneRestartCnt = 0;
                step = 1;
            }
            break;
        case 1:
            if(uiPhoneRestartCnt >= 1000)
            {
                _PWR = 0;  //手机模块开关引脚
                uiPhoneRestartCnt = 0;
                step = 2;
            }
            break;
        case 2:
            if(uiPhoneRestartCnt >= 1000)
            {
                _PWR = 1;  //手机模块开关引脚
                uiPhoneRestartCnt = 0;
                step = 3;
            }
            break;
        case 3:
            if(uiPhoneRestartCnt >= 1000)
            {
                _PWR = 0;  //手机模块开关引脚
                uiPhoneRestartCnt = 0;
                step = 4;
            }
            break;
        case 4:
            if(uiPhoneRestartCnt >= 15000) //重启后30s后 配置新短信直接输出！
            {
                printf("AT+CNMI=3,2,0,0,0\r\n"); //新短信直接输出 AT+CNMI=3,2,0,0,0    【配置掉电不保存】
                uiPhoneRestartCnt = 0;
                step = 0;
                phoneRestartFlag = 0; /*手机重启标志指令 清零 当再一次检测到非正常网络注册状态 再启动该重启指令*/
            }
            break;
        default:
            break;
    }
}


void  TCP_ConnectServer()
{
	uchar temp = 0;
	
				printf("AT+CIPCLOSE=1");
        while (1)
        {
            //关闭移动场景
            temp = Send_AT_Cmd("AT+CIPSHUT\r\n", "SHUT OK",
                               150); //

            if (temp == 2) //如果发送指令后?返回的数据有OK表示成功?此时temp=2
            {
                break;
            }
            if (temp == 3) //连续三次都错误
            {
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
                break;
            }
            if (temp == 3) //连续三次都错误
            {
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
                break;
            }
            if (temp == 3) //连续三次都错误
            {
                break;
            }
        }
        while (1)
        {
            //附着GPRS业务
            temp = Send_AT_Cmd("AT+CGATT=1\r\n", "OK",
                               150); //

            if (temp == 2) //如果发送指令后?返回的数据有OK表示成功?此时temp=2
            {
                break;
            }
            if (temp == 3) //连续三次都错误
            {
                break;
            }
        }
        while (1)
        {
            //设置为GPRS连接模式
            temp = Send_AT_Cmd("AT+CIPCSGP=1,\"CMNET\"\r\n", "OK",
                               150); //

            if (temp == 2) //如果发送指令后?返回的数据有OK表示成功?此时temp=2
            {
                break;
            }
            if (temp == 3) //连续三次都错误
            {
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
                break;
            }
            if (temp == 3) //连续三次都错误
            {
                break;
            }
        }
				        printf("AT+CIPSTART=\"TCP\",\"");       //          |
                printf(GPRS_ServerIp);                  //          |
                printf("\",\"");                        //           > SIM发Ip地址和端口命令
                printf(GPRS_ServerPort);                //          |
                printf("\"\r\n");                       //          |
}
