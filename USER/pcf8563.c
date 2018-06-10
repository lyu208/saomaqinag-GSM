#include "PCF8563.h"
#include "tools.h"
 
 
 
#define   SLAVE_READ_ADDRESS    0xA3  //从机读地址
#define   SLAVE_WRITE_ADDRESS    0xA2  //从机写地址



uchar Gu8_Year = 0, Gu8_Month = 0, Gu8_Day = 0;
uchar Gu8_Week = 0;
uchar Gu8_Hour = 0, Gu8_Minute = 0, Gu8_Second =0;





void  PCF8563_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //SDA
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //SCL
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出

    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9); //设置高电平
}
/************************************************
*函数名 :       PCF8563_Init

*函数参数 :       

*函数返回值 :     无

*函数功能描述 :    上电初始化，读取数据如果错误，则重写一个默认值

*注意 :
*************************************************/
void PCF8563_Init(void)
{
    ReadTimesFro_PCF8563();

    if(Gu8_Second >= 60 || Gu8_Minute >= 60 || Gu8_Hour >= 24)
    {
			  //2016年5月14日 09:30:01 星期六
        Gu8_Second = 1;
        Gu8_Minute = 30;
        Gu8_Hour = 9;
        Gu8_Day = 14;
        Gu8_Week = 6;
        Gu8_Month = 5;
        Gu8_Year = 16;

        //  PCF8563_CLKOUT_1s();
        //   PCF8563_WriteAdress(0x00, 0x20); //禁止RTC source clock

        //初始化PCF8563的时间
        WriteDataTo_PCF8563(0x02, NumberTo_BCD(Gu8_Second));
        WriteDataTo_PCF8563(0x03, NumberTo_BCD(Gu8_Minute));
        WriteDataTo_PCF8563(0x04, NumberTo_BCD(Gu8_Hour));
        WriteDataTo_PCF8563(0x05, NumberTo_BCD(Gu8_Day));
        WriteDataTo_PCF8563(0x06, Gu8_Week);//数据手册Week 不用BCD转换
        WriteDataTo_PCF8563(0x07, NumberTo_BCD(Gu8_Month));
        WriteDataTo_PCF8563(0x08, NumberTo_BCD(Gu8_Year));
			
      //  WriteDataTo_PCF8563(0x00, 0x00); //Enable RTC sorce clock
    }
}
u8 PCF8563_ReaDAdress(u8 Adress)
{
    u8 ReadData;
    IIC_Start();
    IIC_WriteByte(SLAVE_WRITE_ADDRESS);
    IIC_WaitAck();
    IIC_WriteByte(Adress);
    IIC_WaitAck();
    IIC_Start();
    IIC_WriteByte(SLAVE_READ_ADDRESS);
    IIC_WaitAck();
    ReadData = IIC_ReadByte();
    IIC_Stop();
    return ReadData;
}

/************************************************
*函数名 :       WriteDataTo_PCF8563

*函数参数 :      寄存器地址 ; 待写入的数据

*函数返回值 :     无

*函数功能描述 :    向pcf8563 某个寄存器写入数据

*注意 :
*************************************************/

void WriteDataTo_PCF8563(u8 Adress, u8 DataTX) 
{
    IIC_Start();
    IIC_WriteByte(SLAVE_WRITE_ADDRESS); //PCF8563的 硬件写地址
    IIC_WaitAck();
    IIC_WriteByte(Adress); //PCF8563 寄存器地址
    IIC_WaitAck();
    IIC_WriteByte(DataTX);
    IIC_WaitAck();
    IIC_Stop();
}
 
/************************************************
*函数名 :       ReadTimesFro_PCF8563

*函数参数 :      无

*函数返回值 :     无

*函数功能描述 :   取回八个字节的数据:秒，分，时，天，星期，月份，年份 

*注意 :  ——数据手册：P14
1. Send a START condition and the slave address for write (A2h).
2. Set the address pointer to 2 (VL_seconds) by sending 02h.
3. Send a RESTART condition or STOP followed by START.
4. Send the slave address for read (A3h).
5. Read VL_seconds.
6. Read Minutes.
7. Read Hours.
8. Read Days.
9. Read Weekdays.
10. Read Century_months.
11. Read Years.
12. Send a STOP condition.
*************************************************/
void ReadTimesFro_PCF8563(void)
{
	       //                     秒，分，时，天，星，月，年
		uint8_t  PCF8563_Time[7] = { 50, 59, 23, 31, 6, 12, 12 };


    IIC_Start();
    IIC_WriteByte(SLAVE_WRITE_ADDRESS);
    IIC_WaitAck();
    IIC_WriteByte(0x02);//Set the address pointer to 2 (VL_seconds) by sending 02h.
    IIC_WaitAck();
    IIC_Start();
    IIC_WriteByte(SLAVE_READ_ADDRESS);
    IIC_WaitAck();
    PCF8563_Time[0] = IIC_ReadByte() & 0x7f;
    I2C_Ack();
    PCF8563_Time[1] = IIC_ReadByte() & 0x7f;
    I2C_Ack();
    PCF8563_Time[2] = IIC_ReadByte() & 0x3f;
//    I2C_Ack();
//    PCF8563_Time[3] = IIC_ReadByte() & 0x3f;
//    I2C_Ack();
//    PCF8563_Time[4] = IIC_ReadByte() & 0x07;
//    I2C_Ack();
//    PCF8563_Time[5] = IIC_ReadByte() & 0x1f;
//    I2C_Ack();
//    PCF8563_Time[6] = IIC_ReadByte();
    I2C_NoAck();
    IIC_Stop();

    Gu8_Second = BCD_ToNumber(PCF8563_Time[0]);
    Gu8_Minute = BCD_ToNumber(PCF8563_Time[1]);
    Gu8_Hour = BCD_ToNumber(PCF8563_Time[2]);
//    Gu8_Day = BCD_ToNumber(PCF8563_Time[3]);
//    Gu8_Week = BCD_ToNumber(PCF8563_Time[4]);//0~6 0表示星期日
//    Gu8_Month = BCD_ToNumber(PCF8563_Time[5]);
//    Gu8_Year = BCD_ToNumber(PCF8563_Time[6]);

}


void WriteTimesTo_PCF8563(void)
{
      WriteDataTo_PCF8563(0x03, NumberTo_BCD(Gu8_Minute));
      WriteDataTo_PCF8563(0x04, NumberTo_BCD(Gu8_Hour));
}
//在CLKOUT上定时1S输出一个下降沿脉冲，经过验证，可以设置STM32的GPIO上拉输入，设置成下降沿中断，单片机每过1S产生一次中断
void PCF8563_CLKOUT_1s(void)
{
    WriteDataTo_PCF8563(0x01, 0); //禁止定时器输出，闹铃输出
    WriteDataTo_PCF8563(0x0e, 0); //关闭定时器等等
// PCF8563_WriteAdress(0x0e, 0); //写入1
    WriteDataTo_PCF8563(0x0d, 0x83); //打开输出脉冲
}
