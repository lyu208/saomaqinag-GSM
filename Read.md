# ���ܵ��ӳ�+�´�½ɨ��ǹV1.0
ɨ��ǹʹ�õ�ս����Ӣ��STM32�Ĵ�������PB10,11����
������ԭ���ĵ��ӳ��޸Ķ��� ��
1. void  HeadFirst(void){}���ʼ��USART3_Config();
2. ʹ�õ��Ǵ������������жϡ�����������ɨ��ǹ���ڵ�����
3. һϵ�е����ţ����ܺ��жϵ�ʹ�ܡ���void USART3_Config(void){}�����á�
USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//���������ж�
void RCC_Config(void){}�Ĵ�������ʱ��ʹ�ܡ�
4. дvoid USART3_IRQHandler(){}�����жϺ������ο��ҵĲ�������https://blog.csdn.net/qq_38405680/article/details/80560304��
5.��дuser.c�İ���������ɨ���ά��֮�󣬵��ӳӵļ��̰���#������ͨ��GSMģ��������������ݣ�

 

  

