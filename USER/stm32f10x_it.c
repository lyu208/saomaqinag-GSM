
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include  "stdio.h"
#include  "user.h"

#include  "tools.h"
#include "HX711.h"
#include "sim900a.h"
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/
u8 USART3_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA=0;//接收状态标记	
u16 ReceiveState=0;
extern char gprsSendBuff[50] ;
u8 USART2_state=0;


/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {}
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {}
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
//void PendSV_Handler(void)
//{
//}
/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */



/************************************************
*函数名 ：        USART1_IRQHandler

*函数参数 ：      无

*函数返回值 :     无

*函数功能描述 ：  串口1的中断函数
*************************************************/
void USART1_IRQHandler()
{


    if(USART_GetITStatus(USART1,
                         USART_IT_RXNE) != RESET)
    {

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);


    }
}

void USART2_IRQHandler()
{


    if(USART_GetITStatus(USART2,
                         USART_IT_RXNE) != RESET)
    {

        USART_ClearITPendingBit(USART2, USART_IT_RXNE);

        GSM_recvBuff[recvDataCnt] = USART2->DR; // 252

        recvDataCnt++;
			  USART2_state=1;	
				if(USART2_state==1)
						{
						
							if (FindChar(GSM_recvBuff,"hello")) //在GSM_recvBuff[]数组里，查找有没有含有 ref这么个字符
                {
                  LCD_ShowString(0, 160, 209, 24, 24, " OK  hello");//17*12=204
                }
					if (FindChar(GSM_recvBuff,"err")) //在GSM_recvBuff[]数组里，查找有没有含有 ref这么个字符
                {
                  LCD_ShowString(0, 160, 209, 24, 24, " err  hello");//17*12=204
                }		
						
						USART2_state=0;
						
						
						}				
			
    }
}





void USART3_IRQHandler()
{		
		u8 Temp_Clear_IDLE=Temp_Clear_IDLE; 

	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)  //空闲总线中断
	{
		Temp_Clear_IDLE=USART3->SR;
		Temp_Clear_IDLE=USART3->DR;//清除USART_IT_IDLE位
		USART3_RX_STA|=0x8000;
		gprsService = 1;	
		sprintf(gprsSendBuff,"%s,things\r\n", USART3_RX_BUF);
		Delay_ms(10);
		SendStringBy_USART1(USART3_RX_BUF);
		memset( USART3_RX_BUF, 0x00, sizeof(USART3_RX_BUF) ); //清空数组
	  LCD_ShowString(0, 160, 209, 24, 24, "Hello Data");//17*12=204
		USART3_RX_STA=0;

	
	}
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{	
			 //USART_ClearITPendingBit(USART3, USART_IT_RXNE);
			USART3_RX_BUF[USART3_RX_STA&0X7FFF]=USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
			USART3_RX_STA++;
					
						
						
		}
	
	
}

/************************************************
*函数名 ：        EXTI3_IRQHandler

*函数参数 ：      无

*函数返回值 :     无

*函数功能描述 ：  EXTI线3上的中断
*************************************************/
void EXTI3_IRQHandler()
{
    static uint8_t u8ExtiCnt = 0;
    if (EXTI_GetITStatus(EXTI_Line3) !=
        RESET) //当在外部中断线上发生了选择的边沿事件，该位被置 1，该位由软件清0
    {
        EXTI_ClearITPendingBit(EXTI_Line3); //如果不进行中断标志位的清零，则会一直进中断！！
        u8ExtiCnt++;
        printf("按键中断式检测 配置成功！%d", u8ExtiCnt);
    }
}


/************************************************
*函数名 ：        TIM4_IRQHandler

*函数参数 ：      无

*函数返回值 :     无

*函数功能描述 ：  系统框架时基的中断函数，2.5ms进入中断一次
*************************************************/
void  TIM4_IRQHandler()
{

    if (TIM_GetITStatus(TIM4,
                        TIM_IT_Update) != RESET) //当产生更新事件时，该为被硬件置1，它由软件清0
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

	      
			  
			 if (overTimeCnt > 0)
        {
            overTimeCnt--;
        }
			
				if(cunnectFlag)
				{
				heartbeat++;
					if(heartbeat>=2400)
					{
					heartbeatService=1;
						heartbeat=0;
					}
				
				}
    }
}
void EXTI9_5_IRQHandler()
{

    if (EXTI_GetITStatus(EXTI_Line8) !=
        RESET) //当在外部中断线上发生了选择的边沿事件，该位被置 1，该位由软件清0
    {
        EXTI_ClearITPendingBit(
        EXTI_Line8); //如果不进行中断标志位的清零，则会一直进中断！！

     }
}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
