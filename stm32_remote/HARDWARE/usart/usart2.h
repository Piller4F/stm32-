#ifndef __USART2_H
#define __USART2_H
#include "sys.h"
#define USART2_MAX_RECV_LEN 200


void USART2_IRQHandler(void);
void USART2_Init(u32 pcl,u32 bound);
void TIM4_Init(u16 arr, u16 psc);

extern u8 USART2_RX_BUF[USART2_MAX_RECV_LEN];
extern u16 USART2_RX_STA;         



#endif
