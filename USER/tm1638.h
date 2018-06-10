#ifndef __TI1638_H__
#define __TI1638_H__

#include <stm32f10x.h>
#include "sys.h"


/************************************************
*             �û������궨��(�ɱ�)              *
*************************************************/
//��Щ�궨�壬�б���Ĵ��������궨�壬�Ĵ��������궨��
//�Ǹ���datasheet���ģ� ���û������궨�壬���Ǹ���ԭ��ͼ��ָ���ģ�

#define  STB1           PCout(7)
#define  STB2           PAout(8)

#define  CLK1           PCout(9)
#define  CLK2           PCout(8)

#define  DIO1           PAout(9)
#define  DIO2           PCout(6)

#define  DIO1_IN           PAin(9)
#define  DIO2_IN           PCin(6)
/************************************************
*             �Ĵ��������궨��(�̶�)              *
*************************************************/
// ���������оƬ�ֲ��ϣ���Ҫ����һЩ���壬ʵ��
// ��̷�����궨��ģ��Ǹ��������ֲ����ģ�

#define   LIGHT_LEVE_OFF    0x80 //1000 0000
#define   LIGHT_LEVE_ON     0x88
#define   LIGHT_LEVE_1      0x89
#define   LIGHT_LEVE_2      0x8a
#define   LIGHT_LEVE_3      0x8b
#define   LIGHT_LEVE_4      0x8c
#define   LIGHT_LEVE_5      0x8d
#define   LIGHT_LEVE_6      0x8e
#define   LIGHT_LEVE_7      0x8f


#define   AUTO_ADDR_MODE         0x40    //��ַ�Զ���1
#define   FIXED_ADDR_MODE        0x44   //�̶���ַ

//����Ӧ����ַ��
#define   ADDR_0      0xc0      // 00H
#define   ADDR_1      0xc1      // 01H
#define   ADDR_2      0xc2      // 02H
#define   ADDR_3      0xc3      // 03H
#define   ADDR_4      0xc4      // 04H
#define   ADDR_5      0xc5      // 05H
#define   ADDR_6      0xc6      // 06H
#define   ADDR_7      0xc7      // 07H
#define   ADDR_8      0xc8      // 08H
#define   ADDR_9      0xc9      // 09H
#define   ADDR_10     0xca      // 0aH
#define   ADDR_11     0xcb      // 0bH
#define   ADDR_12     0xcc      // 0cH
#define   ADDR_13     0xcd      // 0dH
#define   ADDR_14     0xce      // 0eH
#define   ADDR_15     0xcf      // 0fH

#define   READ_KEY_MODE   0x42






/************************************************
*             ���⿪��-����                     *
*************************************************/
extern  uchar    ucTm1638_2FreeFlag;
extern  uchar    ucKeyTouch;
extern  uchar const table[];
extern  uchar ucKeyShortTuchFlag5; /*���ݴ˱�־�ж��Ƿ� �ϵ簴�����ð��� */




/************************************************
*             ���⿪��-����                     *
*************************************************/
void  TM1638_Init(void);
void  allotData(uchar tm1638x, uchar tubex, uchar tubeData); //ת��
void  WriteDatasTo_TM1638(uchar select); //��TM1638���ݴ洢����д����
void  ReadKeyValue(void);
void  TM1638_Config(void);






#endif
