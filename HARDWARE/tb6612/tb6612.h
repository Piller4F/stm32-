#ifndef __TB6612_H__
#define __TB6612_H__

#include "sys.h"

#define STBY PBout(8)
#define AIN1 PBout(9)
#define AIN2 PBout(10)
#define BIN1 PBout(11)
#define BIN2 PBout(12)

#define LSPEED TIM4->CCR1
#define RSPEED TIM4->CCR2
#define PWMA  PBout(6)
#define PWMB  PBout(7)

void Tb6612_Init(void);
void Tim4_Pwm_Init(u16 arr,u16 psc); //≈‰÷√pwm

#endif
