#ifndef __HC06_H__
#define __HC06_H__
#include "sys.h"


#define USART2_MAX_RECV_LEN		200					//�����ջ����ֽ���

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�
extern u16 USART2_RX_STA;   						//��������״̬

void USART2_Init(u32 pclk2,u32 bound);              //����2��ʼ��
void TIM4_Init(u16 arr,u16 psc);                    //��ʱ��
void usart2_send_char(u8 c);


#endif
