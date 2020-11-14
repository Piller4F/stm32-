#ifndef __8_8_H__
#define __8_8_H__
#include "sys.h"
//ÁÐ
#define Led7 PAout(3) 
#define Led6 PAout(4)
#define Led5 PAout(5)
#define Led4 PAout(6)
#define Led3 PAout(7)
#define Led2 PAout(8)
#define Led1 PAout(13)
#define Led0 PAout(14)

//ÐÐ
#define Line0 PCout(9)
#define Line1 PCout(8)
#define Line2 PCout(7)
#define Line3 PCout(6)
#define Line4 PBout(0)
#define Line5 PBout(1)
#define Line6 PBout(3)
#define Line7 PBout(4)

void LED_Init(void);
void Led_Light(u8 n);

#endif

