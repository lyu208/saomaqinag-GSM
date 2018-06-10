
/**
  ******************************************************************************
  * @file    ads1230.c
  * @author  ����
  * @date    2016��5��17��
  * @brief    
  ******************************************************************************
  * @attention
  *     Ӳ������:  Ӳ������ ������Ӧ������ �����ұʼǱ���д��
  *    
  *
  *
  *                COPYRIGHT 2016 ���������������豸�з�����
  ******************************************************************************
  */

/**
*  ʵ��˼· �� ��оƬ������ȡpt100�¶�����
1���ϵ����һ��У׼����
2����һ���ⲿ�½��ص�ƽ ���жϷ�ʽ�� ADS1230 ��������־����ȡ���ݲ����ⲿ�жϣ���
3��������ѭ�� ADS1230 ����������ȡ����
4����ȡ��Ϻ󣬽������ģʽ��1���Ӵ���ģʽ���ѣ��ٽ������ݵĶ�ȡ~
*/
#include "ads1230.h"
#include "delay.h"

void ControlConfig()  //CA0 �� CA1 ����   һ��һ�;���
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
//   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; ����ģʽ ������������

    GPIO_Init(GPIOC, &GPIO_InitStructure);


    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource14); //��PC14�����ⲿ�ж�

    EXTI_InitStructure.EXTI_Line = EXTI_Line14; //PC14���ⲿ�ж��ߣ���Ӧ����EXTI15_10_IRQn
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;

    EXTI_Init(&EXTI_InitStructure);





    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //PC14���ⲿ�ж��ߣ���Ӧ����EXTI15_10_IRQn
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03; //��ռ���ȼ�  ����Խ�� ���ȼ�Խ��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //��Ӧ���ȼ�   ����Խ�� ���ȼ�Խ��

    NVIC_Init(&NVIC_InitStructure);


}


void  EnterStandbyMode()
{
    ADS1230_SCLK = 1; //20us�� �������ģʽ

}
void  ExitStandbyMode()
{
    ADS1230_SCLK = 0; //401.8ms �� �����������ݵ���~

}

void  ADS1230_Init()
{
    ADS1230_PDWN = 1; //Ҫ��ADS1230�ϵ��10us�����   ��1��ʼ����
    ADS1230_SCLK = 0; //����0  �ϵ������һ���ⲿ�ж�

//    ADS1230_DOUT = 1;
    EnterStandbyMode();

}



void OffsetCalibration() //У׼
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
    //�ٷ���һ�����壬�ӻ����������� ADS1230_DOUTΪ�ߵ�ƽ�� �������ADS1230_DOUT=1 ��ô��
    for (i = 0; i < 1; i++)
    {
        ADS1230_SCLK = 1;
        Delay_us(4);
        ADS1230_SCLK = 0;
        Delay_us(4);
    }
    //ReadData 20bit ���Ϊ 1048575

    ReadData &= 0xFFFFF;

//  if (ReadData > 0x7ffff)
//  {
//
//      ReadData = 1048576 - ReadData;
//
//  }
    return ReadData;

}









