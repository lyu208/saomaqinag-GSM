
/**
  ******************************************************************************
  * @file    user.c
  * @author  梦之创
  * @date    2018年5月5日
  * @brief   系统功能实现在此文件内
  ******************************************************************************
  * @attention
  *    注意事项:
  *
  *
  *
  *                COPYRIGHT 2018 梦之创物联网设备研发中心
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
*函数名 :       RCC_Config

*函数参数 :      无

*函数返回值 :     无

*函数功能描述 :   需要的各个时钟 统一在这里开启即可

*注意 :
*************************************************/
void RCC_Config(void)
{
    /*      【APB2时钟 最大72M】 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,
                           ENABLE); /* 任何一个端口，当有发生了重映射 要开启此时钟
                           GPIO用作外部中断EXTI的时候，必须开启AFIO时钟！*/


    /*                                                         */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); /*  */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); /* 串口3 PB10 PB11*/
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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); /* 串口1 485通信*/
    /*                                                         */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);   */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);   */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);  */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);  */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);  */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);  */
    /*  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);  */


    /*      【APB1时钟 最大36M】 */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   */
    /*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); /* 通用定时器4 时间基准*/
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
                         ENABLE); /* 单片机与手机模块的通信用串口3*/

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



    /*  【AHB总线时钟】   这个较复杂 写的时候自己查询注意*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


}

/********************************************************************
*函数名 ：        HeadFirst

*函数参数 ：      无

*函数返回值 :     无

*函数功能描述 ：  HeadFirst 此函数，是单片机上电时执行的一些基本的初始化函数。
*********************************************************************/
void  HeadFirst(void)
{
    Delay_init();
    Delay_ms(100);

    verison = 1.00;

    RCC_Config(); //配置与程序相关的一些外设的时钟


    LED_Config();



    USART1_Config();
    USART2_Config();
		USART3_Config();
		SendStringBy_USART1("串口初始化1");

    LCD_Config();


    LCD_Clear(BLACK);//GBLUE
    POINT_COLOR = BLACK;

    /*24号字符的大小，一个字符的宽度为320/26*/

    LCD_ShowString(60, 10, 264, 24, 24, "Library Inquiry"); //显示LCD ID  11*24=

   LCD_ShowString(0, 40, 123, 24, 24, "LYU208");//10*12=120
    LCD_ShowString(0, 80, 209, 24, 24, "xin xi 208:");//17*12=204

    POINT_COLOR = BLUE;

//    LCD_ShowString(120, 40, 123, 24, 24, "0");//称重重量默认显示0
//    LCD_ShowString(204, 80, 123, 24, 24, "0");//单价默认显示0

  

    KeyPadConfig();
		SendStringBy_USART1("串口初始化2");
    TIM4_Config(); //这个通用定时器4就是用来做我程序框架的时间基准！
		SendStringBy_USART1("串口初始化3");
		a1=1;
    TCP_ConnectServer();//TCP连接服务器
    // printf("进入系统");
		SendStringBy_USART1("串口初始化4");
   
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
            if (temp == 2) //如果发送指令后?返回的数据有OK表示成功?此时temp=2
            {
                if(gprsService)//有效通信
                {
                    printf(gprsSendBuff);
									memset( USART3_RX_BUF, 0x00, sizeof(USART3_RX_BUF) ); //清空数组
							
                }
                else if(heartbeatService)  //有效通信的时候 就不要发心跳了  心跳6s一次
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
            if (temp == 3) //连续三次都错误
            {

                USART_ClearFlag(USART2, USART_FLAG_TC);
                USART_SendData(USART2, 0x1B);
                while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

                SendStringBy_USART1("GPRS Send Err\r\n");
              	memset( USART3_RX_BUF, 0x00, sizeof(USART3_RX_BUF) ); //清空数组
            }
            break;

        case 1:


            break;

    }



}

