#include "IIC.h"
#include "delay.h"

void IIC_Start(void)
{ 
    SDA=1;
    SCL=1;
    Delay_us(1);
    SDA=0;
    Delay_us(1);
    SCL=0;
    Delay_us(2);
}
void IIC_Stop(void)
{
    SCL=0;  
    SDA=0; 
	  Delay_us(1);
    SCL=1;
    Delay_us(1);
    SDA=1;
    Delay_us(2);
}

void IIC_WriteByte(u8 byte)
{
     u8 i = 0;
//    for(i = 0; i < 8; i++)
//    {
//        SCL=0;
//        Delay_us(1);
//        if(byte & 0x80)
//        {
//            SDA=1;
//        }
//        else
//        {
//            SDA=0;
//        }
//        Delay_us(1);
//        SCL=1;
//        Delay_us(1);
//        byte <<= 1;
//    }
//    SCL=0;
//    Delay_us(2);
		
	  for(i = 0; i < 8; i++)
    {       
        Delay_us(1);
        if(byte & 0x80)
        {
            SDA=1;
        }
        else
        {
            SDA=0;
        }
				byte <<= 1;
				
        Delay_us(1);
        SCL=1;
        Delay_us(1);
				SCL=0;
    }  
    Delay_us(2);
		
		
//	i = 8;
//	while(i--)
//	{
//		Delay_us(1); 
//		if(byte & 0x80)	SDA = 1;
//		else			SDA	= 0;
//		byte <<= 1;
//		
//		Delay_us(1);
//		
//		SCL = 1;
//		Delay_us(1);
//		SCL = 0;    
//	}
//	Delay_us(2);
}
u8 IIC_ReadByte(void)
{
    u8 i, ReadByte;
    SDA=1;
	  Delay_us(1);
    for(i = 0; i < 8; i++)
    {
        ReadByte <<= 1;
        SCL=0;
        Delay_us(2);
        SCL=1;
        Delay_us(1);
        if(SDA_IN)
        {
            ReadByte |= 0x01;
        }
        else
        {
            ReadByte &= ~(0x01);
        }
    }
    return ReadByte;
}
void I2C_Ack(void)
{
    SCL=0;
    Delay_us(1);
    SDA=0;
    Delay_us(1);
    SCL=1;
    Delay_us(1);


    SCL=0;
    Delay_us(2);
}
void I2C_NoAck(void)
{
    SCL=0;
    Delay_us(1);
    SDA=1;
    Delay_us(1);
    SCL=1;
    Delay_us(1);
    SCL=0;
    Delay_us(1);
}
void IIC_WaitAck(void)
{
    u16 k;
    SCL=0;
    SDA=1;
    Delay_us(1);
    SCL=1;
    k = 0;
    while((SDA_IN != 0) && (k < 60000))k++;
    Delay_us(1);
    SCL=0;
    Delay_us(1);
}
///********************************************
//内部函数，输出ACK ,每个字节传输完成，输出ack=0,结束读书据，ack=1;
//********************************************/
//void WriteACK(uchar ack)
//{
//		SDA=ack;
//   		Delay();
//   		SCL=1;
//   		Delay();
//   		SCL=0;
//}
///********************************************
//内部函数，等待ACK
//********************************************/
//void WaitACK()
//{  
//		uchar errtime=20;
//   		SDA=1;
//   		Delay(); /*读ACK*/
//   		SCL=1;
//   		Delay();
//   		while(SDA)
//   		{  
//				errtime--;
//      			if(!errtime) 
//						Stop();
//   		}
//   		SCL=0;
//   		Delay();
//}
