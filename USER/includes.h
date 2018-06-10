#ifndef __INCLUDES_H__
#define __INCLUDES_H__

/***************************************
*  此文件只用来包含各种头文件，避免main.c太乱！
*  关于这个头文件包含，我多说一嘴，就是
* " 哪个.c文件需要什么头文件，就自己加。。"
* 用不到的头文件，不要加。。乱~~
****************************************/

#include <stm32f10x.h> 
 

#include "user.h"  //因为 main.c 文件用到了 user.c 文件下的某个函数
#include "delay.h" //因为 main.c 文件用到了 delay.c 文件下的某个函数
#include "tools.h" //...........
#include "pcf8563.h"
#include "gsm.h" 
#include "HX711.h"
#include "lcd.h"
#include "sim900a.h" 

#endif


 //若要使用 bool  在 stdbool.h 文件里面 
