# 智能电子秤+新大陆扫码枪V1.0
扫码枪使用的战舰精英板STM32的串口三，PB10,11引脚
程序在原来的电子秤修改而来 ：
1. void  HeadFirst(void){}里初始化USART3_Config();
2. 使用的是串口三“空闲中断”方法来接受扫码枪串口的数据
3. 一系列的引脚，功能和中断的使能。在void USART3_Config(void){}中配置。
USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//开启空闲中断
void RCC_Config(void){}的串口三的时钟使能。
4. 写void USART3_IRQHandler(){}空闲中断函数（参考我的博客历程https://blog.csdn.net/qq_38405680/article/details/80560304）
5.编写user.c的按键函数，扫完二维码之后，电子秤的键盘按‘#键’，通过GSM模块与服务器传数据，

 

  

