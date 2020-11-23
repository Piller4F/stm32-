#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "iic.h"

int main() {
	Stm32_Clock_Init(9);
	delay_init(72);
	uart_init(72,9600);
	while(1) {
		printf("ÄãºÃ\r\n");
	}
}




