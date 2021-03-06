/**
  ******************************************************************************
  * @file    peripheral.c
  * @author  梦之创
  * @date    2018年5月16日
  * @brief   外设的所有配置都在此文件内
  ******************************************************************************
  * @attention
  *    注意事项:
  *
  *
  *
  *                COPYRIGHT 2018 梦之创物联网设备研发中心
  ******************************************************************************
  */

#include "peripheral.h"
#include "user.h"




/************************************************
*函数名 :       LED_Config

*函数参数 :      无

*函数返回值 :     无

*函数功能描述 :

*注意 :    DS0 -- LED0 -- PB5 ; DS1 -- LED1 -- PE5
*************************************************/
void LED_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化后是低电平
    GPIO_Init(GPIOE, &GPIO_InitStructure); //初始化后是低电平

    GPIO_SetBits(GPIOB, GPIO_Pin_5);
    GPIO_SetBits(GPIOE, GPIO_Pin_5);
}


/************************************************
*函数名 :       Key4_IT_Config

*函数参数 :      无

*函数返回值 :     无

*函数功能描述 :   配置一个按键为中断的方式 借机学习一下中断触发  ;
 * 但实际用中断方式的按键很少用 因为抖动的缘故

*注意 :
*************************************************/
void Key4_IT_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    /*GPIO用作外部中断EXTI的时候，必须开启AFIO时钟！！！！*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO_Mode_IN_FLOATING;
//   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; 输入模式 无需配置速率

    GPIO_Init(GPIOC, &GPIO_InitStructure);


    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3); //把PC3用作外部中断

    EXTI_InitStructure.EXTI_Line = EXTI_Line3; //PC3的外部中断线，对应的是EXTI3
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;

    EXTI_Init(&EXTI_InitStructure);





    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel =
        EXTI3_IRQn; //PC3的外部中断线，对应的是EXTI3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =
        0x00; //抢占优先级  数字越低 优先级越高
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =
        0x01; //相应优先级   数字越低 优先级越高

    NVIC_Init(&NVIC_InitStructure);
}


/************************************************
*函数名 :       USART1_Config

*函数参数 :      无

*函数返回值 :     无

*函数功能描述 :   配置串口1

*************************************************/
void USART1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;


    NVIC_InitTypeDef
    NVIC_InitStructure; //【配置串口接收中断用的，如果不需要串口中断，请注释！】

    //GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE); //串口1 需要重映射


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //  USART1_TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //推挽复用输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);


    /*    PA10 是RX端，可以设置浮空输入或上拉输入。。因为起始信号是低电平，所以上拉输入是可行的！！！ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //  USART1_RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    GPIO_Init(GPIOA, &GPIO_InitStructure);


    /*
     * 以下为串口的结构体的一些基本配置了
     * */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //双线全双工
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;


    USART_Init(USART1, &USART_InitStructure);




    USART_Cmd(USART1, ENABLE); //使能外设，真鸡巴费劲 使能了时钟还得再使能外设 烦人

    /**************************************************************
    【注意】 至此，以上的配置就可以确保了串口通信的正常工作，比如向调试助手
    发送点信息之类的就可以了！！！

    ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^

    【看这里】！！！！ 如果需要配置串口的中断方式，比如要中断接收，那么继续往下配置！当然
    如果不需要的话，注释掉就可以了！！！
    **************************************************************/

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //串口接收中断使能

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //没用用来配置抢占优先级
    //      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//1位用来配置抢占优先级
    //      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//2位用来配置抢占优先级
    //      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);//3位用来配置抢占优先级
    //      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//4位用来配置抢占优先级


    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =
        0x01; //抢占优先级，数值越小，优先级越高。
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =
        0x05; //响应优先级，数值越小，优先级越高.
    //抢占优先级是用在中断嵌套的，即高的优先级可以打断低的优先级中断。。。
    //而响应优先级则是两个抢占优先级的中断同事到达时，优先处理相应优先级高的中断。。。
    NVIC_Init(&NVIC_InitStructure);

}


void  USART2_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;


    NVIC_InitTypeDef
    NVIC_InitStructure; //【配置串口接收中断用的，如果不需要串口中断，请注释！】

    //GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE); //串口1 需要重映射


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //  USART2_TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //推挽复用输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);


    /*    PA3 是RX端，可以设置浮空输入或上拉输入。。因为起始信号是低电平，所以上拉输入是可行的！！！ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //  USART2_RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    GPIO_Init(GPIOA, &GPIO_InitStructure);


    /*
     * 以下为串口的结构体的一些基本配置了
     * */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //双线全双工
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;


    USART_Init(USART2, &USART_InitStructure);




    USART_Cmd(USART2, ENABLE); //使能外设，真鸡巴费劲 使能了时钟还得再使能外设 烦人

    /**************************************************************
    【注意】 至此，以上的配置就可以确保了串口通信的正常工作，比如向调试助手
    发送点信息之类的就可以了！！！

    ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^

    【看这里】！！！！ 如果需要配置串口的中断方式，比如要中断接收，那么继续往下配置！当然
    如果不需要的话，注释掉就可以了！！！
    **************************************************************/

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //串口接收中断使能

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //没用用来配置抢占优先级
    //      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//1位用来配置抢占优先级
    //      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//2位用来配置抢占优先级
    //      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);//3位用来配置抢占优先级
    //      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//4位用来配置抢占优先级


    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =
        0x01; //抢占优先级，数值越小，优先级越高。
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =
        0x04; //响应优先级，数值越小，优先级越高.
    //抢占优先级是用在中断嵌套的，即高的优先级可以打断低的优先级中断。。。
    //而响应优先级则是两个抢占优先级的中断同事到达时，优先处理相应优先级高的中断。。。
    NVIC_Init(&NVIC_InitStructure);


}

void USART3_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;


    NVIC_InitTypeDef
    NVIC_InitStructure; //【配置串口接收中断用的，如果不需要串口中断，请注释！】


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // PB10为 USART3_TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //推挽复用输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);


    /*    PA10 是RX端，可以设置浮空输入或上拉输入。。因为起始信号是低电平，所以上拉输入是可行的！！！ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; // PA10为 USART1_RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    GPIO_Init(GPIOB, &GPIO_InitStructure);


    /*
     * 以下为串口的结构体的一些基本配置了
     * */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //双线全双工
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;


    USART_Init(USART3, &USART_InitStructure);

    USART_Cmd(USART3, ENABLE); //使能外设，真鸡巴费劲 使能了时钟还得再使能外设 烦人

    /**************************************************************
    【注意】 至此，以上的配置就可以确保了串口通信的正常工作，比如向调试助手
    发送点信息之类的就可以了！！！

    ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^

    【看这里】！！！！ 如果需要配置串口的中断方式，比如要中断接收，那么继续往下配置！当然
    如果不需要的话，注释掉就可以了！！！
    **************************************************************/

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //串口接收中断使能
		USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//开启空闲中断
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //没用用来配置抢占优先级
//      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//1位用来配置抢占优先级
//      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//2位用来配置抢占优先级
//      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);//3位用来配置抢占优先级
//      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//4位用来配置抢占优先级


    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =
        0x00; //抢占优先级，数值越小，优先级越高。
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =
        0x00; //响应优先级，数值越小，优先级越高.
    //抢占优先级是用在中断嵌套的，即高的优先级可以打断低的优先级中断。。。
    //而响应优先级则是两个抢占优先级的中断同事到达时，优先处理相应优先级高的中断。。。
    NVIC_Init(&NVIC_InitStructure);
}
/************************************************
*函数名 :       TIM4_Config

*函数参数 :      无

*函数返回值 :     无

*函数功能描述 :   通用定时器4就是用来做我程序框架的时间基准！2.5ms中断一次

*注意 :
*************************************************/
void TIM4_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

//     TIM_DeInit (TIM7);//Deinitializes the TIMx peripheral registers to their default reset values.

    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInitStructure.TIM_Period = 5;
    TIM_TimeBaseInitStructure.TIM_Prescaler =
        35999; //72000000/36000=2000HZ 也就是0.5ms一个计数脉冲

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);


    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;

    NVIC_Init(&NVIC_InitStructure);

    TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //清除一下已经产生的更新中断标志位
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //Enables the  TIM4 interrupts
    TIM_Cmd(TIM4, ENABLE); //启动定时器开始定时    【1法】
//       TIM4->CR1 |=0x01;//启动定时器开始定时     【2法】
//       TIM_Cmd(TIM4,DISABLE);//关闭定时器计时    【1法】
//       TIM4->CR1 &=~0x01;//关闭定时器计时        【2法】
}


/************************************************
*函数名 :       PVD_Init

*函数参数 :

*函数返回值 :     无

*函数功能描述 :   配置掉电检测中断函数

*注意 :   stm32 参考手册 P39
*************************************************/
void PVD_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    EXTI_InitTypeDef EXTI_InitStructure;

    NVIC_InitStruct.NVIC_IRQChannel = PVD_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;


    NVIC_Init(&NVIC_InitStruct); //允许中断

    EXTI_StructInit(&EXTI_InitStructure);
    /*       EXTI_ClearITPendingBit(EXTI_Line16); */
    EXTI_InitStructure.EXTI_Line = EXTI_Line16;  //PVD连接到中断线16上
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger =
        EXTI_Trigger_Rising; //看数据手册 当上升沿是掉电 下降沿是上电
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);


    PWR_PVDLevelConfig(
        PWR_PVDLevel_2V9); //设定监控阀值   灵活设置 当2.5不行 我改为2.9 就可以了
    PWR_PVDCmd(ENABLE);
}


void KeyPadConfig(void)
{
    //行是上拉模式  列是推挽输出模式
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_12|GPIO_Pin_0|GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	  GPIO_SetBits(GPIOA, GPIO_Pin_7 | GPIO_Pin_12|GPIO_Pin_6|GPIO_Pin_8);
	
	
//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//	  GPIO_Init(GPIOF, &GPIO_InitStructure);
//	   
//		GPIO_SetBits(GPIOF, GPIO_Pin_6 | GPIO_Pin_7);
}


void  SendDataBy_USART1(int8_t data)
{
    USART_ClearFlag(USART1, USART_FLAG_TC);
    USART_SendData(USART1, (int8_t)data);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}


void  SendStringBy_USART1(char *data)
{
   while(*data!=0)
	 {
	   SendDataBy_USART1(*data);
	   data++;
	 }
}



