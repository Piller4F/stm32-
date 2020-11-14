#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "remote.h"
#include "car.h"

int main() {
	Stm32_Clock_Init(9);   //系统时钟初始化
	delay_init(72);        //延时初始化
	uart_init(72,9600);    //串口初始化
	Remote_Init();         //红外按键初始化
	//car.status=CHOOSE;
	while(1) {
		ChangeStatus();
		if(car.status==CHOOSE) continue;
		printf("%d\r\n",car.status);
	}
}









