/**************************************************************************
                           �����Ƽ����޹�˾
***************************************************************************/
/**************************************************************************
���������ȡ�
SPI Э�飨Serial Peripheral Interface������������Χ�豸�ӿڣ�
��һ�ָ���ȫ˫����ͨ�����ߣ�����Ħ��������˾�����
***************************************************************************/



#include "spi.h"




/************************************************
*������ ��        SPI2_Config

*�������� ��      ��

*��������ֵ :     ��

*������������ ��  SPI2 ���ú���

*���� ��GPIO ����EXTI�ⲿ�жϻ���ӳ�书�ܵ�ʱ�򣬱��뿪�� AFIO ʱ�ӣ�ʹ��Ĭ�ϸ��ù��ܣ�����Ҫ����AFIOʱ��
        PB12(NSS)��PB13(SCK)��PB14(MISO)��PB15(MOSI)  ---��STM32F103�����ֲ�(����).pdf����22ҳ
        PB12(NSS)  ���ã����Ǵ˴��������ʽ����Ƭѡ����(PC6)     
*************************************************/
void SPI2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef SPI_InitStructure;
	
	
  //������Ӧ��ʱ��  -----  GPIOB ��GPIOC  ��APB2������    SPI2 ��APB1������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	
	// SCK �� MOSI ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;//SCK �� MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��STM32�ο��ֲ�_V10.pdf��--��110ҳ--��8.1.11 �����GPIO���á�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	// MISO ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;// MISO 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��STM32�ο��ֲ�_V10.pdf��--��110ҳ--��8.1.11 �����GPIO���á�
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
  //Ƭѡ������   ����PC6 Ƭѡ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // Ϊ�������ģʽ 

	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_6); //�ߵ�ƽΪ��ѡ��״̬��
	
	
	
	/******************************************************************************
	һ�㳣����IC��������SPIͨ�ŵ�ģʽһ���� ģʽ0��ģʽ3   ���ڹ鵵һ�£�������ģ�
	 
	ģʽ0:SPI����ʱ��SCKΪ�͵�ƽ�����ݵĲ�������Ϊ��������
	  ----  ʱ�Ӽ���CPOL=SPI_CPOL_Low(��)      ʱ����λCPHA=SPI_CPHA_1Edge(��)    
	
	ģʽ3:SPI����ʱ��SCKΪ�ߵ�ƽ�����ݵĲ�������Ϊż������
	  ----  ʱ�Ӽ���CPOL=SPI_CPOL_High(��)     ʱ����λCPHA=SPI_CPHA_2Edge(ż)     
	
	******************************************************************************/
	
	

	SPI_Cmd(SPI2, DISABLE); //ʧ�ܺ󷽿����ã����е����ʣ���
  //SPI ģʽ0 ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // �����ʷ�Ƶ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //��ʱ����λ�� �������زɼ����ݣ�spi������һ��Ҫ���ú��ĸ�ģʽ����Ȼ���ݱ����ȡ����������֪��������
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //��ʱ�Ӽ��ԡ� SPIͨ�ſ���ʱ(���Ϊ�հ�NSS����ʱ��Ӧ�ĵط�����)SCKΪ�͵�ƽ
	SPI_InitStructure.SPI_CRCPolynomial = 7; //����������CRCֵ����Ķ���ʽ-----
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // ÿ��ͨ�ŵ����ݴ�СΪ 8λ����
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // ˫��ȫ˫��
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //�����ֽڴ��䶼�Ǹ�λ�� MSB����ǰ��λ��LSB���ں�W5100�ֲ�涨��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //��ģʽ
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //���Ƭѡ

	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
	
}



/************************************************
*������ ��        SPI2_ReadWrite_Byte

*�������� ��      ����

*��������ֵ :     ��

*������������ ��  SPI2 ��д����  SPIͨ�ŵĻ���������

*����     Ҫ�������ݵĽ��գ�Ҳ��Ҫ���з��Ͳſ��ԣ���Ȼ����û������SCK �޷����ж�ȡ
*************************************************/
uint8_t SPI2_ReadWrite_Byte(uint8_t data)
{
	//�ο���STM32�ο��ֲ�_V10.pdf�� ---��489ҳ  ---һ��������
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ͻ������ǿգ�һֱ��������ȴ��������
		;

	
	SPI_I2S_SendData(SPI2, data);//���ͻ�����Ϊ���ˣ��Ž�����һ�����������ݵķ���

	//�ο���STM32�ο��ֲ�_V10.pdf�� ---��489ҳ  ---һ��������
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//���ջ�����Ϊ�գ�һֱ��������
		;

	
	return SPI_I2S_ReceiveData(SPI2);//���ջ����������ˣ�˵���������ˣ���ʼ�������ݵĽ���
}
















void SPI1_Config(void)
{
	SPI1_GPIO_Config();
	SPI1_Struct_Config();

}




/*
 * SPI1��������
 * */
void SPI1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);

	/*
	 * ����AT45DB161 �����������     ����
	 * */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*
	 * ����AT45DB161 Ƭѡ����Ϊ�������ģʽ
	 * */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_8); //�Ȳ�ѡ�и�оƬ���ȵ��õ���ʱ����ѡ��оƬ��
}

/*
 * SPI1��������
 * */
void SPI1_Struct_Config(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_Cmd(SPI1, DISABLE); //ʧ�ܺ󷽿�����

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
 * SPI1��д������ʵ��SPIͨ��������ĺ����壡��
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



