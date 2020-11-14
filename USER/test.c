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
	Stm32_Clock_Init(9);         //系统时钟初始化
	delay_init(72);              //延时初始化
	uart_init(72,9600);          //串口初始化
	Car_Init();                  //小车参数初始化
	Remote_Init();               //红外按键初始化
	Tb6612_Init();               //电机驱动模块初始化
	LED_Init();                  //8*8点阵LED初始化
	Echo_Init();                 //超声波初始化
	Buzzer_Init();               //蜂鸣器初始化
    Tim4_Pwm_Init(MAX_SPEED,0);  //配置pwm
    TIM3_Int_Init(10,7199);	 //88led刷新率初始化
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








