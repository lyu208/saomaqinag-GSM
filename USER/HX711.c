#include "HX711.h"
#include "delay.h"
#include "tools.h"


u32 HX711_Buffer;
u32 Weight_Maopi;
s32 Weight_Shiwu = 0;

u8 Flag_Error = 0;

//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数
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
    /*与 DOUT 相连的 MCU 接口应设置为输入口，
    并且不接任何拉高或拉低电阻，以减少
    MCU 与 ADC 之间的电流交换（干扰）*/

    GPIO_Init(GPIOB, &GPIO_InitStructure);

}

unsigned long ReadCount(void)
{
    unsigned long Count;
    unsigned char i = 0; //局部变量此处需要赋值 因为下文有他用

    ADDO = 1;
    ADSK = 0; //使能AD（PD_SCK 置低）
    Count = 0;
    while(ADDO);  //AD转换未结束则等待，否则开始读取   等待时间大约测试以下为500ms左右
//    {
//        Delay_ms(10);
//          if(i++>=60)
//          {
//           printf("贱货已烂，请弃之"); //600ms都没准备好
//           break;
//          }
//      }
    Delay_us(5);
    for (i = 0; i < 24; i++)
    {
        ADSK = 1; //PD_SCK 置高（发送脉冲）
        Count = Count << 1; //下降沿来时变量Count左移一位，右侧补零
        Delay_us(5);
        ADSK = 0; //PD_SCK 置低
        if(ADDO)
        {
            Count++;
            Delay_us(5);
        }
    }
    ADSK = 1;
    Count = Count ^ 0x800000; //第25个脉冲下降沿来时，转换数据
    Delay_us(5);

    ADSK = 0;
    Delay_us(5);
    return(Count);
}

//****************************************************
//读取HX711
//****************************************************
//u32 HX711_Read(void)    //增益128
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
//    count = count ^ 0x800000; //第25个脉冲下降沿来时，转换数据
//    Delay_us(1);
//    HX711_SCK = 0;
//    return(count);
//}

//****************************************************
//获取毛皮重量
//****************************************************
void GetMaopi(void)
{
    Weight_Maopi = ReadCount();
}

//****************************************************
//称重
//****************************************************
void GetWeight(void)
{
    HX711_Buffer = ReadCount();
  //  printf("HX711:%d  ", HX711_Buffer);
    if(HX711_Buffer > Weight_Maopi)
    {
        Weight_Shiwu = HX711_Buffer;
        Weight_Shiwu = Weight_Shiwu - Weight_Maopi;             //获取实物的AD采样数值。

        Weight_Shiwu = (s32)((float)Weight_Shiwu / GapValue);   //计算实物的实际重量
        //因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正这里的GapValue这个除数。
        //当发现测试出来的重量偏大时，增加该数值。
        //如果测试出来的重量偏小时，减小改数值。

     //   printf("重量:%d  ", Weight_Shiwu);
    }else
    {
		 Weight_Shiwu=0;
		}
}

