
/************************************************** 
 ��Ҫ˵��: ���ε�ADC��������~ һ�㶼Ҫ�˲� �򵥵�ľ���ƽ��ֵ ����16��
 ȡƽ��
 
 һ��˼·�Ǹ�һ����������DMA����16�� Ȼ��16�κ󴥷�DMA�����ж�
 �����жϺ�������һ���򵥵ľ�ֵ�˲�
 
 ��ADC�Ĳ����������õ�Խ�� Խ��ȷԽ�ȶ� 
 ****************************************************/

#include "adc.h"
#include "tools.h"
#include "delay.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)


volatile  uint16_t  u16AdcValue = 0;

 uint16_t  u16AD_Value[2]; //DMAת�����ڴ��ַ


void  ADC_DMA_Config(void);



/**
 * 
 * 
 * @author ������(2016/1/4)
 * @param 
 *  
 * ������ADCһ��Ҫ����DMA�����жϵ��� 
 */
void ADC_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;




    /*�˿ڳ�ʼ��*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //���ο��ֲ�
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
    //����ģʽ��û���ٶȿ��Ե�  ֻ����������ٶ�

    GPIO_Init(GPIOC, &GPIO_InitStructure);



    /*ADC����*/
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //��������ת��ģʽ ����ͣ�ؽ���ADCת��
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // �ɼ����� �Ҷ���
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //��ʹ���ⲿ����ת��  �����������ʽ
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //����ADCģʽ
    ADC_InitStructure.ADC_NbrOfChannel = 2; // Ҫת����ͨ����Ϊ1 (���Ҫɨ���·AD ����4· �� ����Ϊ4)
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //��ֹɨ��ģʽ  ɨ��ģʽ���ڶ�ͨ���ɼ�


    ADC_Init(ADC1, &ADC_InitStructure);

    RCC_ADCCLKConfig(RCC_PCLK2_Div8); //ʱ��Ϊ72/8=9mhz  ����14mhz�� ADC ��ʱ��Ƶ�����Ϊ 14MHz��
    /*����ADC1��ͨ��14Ϊ55.	5����������?����Ϊ1 
     55.5+12.5/9=7.5 us
      */
			
		//ͨ���� ��11��ͨ���Ĳ���˳��Ϊ1 ����ʱ��Ϊ55.5����
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_55Cycles5);




    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /*��λУ׼�Ĵ��� */
    ADC_ResetCalibration(ADC1);
    /*�ȴ�У׼�Ĵ�����λ��� */
    while (ADC_GetResetCalibrationStatus(ADC1));
    /* ADCУ׼ */
    ADC_StartCalibration(ADC1);
    /* �ȴ�У׼���*/
    while (ADC_GetCalibrationStatus(ADC1));





    /*****************************
        ADC��DMA��ʽ����
        �������ҪDMA��ʽ ��ע�͵�����
    ******************************/
 
  /*ADC����ΪDMA�ķ�ʽ*/    
    ADC_DMA_Config();





    /* ����û�в����ⲿ����  ����ʹ���������ADCת�� */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ADC��ʼת��

}

/**
 * 
 * 
 * @author ������(2016/1/7)
 * @param 
 *  
 * ADC��DMA��ʽ���� 
 */
void  ADC_DMA_Config(void)
{
    DMA_InitTypeDef  DMA_InitStructure;
//    NVIC_InitTypeDef  NVIC_InitStructure;


    DMA_DeInit(DMA1_Channel1);


    /*���Ƿ����ڴ浽�ڴ洫�䡿 ������Ϊ DMA_M2M_Disable ��  DMA_M2M_Enable*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    /*�����ݽ������� �����赽�ڴ滹�Ǵ��ڴ浽���� ������Ϊ DMA_DIR_PeripheralDST(�ڴ浽����Ĵ���) �� DMA_DIR_PeripheralSRC(����Ĵ������ڴ�)*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    /*��һ�δ����������Ĵ�С��*/
    DMA_InitStructure.DMA_BufferSize = 2;//15;



    /*�������ַ��*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; //ADC1_DR_Address //ADC1 ���ݼĴ���
    /*���ڴ��ַ��  */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&u16AD_Value;//(uint32_t)&u16AD_Value;


    /*���ݿ��: Byte 8λ  HalfWord 16λ  Word 32λ*/

    /*���������ݿ�ȡ� ������Ϊ DMA_PeripheralDataSize_Byte �� DMA_PeripheralDataSize_HalfWord 
      �� DMA_PeripheralDataSize_Word*/
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    /*���ڴ����ݿ�ȡ� ������Ϊ DMA_MemoryDataSize_Byte �� DMA_MemoryDataSize_HalfWord 
      �� DMA_MemoryDataSize_Word*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;


    /*�������ַ�Ƿ������� ������ΪDMA_PeripheralInc_Disable �� DMA_PeripheralInc_Enable*/
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /*���ڴ��ַ�Ƿ������� ������ΪDMA_MemoryInc_Disable �� DMA_MemoryInc_Enable*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;



    /*��ͨ�����ȼ�:�͡��С��ߡ����ߡ� ������DMA_Priority_VeryHigh ��DMA_Priority_High �� 
      DMA_Priority_Medium �� DMA_Priority_Low*/
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //ֻ��һ��DMAͨ��ʱ �κ����ȼ�������ν
    /*��ģʽ:��ѭ���ɼ����ǵ��βɼ���ѭ���� ������DMA_Mode_Circular �� DMA_Mode_Normal*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;


    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
 
 
    /*****************************
        ADC��DMA�Ƿ�ʹ������ж�
      �������Ҫ�жϷ�ʽ?��ע�ͼ���  
    ******************************/

//    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); //ʹ��DMA��������ж�

//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn; //�����ж�Channel �� stm32f10x.h��167����
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;//1
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;//0
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);



    ADC_DMACmd(ADC1, ENABLE);
 
    DMA_Cmd(DMA1_Channel1, ENABLE);
}


/**
 * 
 * 
 * @author ������(2016/1/4)
 * @param  
 */
uint16_t Get_ADC1_Value(void)
{
    uint16_t temp = 0;

    temp = ADC_GetConversionValue(ADC1);
    temp = temp & 0x0FFF; //�Ҷ��� ȡ��12λ

    return temp;
}
void ADC_ValueFilter(void)
{
    uint16_t u16TempSum = 0; //65535/4095=16.0   �Ƿ���16λ�����ؿ���
    uint8_t  i = 0;

    for (i = 0; i < 15; i++)
    {
        u16TempSum += u16AD_Value[i];
    }

    u16AdcValue = u16TempSum / 15;


}


