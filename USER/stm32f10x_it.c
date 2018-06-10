
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include  "stdio.h"
#include  "user.h"

#include  "tools.h"
#include "HX711.h"
#include "sim900a.h"
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/
u8 USART3_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART3_RX_STA=0;//����״̬���	
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
*������ ��        USART1_IRQHandler

*�������� ��      ��

*��������ֵ :     ��

*������������ ��  ����1���жϺ���
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
						
							if (FindChar(GSM_recvBuff,"hello")) //��GSM_recvBuff[]�����������û�к��� ref��ô���ַ�
                {
                  LCD_ShowString(0, 160, 209, 24, 24, " OK  hello");//17*12=204
                }
					if (FindChar(GSM_recvBuff,"err")) //��GSM_recvBuff[]�����������û�к��� ref��ô���ַ�
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

	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)  //���������ж�
	{
		Temp_Clear_IDLE=USART3->SR;
		Temp_Clear_IDLE=USART3->DR;//���USART_IT_IDLEλ
		USART3_RX_STA|=0x8000;
		gprsService = 1;	
		sprintf(gprsSendBuff,"%s,things\r\n", USART3_RX_BUF);
		Delay_ms(10);
		SendStringBy_USART1(USART3_RX_BUF);
		memset( USART3_RX_BUF, 0x00, sizeof(USART3_RX_BUF) ); //�������
	  LCD_ShowString(0, 160, 209, 24, 24, "Hello Data");//17*12=204
		USART3_RX_STA=0;

	
	}
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{	
			 //USART_ClearITPendingBit(USART3, USART_IT_RXNE);
			USART3_RX_BUF[USART3_RX_STA&0X7FFF]=USART_ReceiveData(USART3);//(USART1->DR);	//��ȡ���յ�������
			USART3_RX_STA++;
					
						
						
		}
	
	
}

/************************************************
*������ ��        EXTI3_IRQHandler

*�������� ��      ��

*��������ֵ :     ��

*������������ ��  EXTI��3�ϵ��ж�
*************************************************/
void EXTI3_IRQHandler()
{
    static uint8_t u8ExtiCnt = 0;
    if (EXTI_GetITStatus(EXTI_Line3) !=
        RESET) //�����ⲿ�ж����Ϸ�����ѡ��ı����¼�����λ���� 1����λ�������0
    {
        EXTI_ClearITPendingBit(EXTI_Line3); //����������жϱ�־λ�����㣬���һֱ���жϣ���
        u8ExtiCnt++;
        printf("�����ж�ʽ��� ���óɹ���%d", u8ExtiCnt);
    }
}


/************************************************
*������ ��        TIM4_IRQHandler

*�������� ��      ��

*��������ֵ :     ��

*������������ ��  ϵͳ���ʱ�����жϺ�����2.5ms�����ж�һ��
*************************************************/
void  TIM4_IRQHandler()
{

    if (TIM_GetITStatus(TIM4,
                        TIM_IT_Update) != RESET) //�����������¼�ʱ����Ϊ��Ӳ����1�����������0
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
        RESET) //�����ⲿ�ж����Ϸ�����ѡ��ı����¼�����λ���� 1����λ�������0
    {
        EXTI_ClearITPendingBit(
        EXTI_Line8); //����������жϱ�־λ�����㣬���һֱ���жϣ���

     }
}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
