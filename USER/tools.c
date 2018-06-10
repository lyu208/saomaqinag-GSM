/**
  ******************************************************************************
  * @file    tools.c 
  * @author  侯霄
  * @date    2016年5月5日
  * @brief   一些工具代码
  ******************************************************************************
  * @attention
  *    注意事项:
  *    
  * 
  *  
  *                COPYRIGHT 2016 冲霄集团物联网设备研发中心
  ******************************************************************************
  */

#include "tools.h"

#define ID_ADDR_ROM    0x1FFFF7E8  //产品唯一身份标识寄存器(96位) 基地址


/********************************************
*              CRC校验码表格                *
*********************************************/
const uint8_t  Gcu8_AuchCRCHi[] =   //全局不可修改类型无符号8位变量
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40
};

const uint8_t  Gcu8_AuchCRCLo[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
    0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
    0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
    0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
    0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
    0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
    0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
    0x40
};


/*************************************[UNIQUE-ID START]**************************************************/
/**
 * 
 * @brief  如果以32位的格式存储的话。则对应的3个地址如下 
 *  0x1FFFF7E8    0x1FFFF7EC    0x1FFFF7F0
 * @author 潇湘子(2016/5/4)
 * @param  
 * @note 
 以下的3种读取方式都可以。 我非常有必要提醒一下。输出查看ID的时候 一定要8位输出 不足8位前面补0  不然 你不知道是 
什么ID 比如 我以前没有补0的方式 输出是如下的 
66aff565355755087142549  因为起始为0 会不输出 所以我常规的以为 此ID为 如下
66aff565 35575508 7142549  最后一个不足8位 所以我以为前面可能是0  所以私自以为ID是  66aff565 35575508 07142549 
但是特错大错 当我以8位 不足补0的方式输出时 情况如下 
066aff565355755087142549  也就是ID为 066aff56 53557550 87142549 
是不是亮瞎双眼~~~  
 */
void  CheckId(uint32_t  *u32p_Unique_ID)
{
    uint32_t  ISN[3]; //96个位 共96/32=3个
    uint32_t  *cptr;
    uint8_t i = 0;
    cptr = (uint32_t *)ID_ADDR_ROM;

    /*   【第一种方法】   */
/* for (i = 0; i < 3;i++)                                                                         */
/*  {                                                                                             */
/*     ISN[i] = *cptr;                                                                            */
/*    printf("%08x",ISN[i]);//输出的16进制 占8位，不足8位在左侧补0       066aff565355755087142549 */
/*   cptr+=0x01;                                                                                  */
/* }                                                                                              */

    /*   【第二种方法】   */
/* for (i = 0; i < 3;i++)                                                                         */
/* {                                                                                              */
/*     ISN[i] = *(cptr+i);                                                                        */
/*                                                                                                */
/*     printf("%08x",ISN[i]);//输出的16进制 占8位，不足8位在左侧补0      066aff565355755087142549 */
/* }                                                                                              */

    /*     【第三种方法】    */
    for (i = 0; i < 3; i++)
    {
        ISN[i] = *cptr++;
        //为了查看 所以输出ID  在知道了此ID后 把ID 记下来 写如对应的UNIQUE_ID数组 屏蔽此输出函数 重新编译程序即可
        printf("%08x", ISN[i]); //输出的16进制 占8位，不足8位在左侧补0      066aff565355755087142549
    }


    for (i = 0; i < 3; i++)
    {
        if (ISN[i] != u32p_Unique_ID[i])
        {
            while (1);
        }
    }
}

/***********************************[UNIQUE-ID END]*********************************************/

/**
 * 
 * 
 * @author 潇湘子(2015/12/31)
 * 
 * @param ch 
 * @param f 
 * 
 * @return int 
 *  
 * 实现printf函数串口输出字符串必备底层函数
 */
int fputc(int ch, FILE *f) //复写此函数
{
    /*
    《STM32参考手册_V10.pdf》---第540页，清晰可见USART_SR 寄存器的默认值是0x00C0
     也就是说默认的TXE和TC的标志都是为1的！！！这样的话，如果不先进行清楚TC的标志，
     会发送一串字符的时候，导致第一个字符的丢失。
     另一个解决办法是：不用下面这个清除函数，但是需要每次发送字符前 加一个空格 就可以了 ^_^
    */
    USART_ClearFlag(USART2, USART_FLAG_TC); /* 清除SR寄存器中的 TC(发送完成)标志 */
    //感觉可以不用清楚标志位也是可以的，貌似一直觉得只要写入了USART_DR寄存器数据后，
    //该标志位自动就清零了。。。

//=======================================================================================
//USART_SendData(USART1, ch);
//这种写法感觉欠妥，因为此标志为1的时候，表示数据已经全部移位到移位寄存器里，并不代表数据
//就一定已经发送完成！！！
//while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//=======================================================================================

    USART_SendData(USART2, (int8_t)ch);
    //这种写法应该是比较沉重老练的写法，当此标志为0的时候，表示还没有发送完毕，死在这里。。
    //当此标志为1的时候，那么表示发送完成，条件不满足，于是跳出while循环！
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

    return ch;
}



/************************************************
*函数名 :	 CRC16

*函数参数 :

*函数返回值	:	  无

*函数功能描述 :  发送校验函数   采用格式为:  CRC-16/MODBUS       x16+x15+x2+1
*************************************************/
uint16_t  CRC16(uint8_t *updata, uint8_t len)
{
    uint16_t   u16_Index;
    uint8_t u8_CRCHi = 0xff;
    uint8_t u8_CRCLo = 0xff;
    while (len--)
    {
        u16_Index = u8_CRCHi ^ *updata++;
        u8_CRCHi = u8_CRCLo ^ Gcu8_AuchCRCHi[u16_Index];
        u8_CRCLo = Gcu8_AuchCRCLo[u16_Index];
    }
    return  (u8_CRCHi << 8 | u8_CRCLo);
}





/************************************************
*函数名 ：   NumberTo_BCD
*函数参数 ：     正常数值
*函数返回值 :    转换为的BCD码
*函数功能描述 ：  ——侯霄
*************************************************/
unsigned char  NumberTo_BCD(unsigned char dat)
{
    return ((dat / 10) << 4) + (dat % 10);
}




/************************************************
*函数名 ：   BCD_ToNumber
*函数参数 ：     BCD码
*函数返回值 :    正常数值
*函数功能描述 ： BCD码转正常数值，不过没有对BCD码做是否符合标准的检测 ——侯霄
*************************************************/
unsigned char  BCD_ToNumber(unsigned char dat)
{
    return (dat >> 4) * 10 + (dat & 0x0f);
}

/************************************************
*函数名 :       IWDG_Configuration

*函数参数 :      无

*函数返回值 :     无

*函数功能描述 :   独立看门狗的配置函数

*注意 :
*************************************************/
void IWDG_Configuration(void)
{
    //使能寄存器 写功能
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    //设置预分频 40K/64=0.625k 一个周期是 1.6ms
    IWDG_SetPrescaler(IWDG_Prescaler_64);
    //设置初值
    IWDG_SetReload(1800); //1800*1.6ms=2.88S
                          //喂狗
    IWDG_ReloadCounter();
    //使能独立看门狗
    IWDG_Enable();
}
/************************************************
*函数名 :       FeedDog

*函数参数 :      无

*函数返回值 :     无

*函数功能描述 :   喂狗函数

*注意 :
*************************************************/
void FeedDog(void)
{
    IWDG_ReloadCounter();
}

/************************************************
*函数名 :       EXTI_Cmd

*函数参数 :      EXTI_Line 要开启或屏蔽哪条线的外部中断 ; NewState 可选参数为EABLE DISABLE

*函数返回值 :     无

*函数功能描述 :   开启或关闭某个外部中断函数

*注意 : 因为库函数没有这个功能 根据手册自己写的
*************************************************/
void  EXTI_Cmd(uint32_t EXTI_Line, FunctionalState NewState)
{
    assert_param(IS_EXTI_LINE(EXTI_Line));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    EXTI->PR |= EXTI_Line; //当在外部中断线上发生了选择的边沿事件?该位被置’1’。在该位中写入’1’可以清除它
    if (NewState != DISABLE) //使能
    {
        EXTI->IMR |= EXTI_Line; //某一位置高 其他位不变
    } else //失能
    {
        EXTI->IMR &= ~EXTI_Line; //某一位置低 其他位不变
    }
}


/************************************************
*函数名 :       GetBetterNum

*函数参数 :      dat 要进行数值滤波处理的数组 ; length 此数组多少个数

*函数返回值 :     较可靠稳定的数值

*函数功能描述 :   对输入的数组，去掉最大值和一个最小值 然后取平均值

*注意 :  
*************************************************/
uint32_t  GetBetterNum(uint32_t * dat,const char length)
{
	unsigned char i=0;
	 uint32_t MinNum=0,MaxNum=0;
	 uint32_t  sum =0;
 
	MinNum =dat[0];
	MaxNum =dat[0];
	for(i=0;i<length;i++)
	{
       if(MinNum>dat[i]) MinNum=dat[i];
       if(MaxNum<dat[i]) MaxNum=dat[i];
       sum=sum+dat[i];
	}
//	printf("最小值%d\n",MinNum); 
//	printf("最大值%d\n",MaxNum);  
//	printf("累加和%d\n",sum);
	
	
	return (sum-MinNum-MaxNum)/(length-2);
}

/************************************************
*函数名 :       SystemReset

*函数参数 :       

*函数返回值 :     

*函数功能描述 :   软复位

*注意 :  
*************************************************/
void  SystemReset()
{
//	NVIC_SETFAULTMASK();
//	GenerateSystemReset();
	__set_FAULTMASK(1);
	NVIC_SystemReset();// 复位
	
}

/************************************************
*函数名 :       NumLength

*函数参数 :       

*函数返回值 :     

*函数功能描述 :   计算数值的数字个数

*注意 :  
*************************************************/
uchar NumLength(uint32_t num)
{
    uchar i = 0;
    if(num < 10)
    {
        i = 1;
    }
    else if(num >= 10)
    {
        while(num != 0)
        {
            num /= 10;
            i++;
        }
    }
    return i;
}


