/**************************************************************************
                           冲霄科技有限公司
***************************************************************************/
/**************************************************************************
【有言在先】
SPI 协议（Serial Peripheral Interface），即串行外围设备接口，
是一种高速全双工的通信总线，它由摩托罗拉公司提出，
***************************************************************************/



#include "spi.h"




/************************************************
*函数名 ：        SPI2_Config

*函数参数 ：      无

*函数返回值 :     无

*函数功能描述 ：  SPI2 配置函数

*其它 ：GPIO 用作EXTI外部中断或重映射功能的时候，必须开启 AFIO 时钟，使用默认复用功能，不需要开启AFIO时钟
        PB12(NSS)、PB13(SCK)、PB14(MISO)、PB15(MOSI)  ---《STM32F103数据手册(中文).pdf》第22页
        PB12(NSS)  不用，我们此处用软件方式控制片选！！(PC6)     
*************************************************/
void SPI2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef SPI_InitStructure;
	
	
  //开启相应的时钟  -----  GPIOB 和GPIOC  在APB2总线上    SPI2 在APB1总线上
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	
	// SCK 和 MOSI 的配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;//SCK 和 MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//《STM32参考手册_V10.pdf》--第110页--【8.1.11 外设的GPIO配置】
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	// MISO 的配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;// MISO 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //《STM32参考手册_V10.pdf》--第110页--【8.1.11 外设的GPIO配置】
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
  //片选的配置   配置PC6 片选引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 为推挽输出模式 

	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_6); //高电平为非选中状态。
	
	
	
	/******************************************************************************
	一般常见的IC器件用于SPI通信的模式一般是 模式0和模式3   现在归档一下，方便查阅：
	 
	模式0:SPI空闲时，SCK为低电平，数据的采样边沿为奇数边沿
	  ----  时钟极性CPOL=SPI_CPOL_Low(低)      时钟相位CPHA=SPI_CPHA_1Edge(奇)    
	
	模式3:SPI空闲时，SCK为高电平，数据的采样边沿为偶数边沿
	  ----  时钟极性CPOL=SPI_CPOL_High(高)     时钟相位CPHA=SPI_CPHA_2Edge(偶)     
	
	******************************************************************************/
	
	

	SPI_Cmd(SPI2, DISABLE); //失能后方可配置？？有点疑问！！
  //SPI 模式0 配置
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // 比特率分频
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //【时钟相位】 奇数边沿采集数据（spi的配置一定要配置好哪个模式，不然数据比如读取错误，死都不知道！！）
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //【时钟极性】 SPI通信空闲时(理解为刚把NSS拉低时对应的地方即可)SCK为低电平
	SPI_InitStructure.SPI_CRCPolynomial = 7; //定义了用于CRC值计算的多项式-----
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // 每次通信的数据大小为 8位数据
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // 双线全双工
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //数据字节传输都是高位（ MSB）在前低位（LSB）在后（W5100手册规定）
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //主模式
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //软件片选

	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
	
}



/************************************************
*函数名 ：        SPI2_ReadWrite_Byte

*函数参数 ：      数据

*函数返回值 :     无

*函数功能描述 ：  SPI2 读写函数  SPI通信的基本函数体

*其它     要进行数据的接收，也需要进行发送才可以，不然主机没法发送SCK 无法进行读取
*************************************************/
uint8_t SPI2_ReadWrite_Byte(uint8_t data)
{
	//参看《STM32参考手册_V10.pdf》 ---第489页  ---一看就明白
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //发送缓冲区非空，一直死在这里等待发送完成
		;

	
	SPI_I2S_SendData(SPI2, data);//发送缓冲区为空了，才进行这一步，进行数据的发送

	//参看《STM32参考手册_V10.pdf》 ---第489页  ---一看就明白
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//接收缓冲区为空，一直死在这里
		;

	
	return SPI_I2S_ReceiveData(SPI2);//接收缓冲区不空了，说明有数据了，开始进行数据的接收
}
















void SPI1_Config(void)
{
	SPI1_GPIO_Config();
	SPI1_Struct_Config();

}




/*
 * SPI1基本配置
 * */
void SPI1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);

	/*
	 * 配置AT45DB161 复用推挽输出     ？？
	 * */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*
	 * 配置AT45DB161 片选引脚为推挽输出模式
	 * */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_8); //先不选中该芯片，等到用到的时候，再选中芯片。
}

/*
 * SPI1基本配置
 * */
void SPI1_Struct_Config(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_Cmd(SPI1, DISABLE); //失能后方可配置

	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}



/*
 * SPI1读写函数，实现SPI通信最基本的函数体！！
 * */

uint8_t SPI1_ReadWrite_Byte(uint8_t data)
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		;

	/* Send Half Word through the SPI1 peripheral */
	SPI_I2S_SendData(SPI1, data);

	/* Wait to receive a Half Word */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
		;

	/* Return the Half Word read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1);
}



