#include "HX711.h"
#include "delay.h"
#include "tools.h"


u32 HX711_Buffer;
u32 Weight_Maopi;
s32 Weight_Shiwu = 0;

u8 Flag_Error = 0;

//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
#define GapValue 198


void HX711_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  //HX711_SCK
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_7);



    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //HX711_DOUT
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //GPIO_Mode_IPU;
    /*�� DOUT ������ MCU �ӿ�Ӧ����Ϊ����ڣ�
    ���Ҳ����κ����߻����͵��裬�Լ���
    MCU �� ADC ֮��ĵ������������ţ�*/

    GPIO_Init(GPIOB, &GPIO_InitStructure);

}

unsigned long ReadCount(void)
{
    unsigned long Count;
    unsigned char i = 0; //�ֲ������˴���Ҫ��ֵ ��Ϊ����������

    ADDO = 1;
    ADSK = 0; //ʹ��AD��PD_SCK �õͣ�
    Count = 0;
    while(ADDO);  //ADת��δ������ȴ�������ʼ��ȡ   �ȴ�ʱ���Լ��������Ϊ500ms����
//    {
//        Delay_ms(10);
//          if(i++>=60)
//          {
//           printf("�������ã�����֮"); //600ms��û׼����
//           break;
//          }
//      }
    Delay_us(5);
    for (i = 0; i < 24; i++)
    {
        ADSK = 1; //PD_SCK �øߣ��������壩
        Count = Count << 1; //�½�����ʱ����Count����һλ���Ҳಹ��
        Delay_us(5);
        ADSK = 0; //PD_SCK �õ�
        if(ADDO)
        {
            Count++;
            Delay_us(5);
        }
    }
    ADSK = 1;
    Count = Count ^ 0x800000; //��25�������½�����ʱ��ת������
    Delay_us(5);

    ADSK = 0;
    Delay_us(5);
    return(Count);
}

//****************************************************
//��ȡHX711
//****************************************************
//u32 HX711_Read(void)    //����128
//{
//    unsigned long count;
//    unsigned char i;
//    HX711_DOUT = 1;
//    Delay_us(1);
//    HX711_SCK = 0;
//    count = 0;
//    while(HX711_DOUT);
//    for(i = 0; i < 24; i++)
//    {
//        HX711_SCK = 1;
//        count = count << 1;
//        Delay_us(1);
//        HX711_SCK = 0;
//        if(HX711_DOUT)
//            count++;
//        Delay_us(1);
//    }
//    HX711_SCK = 1;
//    count = count ^ 0x800000; //��25�������½�����ʱ��ת������
//    Delay_us(1);
//    HX711_SCK = 0;
//    return(count);
//}

//****************************************************
//��ȡëƤ����
//****************************************************
void GetMaopi(void)
{
    Weight_Maopi = ReadCount();
}

//****************************************************
//����
//****************************************************
void GetWeight(void)
{
    HX711_Buffer = ReadCount();
  //  printf("HX711:%d  ", HX711_Buffer);
    if(HX711_Buffer > Weight_Maopi)
    {
        Weight_Shiwu = HX711_Buffer;
        Weight_Shiwu = Weight_Shiwu - Weight_Maopi;             //��ȡʵ���AD������ֵ��

        Weight_Shiwu = (s32)((float)Weight_Shiwu / GapValue);   //����ʵ���ʵ������
        //��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������GapValue���������
        //�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
        //������Գ���������ƫСʱ����С����ֵ��

     //   printf("����:%d  ", Weight_Shiwu);
    }else
    {
		 Weight_Shiwu=0;
		}
}

