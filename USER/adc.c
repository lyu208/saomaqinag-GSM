
/************************************************** 
 重要说明: 单次的ADC毫无意义~ 一般都要滤波 简单点的就是平均值 比如16次
 取平均
 
 一般思路是搞一个数组连续DMA采样16次 然后16次后触发DMA发成中断
 进入中断函数进行一个简单的均值滤波
 
 把ADC的采样周期设置的越长 越精确越稳定 
 ****************************************************/

#include "adc.h"
#include "tools.h"
#include "delay.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)


volatile  uint16_t  u16AdcValue = 0;

 uint16_t  u16AD_Value[2]; //DMA转化的内存地址


void  ADC_DMA_Config(void);



/**
 * 
 * 
 * @author 潇湘子(2016/1/4)
 * @param 
 *  
 * 真正的ADC一定要放在DMA或者中断当中 
 */
void ADC_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;




    /*端口初始化*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //看参考手册
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
    //输入模式是没有速度可言的  只有输出才有速度

    GPIO_Init(GPIOC, &GPIO_InitStructure);



    /*ADC配置*/
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //开启连续转换模式 即不停地进行ADC转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 采集数据 右对齐
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //不使用外部触发转换  用软件触发方式
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //独立ADC模式
    ADC_InitStructure.ADC_NbrOfChannel = 2; // 要转换的通道数为1 (如果要扫描多路AD 比如4路 则 此数为4)
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //禁止扫描模式  扫描模式用于多通道采集


    ADC_Init(ADC1, &ADC_InitStructure);

    RCC_ADCCLKConfig(RCC_PCLK2_Div8); //时钟为72/8=9mhz  低于14mhz（ ADC 的时钟频率最高为 14MHz）
    /*配置ADC1的通道14为55.	5个采样周期?序列为1 
     55.5+12.5/9=7.5 us
      */
			
		//通道组 第11个通道的采样顺序为1 采样时间为55.5周期
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_55Cycles5);




    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /*复位校准寄存器 */
    ADC_ResetCalibration(ADC1);
    /*等待校准寄存器复位完成 */
    while (ADC_GetResetCalibrationStatus(ADC1));
    /* ADC校准 */
    ADC_StartCalibration(ADC1);
    /* 等待校准完成*/
    while (ADC_GetCalibrationStatus(ADC1));





    /*****************************
        ADC的DMA方式配置
        如果不需要DMA方式 则注释掉即可
    ******************************/
 
  /*ADC配置为DMA的方式*/    
    ADC_DMA_Config();





    /* 由于没有采用外部触发  所以使用软件触发ADC转换 */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ADC开始转化

}

/**
 * 
 * 
 * @author 潇湘子(2016/1/7)
 * @param 
 *  
 * ADC的DMA方式配置 
 */
void  ADC_DMA_Config(void)
{
    DMA_InitTypeDef  DMA_InitStructure;
//    NVIC_InitTypeDef  NVIC_InitStructure;


    DMA_DeInit(DMA1_Channel1);


    /*【是否是内存到内存传输】 可设置为 DMA_M2M_Disable 或  DMA_M2M_Enable*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    /*【数据交换方向】 从外设到内存还是从内存到外设 可设置为 DMA_DIR_PeripheralDST(内存到外设寄存器) 或 DMA_DIR_PeripheralSRC(外设寄存器到内存)*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    /*【一次传输数据量的大小】*/
    DMA_InitStructure.DMA_BufferSize = 2;//15;



    /*【外设地址】*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; //ADC1_DR_Address //ADC1 数据寄存器
    /*【内存地址】  */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&u16AD_Value;//(uint32_t)&u16AD_Value;


    /*数据宽度: Byte 8位  HalfWord 16位  Word 32位*/

    /*【外设数据宽度】 可设置为 DMA_PeripheralDataSize_Byte 或 DMA_PeripheralDataSize_HalfWord 
      或 DMA_PeripheralDataSize_Word*/
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    /*【内存数据宽度】 可设置为 DMA_MemoryDataSize_Byte 或 DMA_MemoryDataSize_HalfWord 
      或 DMA_MemoryDataSize_Word*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;


    /*【外设地址是否自增】 可设置为DMA_PeripheralInc_Disable 或 DMA_PeripheralInc_Enable*/
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /*【内存地址是否自增】 可设置为DMA_MemoryInc_Disable 或 DMA_MemoryInc_Enable*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;



    /*【通道优先级:低、中、高、超高】 可设置DMA_Priority_VeryHigh 或DMA_Priority_High 或 
      DMA_Priority_Medium 或 DMA_Priority_Low*/
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //只有一个DMA通道时 任何优先级都无所谓
    /*【模式:是循环采集还是单次采集不循环】 可设置DMA_Mode_Circular 或 DMA_Mode_Normal*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;


    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
 
 
    /*****************************
        ADC的DMA是否使用完成中断
      如果不需要中断方式?则注释即可  
    ******************************/

//    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); //使能DMA传输完成中断

//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn; //关于中断Channel 看 stm32f10x.h【167】行
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
 * @author 潇湘子(2016/1/4)
 * @param  
 */
uint16_t Get_ADC1_Value(void)
{
    uint16_t temp = 0;

    temp = ADC_GetConversionValue(ADC1);
    temp = temp & 0x0FFF; //右对齐 取低12位

    return temp;
}
void ADC_ValueFilter(void)
{
    uint16_t u16TempSum = 0; //65535/4095=16.0   是否用16位得慎重考虑
    uint8_t  i = 0;

    for (i = 0; i < 15; i++)
    {
        u16TempSum += u16AD_Value[i];
    }

    u16AdcValue = u16TempSum / 15;


}


