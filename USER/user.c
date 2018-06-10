
/**
  ******************************************************************************
  * @file    user.c
  * @author  ��֮��
  * @date    2018��5��5��
  * @brief   ϵͳ����ʵ���ڴ��ļ���
  ******************************************************************************
  * @attention
  *    ע������:
  *
  *
  *
  *                COPYRIGHT 2018 ��֮���������豸�з�����
  ******************************************************************************
  */

#include "user.h"

#include "delay.h"
#include "tools.h"
#include "peripheral.h"
#include "lcd.h"
#include "HX711.h"
#include "sim900a.h"
#include "stm32f10x_it.h"
u8 len;	
u8 t;
extern  u8 ReceiveState;
extern u16 USART3_RX_STA;
extern u8 USART3_RX_BUF[200];
int a1=0;
float  verison = 0;

uint16_t DS0_TimeCnt = 0;

uchar keyPadRowScan = 0;
uint16_t keyPadCnt = 0;
uchar keyTouch = 0;
uchar getWeightService = 0;
uchar getWeightCnt = 0;
//char weightStr[10] = {0};
//char unitPrice[8] = {0};
//char totalMoney[8] = {0};
//char totalWeight[20]={0};
char gprsSendBuff[50] = {0};
uchar tradeLock = 0;
uchar  ledFlickFlag = 0;
uchar gprsService = 0;
uint16_t heartbeat = 0;
uchar cunnectFlag = 0;
uchar heartbeatService = 0;


void RCC_Config(void);
void DISPLAY_RENCODE_TO_TFT(uint8_t *qrcode_data);
/************************************************
*������ :       RCC_Config

*�������� :      ��

*��������ֵ :     ��

*������������ :   ��Ҫ�ĸ���ʱ�� ͳһ�����￪������

*ע�� :
*************************************************/
void RCC_Config(void)
{
    /*      ��APB2ʱ�� ���72M�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,
                           ENABLE); /* �κ�һ���˿ڣ����з�������ӳ�� Ҫ������ʱ��
                           GPIO�����ⲿ�ж�EXTI��ʱ�򣬱��뿪��AFIOʱ�ӣ�*/


    /*                                                         */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); /*  */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); /* ����3 PB10 PB11*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); /*  */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); /*  */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    //  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);  */
    /*                                                         */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);   */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);   */
    /*                                                         */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);   */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); /* ����1 485ͨ��*/
    /*                                                         */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);   */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);   */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);  */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);  */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);  */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);  */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);  */


    /*      ��APB1ʱ�� ���36M�� */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); /* ͨ�ö�ʱ��4 ʱ���׼*/
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);   */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);   */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);   */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);  */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);  */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);  */
    /*                                                         */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);   */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);   */
    /*                                                         */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);   */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);   */
    /*                                                         */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,
                         ENABLE); /* ��Ƭ�����ֻ�ģ���ͨ���ô���3*/

    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART4, ENABLE); */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART5, ENABLE); */
    /*                                                         */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);   */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);    */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);   */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);    */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);    */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CEC, ENABLE);    */



    /*  ��AHB����ʱ�ӡ�   ����ϸ��� д��ʱ���Լ���ѯע��*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


}

/********************************************************************
*������ ��        HeadFirst

*�������� ��      ��

*��������ֵ :     ��

*������������ ��  HeadFirst �˺������ǵ�Ƭ���ϵ�ʱִ�е�һЩ�����ĳ�ʼ��������
*********************************************************************/
void  HeadFirst(void)
{
    Delay_init();
    Delay_ms(100);

    verison = 1.00;

    RCC_Config(); //�����������ص�һЩ�����ʱ��


    LED_Config();



    USART1_Config();
    USART2_Config();
		USART3_Config();
		SendStringBy_USART1("���ڳ�ʼ��1");

    LCD_Config();


    LCD_Clear(BLACK);//GBLUE
    POINT_COLOR = BLACK;

    /*24���ַ��Ĵ�С��һ���ַ��Ŀ��Ϊ320/26*/

    LCD_ShowString(60, 10, 264, 24, 24, "Library Inquiry"); //��ʾLCD ID  11*24=

   LCD_ShowString(0, 40, 123, 24, 24, "LYU208");//10*12=120
    LCD_ShowString(0, 80, 209, 24, 24, "xin xi 208:");//17*12=204

    POINT_COLOR = BLUE;

//    LCD_ShowString(120, 40, 123, 24, 24, "0");//��������Ĭ����ʾ0
//    LCD_ShowString(204, 80, 123, 24, 24, "0");//����Ĭ����ʾ0

  

    KeyPadConfig();
		SendStringBy_USART1("���ڳ�ʼ��2");
    TIM4_Config(); //���ͨ�ö�ʱ��4�����������ҳ����ܵ�ʱ���׼��
		SendStringBy_USART1("���ڳ�ʼ��3");
		a1=1;
    TCP_ConnectServer();//TCP���ӷ�����
    // printf("����ϵͳ");
		SendStringBy_USART1("���ڳ�ʼ��4");
   
}



void  GPRS_Service()
{
    static uchar step = 0;

    uint8_t   temp = 0;
    if(gprsService == 0 && heartbeatService == 0) return;

    switch(step)
    {
        case 0:
            temp = Send_AT_Cmd("AT+CIPSEND\r\n", ">", 1000);
            if (temp == 2) //�������ָ���?���ص�������OK��ʾ�ɹ�?��ʱtemp=2
            {
                if(gprsService)//��Чͨ��
                {
                    printf(gprsSendBuff);
									memset( USART3_RX_BUF, 0x00, sizeof(USART3_RX_BUF) ); //�������
							
                }
                else if(heartbeatService)  //��Чͨ�ŵ�ʱ�� �Ͳ�Ҫ��������  ����6sһ��
                {
                    heartbeatService = 0;

                    USART_ClearFlag(USART2, USART_FLAG_TC);
                    USART_SendData(USART2, 0x00);
                    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
									  SendStringBy_USART1("Heart\r\n");
                }

                USART_ClearFlag(USART2, USART_FLAG_TC);
                USART_SendData(USART2, 0x1A);
                while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
                gprsService = 0;
            }
            if (temp == 3) //�������ζ�����
            {

                USART_ClearFlag(USART2, USART_FLAG_TC);
                USART_SendData(USART2, 0x1B);
                while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

                SendStringBy_USART1("GPRS Send Err\r\n");
              	memset( USART3_RX_BUF, 0x00, sizeof(USART3_RX_BUF) ); //�������
            }
            break;

        case 1:


            break;

    }



}

