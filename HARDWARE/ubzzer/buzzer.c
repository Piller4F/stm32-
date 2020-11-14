#include "buzzer.h"
#include "sys.h"

void Buzzer_Init(void) {
	RCC->APB2ENR|=1<<2;      //使能GPIOA
	GPIOA->CRL&=0xFFFFF0FF;  //PA2配置为输出
	GPIOA->CRL|=0x00000300;  
	
	BUZZER=0;                //蜂鸣器不响
}


