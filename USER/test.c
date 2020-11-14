#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "remote.h"
#include "car.h"
#include "tb6612.h"
#include "88led.h"
#include "buzzer.h"
#include "timer.h"
#include "echo.h"

int main() {
	Stm32_Clock_Init(9);         //ϵͳʱ�ӳ�ʼ��
	delay_init(72);              //��ʱ��ʼ��
	uart_init(72,9600);          //���ڳ�ʼ��
	Car_Init();                  //С��������ʼ��
	Remote_Init();               //���ⰴ����ʼ��
	Tb6612_Init();               //�������ģ���ʼ��
	LED_Init();                  //8*8����LED��ʼ��
	Echo_Init();                 //��������ʼ��
	Buzzer_Init();               //��������ʼ��
    Tim4_Pwm_Init(MAX_SPEED,0);  //����pwm
    TIM3_Int_Init(10,7199);	 //88ledˢ���ʳ�ʼ��
	Tim2_Cap_Init(65535,72);
	while(1) {
		Change_Status();
		if(car.status==CHOOSE) {
			Car_Init();
			continue;
		}
		//printf("%d\r\n",car.status);
	}
}








