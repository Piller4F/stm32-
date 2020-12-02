#ifndef __HC06_H__
#define __HC06_H__
#include "sys.h"


#define USART2_MAX_RECV_LEN		200					//最大接收缓存字节数

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//接收缓冲,最大USART2_MAX_RECV_LEN字节
extern u16 USART2_RX_STA;   						//接收数据状态

void USART2_Init(u32 pclk2,u32 bound);              //串口2初始化
void TIM4_Init(u16 arr,u16 psc);                    //定时器
void usart2_send_char(u8 c);


#endif
