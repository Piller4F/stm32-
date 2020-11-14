#include "88led.h"
#include "sys.h"

int led[8]={3,4,5,6,7,8,13,14};   //列
int line[8]={9,8,7,6,0,1,3,4};    //行
int picture[11][8]= {{0xff,0xe3,0xdf,0xdf,0xe7,0xfb,0xfb,0xc7},
					{0},
					{231,219,189,24,219,219,219,195},
					{195,219,219,219,24,189,219,231},
					{239,207,176,126,126,176,207,239},
					{247,243,5,126,126,5,243,247},
					{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
					{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
					{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
					{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
					{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
};

void LED_Init(void) {
	JTAG_Set(JTAG_SWD_DISABLE);
	RCC->APB2ENR|=1<<2;     //使能GPIOA
	RCC->APB2ENR|=1<<3;     //B
	RCC->APB2ENR|=1<<4;     //C
	
	GPIOA->CRL&=0x00000FFF;
	GPIOA->CRL|=0x33333000;
	GPIOA->CRH&=0xF00FFFF0;
	GPIOA->CRH|=0x03300003;
	
	GPIOB->CRL&=0xFFF00F00;
	GPIOB->CRL|=0x00033033;
	
	GPIOC->CRL&=0x00FFFFFF;
	GPIOC->CRL|=0x33000000;
	GPIOC->CRH&=0xFFFFFF00;
	GPIOC->CRH|=0x00000033;
	
	Line0=Line1=Line2=Line3=Line4=Line5=Line6=Line7=1;
	
	Led0=Led1=Led2=Led3=Led4=Led5=Led6=Led7=1;
	
}

void Led_Light(u8 n) {
	int i=0;
	//for(i=0;i<8;++i) PAout(led[cnt])=0;
	for(i=0;i<8;++i) {
		if((n&1) == 0) {
			PAout(led[i])=0;
		} else {
			PAout(led[i])=1;
		}
		n/=2;
	}
}


