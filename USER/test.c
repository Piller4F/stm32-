#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "remote.h"
#include "car.h"

int main() {
	Stm32_Clock_Init(9);   //ϵͳʱ�ӳ�ʼ��
	delay_init(72);        //��ʱ��ʼ��
	uart_init(72,9600);    //���ڳ�ʼ��
	Remote_Init();         //���ⰴ����ʼ��
	//car.status=CHOOSE;
	while(1) {
		ChangeStatus();
		if(car.status==CHOOSE) continue;
		printf("%d\r\n",car.status);
	}
}









