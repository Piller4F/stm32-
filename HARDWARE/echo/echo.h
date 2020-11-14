#ifndef __ECHO_H__
#define __ECHO_H__

#include "sys.h"

#define Trig PBout(13)
#define ECHO PAin(0)

void Echo_Init(void);
void Tim2_Cap_Init(u16 arr,u16 psc);

extern u16 TIM2_CAP_STA;
extern u16 TIM2_CAP_VAL;

#endif
