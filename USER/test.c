#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "remote.h"

typedef enum {
	CHOOSE,CHECK,REMOTE,AVOID,SEARCH,SEARCH_PID,POSITION_PID
}CarStatus;

typedef struct {
	CarStatus status;       //�ٿ�״̬
	KEY key;                //���ⰴ��
}Car;

Car car; 

void ChangeStatus(void);

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

void ChangeStatus(void) {
	car.key=(KEY)Remote_Scan();
	if(!car.key) return;
	switch(car.key) {
		case POWER:
			car.status=CHOOSE;
			break;
		case ZERO:
			car.status=CHECK;
			break;
		case ONE:
			car.status=REMOTE;
			break;
		case TWO:
			car.status=AVOID;
			break;
		case THREE:
			car.status=SEARCH;
			break;
		case FOUR:
			car.status=SEARCH_PID;
			break;
		case FIVE:
			car.status=POSITION_PID;
			break;
	}
}







