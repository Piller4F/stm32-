#include "buzzer.h"
#include "sys.h"

void Buzzer_Init(void) {
	RCC->APB2ENR|=1<<2;      //ʹ��GPIOA
	GPIOA->CRL&=0xFFFFF0FF;  //PA2����Ϊ���
	GPIOA->CRL|=0x00000300;  
	
	BUZZER=0;                //����������
}


