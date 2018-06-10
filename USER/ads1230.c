
/**
  ******************************************************************************
  * @file    ads1230.c
  * @author  侯霄
  * @date    2016年5月17日
  * @brief    
  ******************************************************************************
  * @attention
  *     硬件连接:  硬件连接 包括对应的名字 都在我笔记本上写着
  *    
  *
  *
  *                COPYRIGHT 2016 冲霄集团物联网设备研发中心
  ******************************************************************************
  */

/**
*  实现思路 ： 此芯片用来读取pt100温度数据
1、上电进行一次校准操作
2、第一个外部下降沿电平 靠中断方式， ADS1230 服务函数标志（读取数据不在外部中断）。
3、启动主循环 ADS1230 服务函数，读取数据
4、读取完毕后，进入待机模式，1秒后从待机模式唤醒，再进行数据的读取~
*/
#include "ads1230.h"
#include "delay.h"

void ControlConfig()  //CA0 和 CA1 配置   一高一低就行
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_1);
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}
void  ADS1230_Config()
{


    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    ControlConfig();

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(GPIOC, &GPIO_InitStructure);



    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO_Mode_IN_FLOATING;
//   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; 输入模式 无需配置速率

    GPIO_Init(GPIOC, &GPIO_InitStructure);


    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource14); //把PC14用作外部中断

    EXTI_InitStructure.EXTI_Line = EXTI_Line14; //PC14的外部中断线，对应的是EXTI15_10_IRQn
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;

    EXTI_Init(&EXTI_InitStructure);





    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //PC14的外部中断线，对应的是EXTI15_10_IRQn
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03; //抢占优先级  数字越低 优先级越高
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //相应优先级   数字越低 优先级越高

    NVIC_Init(&NVIC_InitStructure);


}


void  EnterStandbyMode()
{
    ADS1230_SCLK = 1; //20us后 进入待机模式

}
void  ExitStandbyMode()
{
    ADS1230_SCLK = 0; //401.8ms 后 就能有新数据到来~

}

void  ADS1230_Init()
{
    ADS1230_PDWN = 1; //要在ADS1230上电后10us后操作   置1后开始工作
    ADS1230_SCLK = 0; //等于0  上电就启动一次外部中断

//    ADS1230_DOUT = 1;
    EnterStandbyMode();

}



void OffsetCalibration() //校准
{
    uint8_t i = 0;
    for (i = 0; i < 26; i++)
    {
        ADS1230_SCLK = 0;
        Delay_us(4);
        ADS1230_SCLK = 1;
        Delay_us(4);
    }
}

uint32_t ReadDataFro_ADS1230()
{
    uint8_t i = 0;
    uint32_t  ReadData = 0;
    for (i = 0; i < 20; i++)
    {
        ReadData <<= 1;

        ADS1230_SCLK = 1;
        Delay_us(4);
        if (ADS1230_DOUT_IN)
        {
            ReadData = ReadData | 0x01;
 
        }
        ADS1230_SCLK = 0;
        Delay_us(4);
    }
    //再发送一个脉冲，从机拉高数据线 ADS1230_DOUT为高电平， 如果主机ADS1230_DOUT=1 行么？
    for (i = 0; i < 1; i++)
    {
        ADS1230_SCLK = 1;
        Delay_us(4);
        ADS1230_SCLK = 0;
        Delay_us(4);
    }
    //ReadData 20bit 最大为 1048575

    ReadData &= 0xFFFFF;

//  if (ReadData > 0x7ffff)
//  {
//
//      ReadData = 1048576 - ReadData;
//
//  }
    return ReadData;

}









