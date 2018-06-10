#include "gsm.h"
#include "tools.h"
#include "delay.h"
#include "user.h"
#include  "string.h"
/**
 *
 * 1�Ż��¶ȣ�31.6�� 32.2��;
 *
 * ������ʾ
 * x�Ż��¶ȣ�xx.x�� xx.x��
 *
 * 1�Ż����¶ȣ�31.3�� 32.6�棻2�Ż����¶ȣ�33.1�� 36.4�棻3�Ż����¶ȣ�35.5�� 29.5�棻
 *
 */

/**  0     1     2    3    4    5     6    7    8    9
 *  0030 0031 0032 0033 0034  0035  0036 0037 0038 0039
 *
 */

/**
 *  .     �ո�      ��       ��
 *  002E  0020    FF1A     2103
 */


/**
 * �Ż����¶�
 * 53F7 673A 533A 6E29 5EA6
 */

/**
 * ���ȫ����
 * 98CE 53E3 5168 90E8 6253 5F00
 */

/**
* ���ȫ���ر�
* 98CE 53E3 5168 90E8 5173 95ED
*/

/**
* �ָ��Զ�����
* 6062 590D 81EA 52A8 63A7 5236
*/
/**
 *
 *
 */
/*********************************
         �������岿��
**********************************/
char   GPRS_ServerIp[16]={"124.134.239.29"}; //����һ��ҪΪ16 ����һ��  /
char   GPRS_ServerPort[5]={"6666"};


uint8_t     regSuccessFlag = 0;  //��⵽ע������ɹ�����Ϊ1
uchar      checkCenterNumFlag =
    0; //��⵽ע������ɹ�������������ĺ��룬Ҳ�ɹ���Ϊ1

//uint8_t     GSM_recvBuff[200]= {"CMT:   8615866550271 ,user3 15953332029"}; // \"+8615866550271\", ,\"15/09/23,15:21:07\"
char     GSM_recvBuff[GSM_BUFF_LENGTH] = {0}; //���ڽ���GSMģ�鷢�ͻ���������
char     GSM_recvBuff2[200] = {"+CMT: \"+8615866550271\", ,\"15/09/06,11:12:05\"\r\n752862370031002000310035003800360036003500350030003200370031"};

uint8_t     recvDataCnt = 0; //�����жϣ��������ݼ���


char  userPhone_1[13] = {"15866550271"};//18706522160
//char  userPhone_2[13] = {"15866550271"}; //Ϊʲô����Ϊ13  ��Ϊ��ת���� ���һ��F Ȼ�����/0 ����13��


char  userPhone_1Back[13] = {0};
//char  userPhone_2Back[13] = {0};

char  callInPhone[12] = { 0 }; //char  callInPhone[]="-----------";
char  callInPhoneTemp[12] = { 0 };
char  callInPhoneBack[13] = { 0 };

//char  userPhone[12] =
//    "15866550271";  //15854989578�û��ֻ��ţ������õ������ģ���ʱ������û����룡��   ����Ӣ�Ķ�����
//char  userPhoneBack[12] = { 0 }; //�ֻ��ţ�ȡ����洢�ڴˣ��� ��������ķ���ʱ��Ҫ�õ��ģ�          �������Ķ�����



//char  centreNum[]="+8613800536500";
char  centreNum[15] = { 0 };  //�������ĺ�    ����Ӣ�Ķ�����
char  centreNumBack[15] = { 0 }; //ȡ����Ķ������ĺŴ洢�ڴˣ��� �������Ķ���ʱ�ã���



uint16_t  messageLength =
    0; //�������Ķ���ʱ���и�ATָ� AT+CMGS=xx  ��Ҫ���ö��ŵĳ���


char  messageBuff_CH[500] = { 0 };




char  PDU_Content[280] = {0}; //"6D4B8BD55DF27ECF5B8C6210�����Ѿ����"  024F60 PDUģʽ���Ŵ洢����  --- ���Ķ���

char  centreNumBuff[19] = {0}; //18�������ĺ��������    ������
char  phoneNumBuff[15] = {0}; //14�û��ֻ����������     ������







uint16_t       overTimeCnt = 0;
uint8_t        u8CheckPhoneDownCnt = 0;
uint16_t         uiPhoneRestartCnt = 0;


uint8_t  phoneRestartFlag = 0;
uchar  ucRestartCnt = 0;
/*********************************
             ��������
**********************************/

void  getBackNum(char *num, char *numBack)
{
    char temp;
    unsigned char length = 0, i = 0;

    length = strlen(num);
    if ((length % 2) == 1) //���������  ��ĩβ�� ��F��
    {
        num[length] = 'F';
        num[length + 1] = '\0';
    }

    for (i = 0; i < length;) //������ת
    {
        temp = num[i];
        numBack[i] = num[i + 1];
        numBack[i + 1] = temp;
        i += 2;
    }

    num[length] = '\0'; //���Է��� ��Ҫ���ϣ���Ȼ�����һ�δ�绰����ϣ����һ��F
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
            printf(cmd);
            askCnt++;
            
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



void  GetCenterNum()
{
    uint8_t overTime = 0, i, j;
    uint8_t index = 0;
    char  centreNumTemp[15] = { '0' };

    if (regSuccessFlag == 0) return;  //ע��ɹ�����ִ�ж������ĺ���Ļ�ȡ��

    while (1)
    {
         FeedDog(); //ι��
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

        if (checkCenterNumFlag == 1) //����ɹ���⵽���������ķ��ص�����
        {
 
            for (i = 0; i < 14; i++)
            {
                centreNum[i] = GSM_recvBuff[index + 4]; //�������ĺ�ȡ����������'+'

                index++;
            }
            for (i = 0; i < 13; i++) //8613800536500
            {
                centreNumTemp[i] = centreNum[i + 1]; //��'+'����ȥ��
            }
            getBackNum(centreNumTemp,
                       centreNumBack); //˳��Ѷ������ĺ� Ҳ ȡ������  ����Ҫ������+��
 
            return; //break ���
        }

        overTime++;
        if (overTime > 3)
        {
            overTime = 0;
            checkCenterNumFlag = 0;  //ʧ��
            return;   //break ���
        }
    }
}




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
//void Clear_GSM_BUff2()
//{
//    uint8_t j;

//    for (j = 0; j < 200; j++)      //��������������
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
        FeedDog(); //ι��
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
//                                       break;//��break ֻ���˳���forѭ��   ��û���˳�while(1)
                    return; // ��return  ��ס�ˣ�����
                }
            }
        }

        overTime++;
        if (overTime >=
            40) //���20s�ˣ���ô���Ǳ�ʾ û�з����ֻ��������ֻ��������Ϲ���
        {
            regSuccessFlag = 0;
            break; //�����ֱϵѭ�� �� while(1)  ���Կ�����break ����
        }
    }
}



/**
* �˺���ֻ��������� ���ϵ��ֻ����ɹ������������е��У����ߵȡ��� ֻ������ģ���ϵ���ܺ�sim������ͨ�� �Զ�ע�����磡
*  
* 15���Ӽ����ߵ���� ����һ��   �������5��������ע�᲻������ 
* ���ټ���  ���ǵڶ�������7�� �ٻָ���� 
*/
void   CheckPhoneMode(void)
{
    uchar  j = 0;
    static uchar  step = 0; 
    static uint16_t  minuteCnt=0;
   
    if (phoneRestartFlag) //�ֻ��������� ��û�������� �����
    {
        return;
    }
    if (checkCenterNumFlag == 0 || MG_CHECK == 1 || ucRestartCnt >= 5) return; //û���ֻ�ģ�� ���� ���� ����û���ɹ��ˣ����˳���ִ��

    if(sendMessageService == 1 ) //�����ڷ��Ͷ��ŵķ����� ���˳�
    {
        step = 0;
        return;
    }

    switch(step)
    {
    case 0:
            /*0.5s���һ���Ƿ����  ���ж��н���2ms�Լ�  ��û�в�ȡ�ж����İ�ȫ��ʽ*/
            if(u8CheckPhoneDownCnt >= 250)
            {
                if (++minuteCnt>=1800) /*1800 ��̬���� ��ʮ����Ӽ��һ���Ƿ����*/
                {
                Clear_GSM_BUff();
                printf("AT+CREG?\r\n"); 
                minuteCnt=0;
                step = 1;
                }
                u8CheckPhoneDownCnt = 0;
            }
            break;
        case 1: //�����˼��ָ��󣬵�200ms ��ⷵ�ص����ݣ�
            if(u8CheckPhoneDownCnt >= 100) //200ms��ⷵ�ص�����
            {
                for (j = 0; j < GSM_BUFF_LENGTH; j++)
                {
                    if (GSM_recvBuff[j] == 'G' && GSM_recvBuff[j + 1] == ':')
                    {
                         /*ע���˱������� �� �������� ���ɹ�*/
                         if (GSM_recvBuff[j + 5] == '1' || GSM_recvBuff[j + 5] == '5' ) 
                         {
                             ucRestartCnt=0;  //ֻ����������5�ζ�ʧ�� �Ų���ִ��
                         }else /*������� �����е��������ֻ�ģ��*/
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
                _PWR = 1;  //�ֻ�ģ����ƿ��ػ���pwr����
                uiPhoneRestartCnt = 0;
                step = 1;
            }
            break;
        case 1:
            if(uiPhoneRestartCnt >= 1000)
            {
                _PWR = 0;  //�ֻ�ģ�鿪������
                uiPhoneRestartCnt = 0;
                step = 2;
            }
            break;
        case 2:
            if(uiPhoneRestartCnt >= 1000)
            {
                _PWR = 1;  //�ֻ�ģ�鿪������
                uiPhoneRestartCnt = 0;
                step = 3;
            }
            break;
        case 3:
            if(uiPhoneRestartCnt >= 1000)
            {
                _PWR = 0;  //�ֻ�ģ�鿪������
                uiPhoneRestartCnt = 0;
                step = 4;
            }
            break;
        case 4:
            if(uiPhoneRestartCnt >= 15000) //������30s�� �����¶���ֱ�������
            {
                printf("AT+CNMI=3,2,0,0,0\r\n"); //�¶���ֱ����� AT+CNMI=3,2,0,0,0    �����õ��粻���桿
                uiPhoneRestartCnt = 0;
                step = 0;
                phoneRestartFlag = 0; /*�ֻ�������־ָ�� ���� ����һ�μ�⵽����������ע��״̬ ������������ָ��*/
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
            //�ر��ƶ�����
            temp = Send_AT_Cmd("AT+CIPSHUT\r\n", "SHUT OK",
                               150); //

            if (temp == 2) //�������ָ���?���ص�������OK��ʾ�ɹ�?��ʱtemp=2
            {
                break;
            }
            if (temp == 3) //�������ζ�����
            {
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
                break;
            }
            if (temp == 3) //�������ζ�����
            {
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
                break;
            }
            if (temp == 3) //�������ζ�����
            {
                break;
            }
        }
        while (1)
        {
            //����GPRSҵ��
            temp = Send_AT_Cmd("AT+CGATT=1\r\n", "OK",
                               150); //

            if (temp == 2) //�������ָ���?���ص�������OK��ʾ�ɹ�?��ʱtemp=2
            {
                break;
            }
            if (temp == 3) //�������ζ�����
            {
                break;
            }
        }
        while (1)
        {
            //����ΪGPRS����ģʽ
            temp = Send_AT_Cmd("AT+CIPCSGP=1,\"CMNET\"\r\n", "OK",
                               150); //

            if (temp == 2) //�������ָ���?���ص�������OK��ʾ�ɹ�?��ʱtemp=2
            {
                break;
            }
            if (temp == 3) //�������ζ�����
            {
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
                break;
            }
            if (temp == 3) //�������ζ�����
            {
                break;
            }
        }
				        printf("AT+CIPSTART=\"TCP\",\"");       //          |
                printf(GPRS_ServerIp);                  //          |
                printf("\",\"");                        //           > SIM��Ip��ַ�Ͷ˿�����
                printf(GPRS_ServerPort);                //          |
                printf("\"\r\n");                       //          |
}
