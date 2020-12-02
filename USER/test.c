#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "iic.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "hc06.h"

int main() {
	int i,len;
	Stm32_Clock_Init(9);
	delay_init(72);
	uart_init(72,38400);
	USART2_Init(36,9600);
	//TIM4_Init(10,7199);
	while(1) {
//		usart2_send_char('0');
		if(USART_RX_STA&0x8000) {
			len=USART_RX_STA&0x3FFF;
			USART_RX_BUF[len++]='\r';
			USART_RX_BUF[len++]='\n';
			for(i=0;i<len;++i) {
				USART2->DR=USART_RX_BUF[i];
				while((USART2->SR&0x40)==0);
			}
			USART_RX_STA=0;
		}
		if(USART2_RX_STA&0x8000) {
			len=USART2_RX_STA&0x3FFF;
			for(i=0;i<len;++i) {
				USART1->DR=USART2_RX_BUF[i];
				while((USART1->SR&0x40)==0);
			}
			USART2_RX_STA=0;
		}
	}
}




