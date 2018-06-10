#include "ds18b20.h"
#include "delay.h"
 

/********************************************
*              ��������                     *
*********************************************/

uchar   romEncode[DS18B20_ROM_LENGTH];  //18b20  64λ8���ֹ���ROM���a���M





void  DS18B20_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ�����ǵ͵�ƽ 
	
   // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	  GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ�����ǵ͵�ƽ 
	
	  GPIO_SetBits(GPIOA,GPIO_Pin_3);
	  GPIO_SetBits(GPIOC,GPIO_Pin_3);
}
 
/************************************************
*���������� ��	DS18B20_Reset

*������������ ��  �o

*����������ֵ��	:	   �ɹ�����true ʧ������false

*���������������� ��   18b20�ď�λ����
*************************************************/
BOOL DS18B20_Reset(uchar select)
{
    uchar i;

    switch (select)
    {
    case 1:
        DQ_1 = 0;                     //�ͳ��͵�ƽ��λ�ź�
        Delay_us(480);              //��ʱ����480us
        Delay_us(60);
        DQ_1 = 1;                     //�ͷ�������
        Delay_us(60);               //�ȴ�60us
        i = DQ_1_IN;                    //����������
        Delay_us(420);              //�ȴ��豸�ͷ�������
        Delay_us(60);
        break;
    case 2:
        DQ_2 = 0;                     //�ͳ��͵�ƽ��λ�ź�
        Delay_us(480);              //��ʱ����480us
        Delay_us(60);
        DQ_2 = 1;                     //�ͷ�������
        Delay_us(60);               //�ȴ�60us
        i = DQ_2_IN;                    //����������
        Delay_us(420);              //�ȴ��豸�ͷ�������
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
*���������� ��	WriteByteTo_DS18B20

*������������ �� Ҫ�l�͵��ֹ�ֵ

*����������ֵ��	:	�o

*���������������� �� ��18b20��һ���ֹ��ĺ���
*************************************************/
void WriteByteTo_DS18B20(uchar dat, uchar select)
{
    uchar i;
    switch (select)
    {
    case 1:
        for (i = 0; i < 8; i++)             //8λ������
        {
            DQ_1 = 0;
            Delay_us(2);
            DQ_1 = dat & 0x01; //ȡ���λ�x�oDQ���_
            Delay_us(60);               //�ȴ�ʱ��Ƭ����
            Delay_us(20);
            DQ_1 = 1;
            Delay_us(1); // ���r��D�������@��Trec �༴�֏͕r�g��
            Delay_us(1);
            dat >>= 1;
        }
        break;
    case 2:
        for (i = 0; i < 8; i++)             //8λ������
        {
            DQ_2 = 0;
            Delay_us(2);
            DQ_2 = dat & 0x01; //ȡ���λ�x�oDQ���_
            Delay_us(60);               //�ȴ�ʱ��Ƭ����
            Delay_us(20);
            DQ_2 = 1;
            Delay_us(1); // ���r��D�������@��Trec �༴�֏͕r�g��
            Delay_us(1);
            dat >>= 1;
        }
        break;
    }


}

/************************************************
*������ ��	   ReadByteFr_DS18B20

*�������� ��	�o

*��������ֵ	:	���ص��ֹ�

*������������ �� ��18b20�xȡһ���ֹ�
*************************************************/
uchar ReadByteFr_DS18B20(uchar select)
{
    uchar dat = 0;
    uchar i;
    switch (select)
    {
    case 1:
        for (i = 0; i < 8; i++)             //8λ������
        {
            dat >>= 1; //���λ��0
            DQ_1 = 0;
            Delay_us(2);
            DQ_1 = 1; //����DQ ���ٷ��փ�Ӣ���Y��
            Delay_us(4); //�����ӽ��ٽ�15us�ĵط�
            if (DQ_1_IN)
            {
                dat |= 0x80; //����1 ������׃
            }
            Delay_us(60);
        }
        break;
    case 2:
        for (i = 0; i < 8; i++)             //8λ������
        {
            dat >>= 1; //���λ��0
            DQ_2 = 0;
            Delay_us(2);
            DQ_2 = 1; //����DQ ���ٷ��փ�Ӣ���Y��
            Delay_us(4); //�����ӽ��ٽ�15us�ĵط�
            if (DQ_2_IN)
            {
                dat |= 0x80; //����1 ������׃
            }
            Delay_us(60);
        }
        break;
    }
    return dat;
}


/************************************************
*���������� ��	 Read_DS18B20_ROM

*������������ ��	�o

*����������ֵ��	:	�o

*���������������� �����߿�������ȡ64λROM��ֻ�е�������ֻ����һ��
DS18B20��ʱ�򣬲ſ���ʹ�ô����������ز�ֹһ��
��ͨ��ʱ���ᷢ�����ݳ�ͻ��
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



 
 
 
