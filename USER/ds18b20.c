#include "ds18b20.h"
#include "delay.h"
 

/********************************************
*              变量定义                     *
*********************************************/

uchar   romEncode[DS18B20_ROM_LENGTH];  //18b20  64位8個字節的ROM編碼數組





void  DS18B20_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化后是低电平 
	
   // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	  GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化后是低电平 
	
	  GPIO_SetBits(GPIOA,GPIO_Pin_3);
	  GPIO_SetBits(GPIOC,GPIO_Pin_3);
}
 
/************************************************
*【函数名】 ：	DS18B20_Reset

*【函数参数】 ：  無

*【函数返回值】	:	   成功返回true 失敗返回false

*【函数功能描述】 ：   18b20的復位函數
*************************************************/
BOOL DS18B20_Reset(uchar select)
{
    uchar i;

    switch (select)
    {
    case 1:
        DQ_1 = 0;                     //送出低电平复位信号
        Delay_us(480);              //延时至少480us
        Delay_us(60);
        DQ_1 = 1;                     //释放数据线
        Delay_us(60);               //等待60us
        i = DQ_1_IN;                    //检测存在脉冲
        Delay_us(420);              //等待设备释放数据线
        Delay_us(60);
        break;
    case 2:
        DQ_2 = 0;                     //送出低电平复位信号
        Delay_us(480);              //延时至少480us
        Delay_us(60);
        DQ_2 = 1;                     //释放数据线
        Delay_us(60);               //等待60us
        i = DQ_2_IN;                    //检测存在脉冲
        Delay_us(420);              //等待设备释放数据线
        Delay_us(60);
        break;
    }


    if (i == 0)
    {
        return TRUE;
    } else
    {
        return FALSE;
    }
}

/************************************************
*【函数名】 ：	WriteByteTo_DS18B20

*【函数参数】 ： 要發送的字節值

*【函数返回值】	:	無

*【函数功能描述】 ： 向18b20寫一個字節的函數
*************************************************/
void WriteByteTo_DS18B20(uchar dat, uchar select)
{
    uchar i;
    switch (select)
    {
    case 1:
        for (i = 0; i < 8; i++)             //8位计数器
        {
            DQ_1 = 0;
            Delay_us(2);
            DQ_1 = dat & 0x01; //取最低位賦給DQ引腳
            Delay_us(60);               //等待时间片结束
            Delay_us(20);
            DQ_1 = 1;
            Delay_us(1); // 按時序圖分析，這是Trec 亦即恢復時間。
            Delay_us(1);
            dat >>= 1;
        }
        break;
    case 2:
        for (i = 0; i < 8; i++)             //8位计数器
        {
            DQ_2 = 0;
            Delay_us(2);
            DQ_2 = dat & 0x01; //取最低位賦給DQ引腳
            Delay_us(60);               //等待时间片结束
            Delay_us(20);
            DQ_2 = 1;
            Delay_us(1); // 按時序圖分析，這是Trec 亦即恢復時間。
            Delay_us(1);
            dat >>= 1;
        }
        break;
    }


}

/************************************************
*函数名 ：	   ReadByteFr_DS18B20

*函数参数 ：	無

*函数返回值	:	返回的字節

*函数功能描述 ： 從18b20讀取一個字節
*************************************************/
uchar ReadByteFr_DS18B20(uchar select)
{
    uchar dat = 0;
    uchar i;
    switch (select)
    {
    case 1:
        for (i = 0; i < 8; i++)             //8位计数器
        {
            dat >>= 1; //最高位填0
            DQ_1 = 0;
            Delay_us(2);
            DQ_1 = 1; //拉高DQ 看官方手冊英文資料
            Delay_us(4); //尽量接近临近15us的地方
            if (DQ_1_IN)
            {
                dat |= 0x80; //想置1 其他不變
            }
            Delay_us(60);
        }
        break;
    case 2:
        for (i = 0; i < 8; i++)             //8位计数器
        {
            dat >>= 1; //最高位填0
            DQ_2 = 0;
            Delay_us(2);
            DQ_2 = 1; //拉高DQ 看官方手冊英文資料
            Delay_us(4); //尽量接近临近15us的地方
            if (DQ_2_IN)
            {
                dat |= 0x80; //想置1 其他不變
            }
            Delay_us(60);
        }
        break;
    }
    return dat;
}


/************************************************
*【函数名】 ：	 Read_DS18B20_ROM

*【函数参数】 ：	無

*【函数返回值】	:	無

*【函数功能描述】 ：总线控制器读取64位ROM，只有当总线上只存在一个
DS18B20的时候，才可以使用此命令，如果挂载不止一个
当通信时将会发生数据冲突。
*************************************************/
//void Read_DS18B20_ROM()
//{
//    uchar i=0;
//    BOOL DS18B20_Reset();
//    WriteByteTo_DS18B20(0x33);
//    for ( i=0; i<8; i++ )
//    {
//        romEncode[i]=ReadByteFr_DS18B20();
//    }
//}



 
 
 
