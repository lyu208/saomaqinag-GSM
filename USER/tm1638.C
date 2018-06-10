/*******************(C) COPYRIGHT 冲霄科技有限公司**************************/

#include "tm1638.h"
#include "delay.h"



/************************************************
*                变量的定义                       *
*************************************************/
uchar           ucTm1638_2FreeFlag = 1; /* TM1638空闲标志 数码管写显示的时候 第二个tm1638芯片不能读取按键 根据这个变量进行互锁 此变量对外开放 */

uchar           ucKeyTouch = 0; /*触发按键编号  表示对应按了哪个按键(1~9)  此变量对外开放 供user.c 使用*/


uchar           ucKeyLock_1 = 0;  //按键上锁  全局变量默认为0
uchar           ucKeyLock_2 = 0;
uchar           ucKeyLock_3 = 0;
uchar           ucKeyLock_4 = 0;
uchar           ucKeyLock_5 = 0;
uchar           ucKeyLock_6 = 0;
uchar           ucKeyLock_7 = 0;
uchar           ucKeyLock_8 = 0;


uchar ucKeyPressCnt1 = 0; /*按键检测时间的计时变量*/
uchar ucKeyShortTuchFlag1 = 0;

uchar ucKeyPressCnt3 = 0;
uchar ucKeyShortTuchFlag3 = 0;

uchar ucKeyPressCnt4 = 0;
uchar ucKeyShortTuchFlag4 = 0;

uchar ucKeyPressCnt5 = 0;
uchar ucKeyShortTuchFlag5 = 0;

uchar ucKeyPressCnt6 = 0;
uchar ucKeySequenceFlag6 = 0;
uchar ucKeyShortTuchFlag6 = 0;

uchar  ucKeyPressCnt7 = 0;
uchar ucKeySequenceFlag7 = 0;
uchar ucKeyShortTuchFlag7 = 0;


/*0    1     2      3     4     5    6     7     8      9    空    -    c */
uchar  const  table[13] = { 0x3f, 0x06, 0x5B, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x00, 0x40, 0x58 };

/*芯片内部转换码。往芯片里面写的就是这写个数据 */
volatile uchar  dat1[16] = { 0 }; //第一片tm1638芯片数据
volatile uchar  dat2[16] = { 0 }; //第二片tm1638芯片数据








/************************************************
*              私有函数声明                       *
*************************************************/
uchar ReadByteFro_TM1638(void); //读取按键的，所以我只写了T2芯片的(按键检测是T2控制的！)
void  WriteByteTo_TM1638_SAME(uchar byte); //中断当中调用 读取按键用的

void WriteByteTo_TM1638(uchar select, uchar byte);
void WriteCmdTo_TM1638(uchar select, uchar cmd);


void delay(u32 nCount);






/************************************************
*函数名 ：     allotData

*函数参数 ： tm1638x :表示第几个tm1638芯片 ;  tubex :表示此芯片控制的第几个数码管 ;tubeData: 要显示的数据

*函数返回值 :   无

*函数功能描述 ：  转码函数 ——侯霄
高人有云：每个看似有规律的程序，都是可以简化的
    于是，我想把2511行（太TM不像话了）的程序简化一下！呵呵~~
    牺牲空间换取时间亦或牺牲时间换取空间 由你决定！——侯霄
*************************************************/
void allotData(uchar tm1638x, uchar tubex, uchar tubeData)
{
    uchar i;

    switch (tm1638x) //第几个tm1638芯片 此项目用了3片！
    {
    case 1:
        if (tubex <=
                8) //这个if else 判断语句 放在外面比放在for循环里好，效率应该高点，你觉得呢！！
        {
            for (i = 0; i < 8; i++)
            {
                if ((tubeData & 0x01) == 1) dat1[2 * i] |= (0x01 << (tubex - 1));
                else dat1[2 * i] &= ~(0x01 << (tubex - 1));
                tubeData >>= 1;
            }
        } else  //如果操作第九个 第十个数码管，就用到了这个部分
        {
            for (i = 0; i < 8; i++)
            {
                if ((tubeData & 0x01) == 1) dat1[2 * i + 1] |= (0x01 << (tubex - 9));
                else dat1[2 * i + 1] &= ~(0x01 << (tubex - 9));
                tubeData >>= 1;
            }
        }
        break;
    case 2:
        if (tubex <= 8)
        {
            for (i = 0; i < 8; i++)
            {
                if ((tubeData & 0x01) == 1) dat2[2 * i] |= (0x01 << (tubex - 1));
                else dat2[2 * i] &= ~(0x01 << (tubex - 1));
                tubeData >>= 1;
            }
        } else
        {
            for (i = 0; i < 8; i++)
            {
                if ((tubeData & 0x01) == 1) dat2[2 * i + 1] |= (0x01 << (tubex - 9));
                else dat2[2 * i + 1] &= ~(0x01 << (tubex - 9));
                tubeData >>= 1;
            }
        }
        break;
    }

}

/************************************************
*函数名 :       TM1638_Config

*函数参数 :      无

*函数返回值 :     无

*函数功能描述 :    配置两个tm1638芯片的引脚模式

*注意 :    
*************************************************/
void  TM1638_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //不明白为什么开漏不行
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//不明白为什么开漏不行
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9);
}



/************************************************
*函数名 ：TM1638_Init

*函数参数 ： 无

*函数返回值 :  无

*函数功能描述 ： 上电对显示寄存器 进行一次清零操作
*************************************************/
void TM1638_Init()       //看官方手册流程图写程序
{
    uchar i;

    WriteCmdTo_TM1638(1, AUTO_ADDR_MODE); //數據命令
    WriteCmdTo_TM1638(1, ADDR_0); //顯示地址
    for (i = 0; i < 16; i++)
    {
        WriteByteTo_TM1638(1, 0x00); //第一个tm1638控制的所有数码管，什么也不显示
    }
    WriteCmdTo_TM1638(1, LIGHT_LEVE_6); //亮度调节  以后就不用每次写数据都开了。。。
    STB1 = 1;



    WriteCmdTo_TM1638(2, AUTO_ADDR_MODE);
    WriteCmdTo_TM1638(2, ADDR_0);
    for (i = 0; i < 16; i++)
    {
        WriteByteTo_TM1638(2, 0x00); //第2个tm1638控制的所有数码管，什么也不显示
    }
    WriteCmdTo_TM1638(2, LIGHT_LEVE_6); //亮度调节   以后就不用每次写数据都开了。。。
    STB2 = 1;
}




/************************************************
*函数名 ：    ReadByteFro_TM1638

*函数参数 ：

*函数返回值 :

*函数功能描述 :   读取按键需要用到此函数
*************************************************/
uchar ReadByteFro_TM1638(void)                  //读数据函数
{
    unsigned char i;
    unsigned char temp = 0;
    DIO2 = 1; //拉高数据线
              delay(12);//新
    for (i = 0; i < 8; i++)
    {
        CLK2 = 0;
         delay(12);	 //新
        temp >>= 1;
        CLK2 = 1;
        if (DIO2_IN == 1)
        {
            temp |= 0x80;
        }
         delay(12);//Delay_us(1); 
    }
    return temp;
}


/************************************************
*函数名 ：     WriteByteTo_TM1638_SAME

*函数参数 ：

*函数返回值 :

*函数功能描述 ：  向TM1638的数据缓存区写入数据

*注意: 中断里面调用 中断里和主函数里不能同时调用同一个函数  所以我复制了一个相同的函数 
      供中断读取按键使用
*************************************************/
void WriteByteTo_TM1638_SAME(uchar byte) //
{
    uchar i;
    STB2 = 1;
    delay(12);//Delay_us(2);
    STB2 = 0;

    for (i = 0; i < 8; i++)
    {
        CLK2 = 0;
       delay(12);// Delay_us(1); //tSETUP  最小100ns
        if ((byte & 0x01) == 1)
        {
            DIO2 = 1;
        } else
        {
            DIO2 = 0;
        }
        CLK2 = 1;
        delay(12);//Delay_us(1); //tHOLD   最小100ns
        byte >>= 1;
    }
    delay(24);//Delay_us(2); //tCLKSTB    最小1us
}



/************************************************
*函数名 ：     WriteDatasToTm1638

*函数参数 ：

*函数返回值 :

*函数功能描述 ：  向TM1638的数据缓存区写入数据
*************************************************/
void WriteDatasTo_TM1638(uchar select)
{
    uchar i;
    switch (select)
    {
        /* 第1片tm1638芯片   控制10个数码管 */
    case 1:

        WriteCmdTo_TM1638(1, AUTO_ADDR_MODE);
        WriteCmdTo_TM1638(1, ADDR_0); //【显示地址】从0xc0地址处 开始自动加1
        for (i = 0; i < 16; i++)
        {
            WriteByteTo_TM1638(1, dat1[i]); //第一个tm1638控制的所有数码管，什么也不显示
        }
        WriteCmdTo_TM1638(1, LIGHT_LEVE_6); //亮度调节
        STB1 = 1;
        //	Delay2us();//new
        break;
        /* 第二片tm1638芯片   控制10个数码管 */
    case 2:

        ucTm1638_2FreeFlag = 0; /* 第二个数码管兼具扫描按键的功能 当写数码管的时候 牛仔很忙 没时间读取按键 */


        WriteCmdTo_TM1638(2, AUTO_ADDR_MODE); //设置数据命令
        WriteCmdTo_TM1638(2, ADDR_0); //【显示地址】从0xc0地址处 开始自动加1   设置显示地址
        for (i = 0; i < 16; i++)
        {
            WriteByteTo_TM1638(2, dat2[i]); //第一个tm1638控制的所有数码管，什么也不显示
        }
        WriteCmdTo_TM1638(2, LIGHT_LEVE_6); //亮度调节    显示控制指令
        STB2 = 1;



        ucTm1638_2FreeFlag = 1;  /*第二个数码管空闲了 可以进行读取按键 */
        break;

    default :
        break;
    }
}

/**
 *6000000  500ms  只是猜测 看网上的
 * 12000   1ms
 * 12      1us
 */
void delay(u32 nCount)
{
	for(;nCount!=0;nCount--);
}

/************************************************
*函数名 :          ReadKeyValue

*函数参数 :        无

*函数返回值 :       无

*函数功能描述 :    检测触发的是哪个按键

*注意  :    虽然按键一共有8个 但是根据不同的按键方式 长按短按 等 有多种不同的按键形式被触发
*************************************************/
void ReadKeyValue()
{
    uchar keyBuff[4], i;

    WriteByteTo_TM1638_SAME(
        READ_KEY_MODE); //主函数和中断里不能出现同时条用一个函数的情况
   delay(12);
 
    for (i = 0; i < 4; i++)
    {
        keyBuff[i] = ReadByteFro_TM1638(); //读取按键的4个BYTE
    }
    STB2 = 1;

    /**
    *   解析出哪个按键被按下? 配合TM1638数据手册 4页和13页?采用的是共阳方式?
    * 【浅浅的约定】:
    *  S1://手动自动
    *  S2://晨风功能
    *  S3://1放风开关
    *  S4://2放风开关
    *  S5://设置按键
    *  S6://加按键
    *  S7://减按键
    *  S8://确认
    * @author 侯霄 (2015年8月29日)
    */
    /*
    *        按键连接图
    *        SW1(K1-KS1)  SW2(K1-KS2)   SW3(K1-KS3)  SW4(K1-KS4)  SW5(K1-KS5)
    *        SW6(K1-KS6)  SW7(K1-KS7)   SW8(K1-KS8)
    */
    for (i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0:
            if (keyBuff[i] != 0x04)
            {
                ucKeyLock_1 = 0;
                ucKeyPressCnt1 = 0;
                if (ucKeyShortTuchFlag1 == 1)
                {
                    ucKeyShortTuchFlag1 = 0;
                    ucKeyTouch = 1;
                }
            } else if (ucKeyLock_1 == 0)
            {
                ucKeyPressCnt1++;
                ucKeyShortTuchFlag1 = 1;
                if (ucKeyPressCnt1 >= 200)
                {
                    ucKeyShortTuchFlag1 = 0;
                    ucKeyLock_1 = 1;
                    ucKeyPressCnt1 = 0;
                    ucKeyTouch = 12;   //多系统时 一键恢复到自动模式
                }
            }





            /************************************************/
            //放晨風功能
            if (keyBuff[i] == 0x40 &&
                ucKeyLock_2 == 0) //2号键被按下且是第一次被按下 KS2 & K1
            {
                ucKeyTouch = 2;
                ucKeyLock_2 = 1; //2号键上锁 防止一直触发

            } else if (keyBuff[i] != 0x40)
            {
                ucKeyLock_2 = 0; //2号键解锁
            }
            break;
        case 1:
            //3号按键?1號放風开
  
            if (keyBuff[i] != 0x04)
            {
                ucKeyLock_3 = 0;
                ucKeyPressCnt3 = 0;
                if (ucKeyShortTuchFlag3 == 1)
                {
                    ucKeyShortTuchFlag3 = 0;
                    ucKeyTouch = 3;
                }
            } else if (ucKeyLock_3 == 0)
            {
                ucKeyPressCnt3++;
                ucKeyShortTuchFlag3 = 1;
                if (ucKeyPressCnt3 >= 200)
                {
                    ucKeyShortTuchFlag3 = 0;
                    ucKeyLock_3 = 1;
                    ucKeyPressCnt3 = 0;
                    ucKeyTouch = 10;   //多系统时 一键打开分口
                }
            }



            /************************************************/


            //4号按键 1號放風关
            if (keyBuff[i] != 0x40)
            {
                ucKeyLock_4 = 0;
                ucKeyPressCnt4 = 0;
                if (ucKeyShortTuchFlag4 == 1)
                {
                    ucKeyShortTuchFlag4 = 0;
                    ucKeyTouch = 4;
                }
            } else if (ucKeyLock_4 == 0)
            {
                ucKeyPressCnt4++;
                ucKeyShortTuchFlag4 = 1;
                if (ucKeyPressCnt4 >= 200)
                {
                    ucKeyShortTuchFlag4 = 0;
                    ucKeyLock_4 = 1;
                    ucKeyPressCnt4 = 0;
                    ucKeyTouch = 11;   //多系统时 一键关闭分口
                }
            }


            break;
        case 2:
            // 5号按键  设置
            if (keyBuff[i] != 0x04)
            {
                ucKeyLock_5 = 0;
                ucKeyPressCnt5 = 0;
                if (ucKeyShortTuchFlag5 == 1)
                {
                    ucKeyShortTuchFlag5 = 0;
                    ucKeyTouch = 5;
                }
            } else if (ucKeyLock_5 == 0)
            {
                ucKeyPressCnt5++;
                ucKeyShortTuchFlag5 = 1;
                if (ucKeyPressCnt5 >= 200)
                {
                    ucKeyShortTuchFlag5 = 0;
                    ucKeyLock_5 = 1;
                    ucKeyPressCnt5 = 0;
                    ucKeyTouch = 9;   //后台设置
                }
            }



            /************************************************/

            // 6号按键  加   复用 2区风口开

            if (keyBuff[i] != 0x40)
            {
                ucKeyLock_6 = 0;
                ucKeyPressCnt6 = 0;
                ucKeySequenceFlag6 = 0;
                if (ucKeyShortTuchFlag6 == 1) //?????
                {
                    ucKeyShortTuchFlag6 = 0;
                    ucKeyTouch = 6;
                }
            } else if (keyBuff[i] == 0x40 && ucKeyLock_6 == 0)
            {
                ucKeyPressCnt6++;
                ucKeyShortTuchFlag6 = 1;

                if (ucKeyPressCnt6 > 220 || ucKeySequenceFlag6 == 1)
                {
                    ucKeySequenceFlag6 = 1;
                    ucKeyPressCnt6++;
                    if (ucKeyPressCnt6 > 15) //25
                    {
                        ucKeyPressCnt6 = 0;
                        ucKeyTouch = 6;
                    }
                }
            }

            break;
        case 3:
            //7号按键?减  复用 2区风口关

            if (keyBuff[i] != 0x04)
            {
                ucKeyLock_7 = 0;
                ucKeyPressCnt7 = 0;
                ucKeySequenceFlag7 = 0;
                if (ucKeyShortTuchFlag7 == 1) //?????
                {
                    ucKeyShortTuchFlag7 = 0;
                    ucKeyTouch = 7;
                }
            } else if (keyBuff[i] == 0x04 && ucKeyLock_7 == 0)
            {
                ucKeyPressCnt7++;
                ucKeyShortTuchFlag7 = 1;

                if (ucKeyPressCnt7 > 220 || ucKeySequenceFlag7 == 1)
                {
                    ucKeySequenceFlag7 = 1;
                    ucKeyPressCnt7++;
                    if (ucKeyPressCnt7 > 15) //25
                    {
                        ucKeyPressCnt7 = 0;
                        ucKeyTouch = 7;
                    }
                }
            }


            /************************************************/
            //8号键  确认

            if (keyBuff[i] == 0x40 &&
                ucKeyLock_8 == 0) //8号键被按下且是第一次被按下 KS8 & K1
            {
                ucKeyTouch = 8;
                ucKeyLock_8 = 1; //8号键上锁 防止一直触发
            } else if (keyBuff[i] != 0x40)   //没被按下
            {
                ucKeyLock_8 = 0; //8号键解锁
            }


            break;
        }
    }

}









/***************************************
       【以下为底层函数】
****************************************/


/************************************************
*函数名 ：    WriteByteTo_TM1638

*函数参数 ：  select: 第几片TM1638 控制芯片
          cmd:  发送的命令指令

*函数返回值 :   无

*函数功能描述 ：  实现一个字节的写入
*************************************************/
void WriteByteTo_TM1638(uchar select, uchar byte)
{
    uchar i;
    switch (select)
    {
    case 1:
        for (i = 0; i < 8; i++)
        {
            CLK1 = 0;
            Delay_us(1); //tSETUP  最小100ns
            if ((byte & 0x01) == 1)
            {
                DIO1 = 1;
            } else
            {
                DIO1 = 0;
            }

            CLK1 = 1;
            Delay_us(1); //tHOLD   最小100ns
            byte >>= 1;
        }

        break;
    case 2:
        for (i = 0; i < 8; i++)
        {
            CLK2 = 0;
            Delay_us(1); //tSETUP  最小100ns
            if ((byte & 0x01) == 1)
            {
                DIO2 = 1;
            } else
            {
                DIO2 = 0;
            }
            CLK2 = 1;
            Delay_us(1); //tHOLD   最小100ns
            byte >>= 1;
        }
        break;
    default :
        break;
    }
    Delay_us(2); //tCLKSTB    最小1us
}

/************************************************
*函数名 ：     WriteCmdTo_TM1638

*函数参数 ：    select: 第几片TM1638 控制芯片
          cmd:  发送的命令指令
*函数返回值 :   无

*函数功能描述 ： 向TM1638发送命令的函数   
*************************************************/
void WriteCmdTo_TM1638(uchar select, uchar cmd)
{
    switch (select)
    {
    case 1:
        STB1 = 1;
        Delay_us(2);
        STB1 = 0;
        // Delay1us();//new
        WriteByteTo_TM1638(1, cmd);

        break;
    case 2:
        STB2 = 1;
        Delay_us(2); //  PWSTB  最小1us
        STB2 = 0;
        // Delay1us();//new
        WriteByteTo_TM1638(2, cmd);

        break;
    default :
        break;

    }
}


/************************************************
*函数名 ：     WriteDataToAddr

*函数参数 ：

*函数返回值 :

*函数功能描述 ：  写一个字节数据到指定的地址
*************************************************/
//void WriteDataToAddr(uchar select, uchar addr, uchar dat) //指定地址写入数据
//{
//    switch (select)
//    {
//    case 1:
//        WriteCmdTo_TM1638(1, addr);
//        WriteByteTo_TM1638(1, dat);

//        break;
//    case 2:
//        WriteCmdTo_TM1638(2, addr);
//        WriteByteTo_TM1638(2, dat);

//        break;
//
//    }
//}


