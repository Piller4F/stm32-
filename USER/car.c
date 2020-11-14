#include "car.h"
#include "buzzer.h"
#include "88led.h"
#include "tb6612.h"
#include "delay.h"
#include "echo.h"
#include <time.h>
#include "stdlib.h"
#include "usart.h"

Car car; 

void GO_Stop(void) {
	AIN1=0,AIN2=0;
	BIN1=0,BIN2=0;
}

void GO_Sharp(void) {
	AIN1=1,AIN2=0;
	BIN1=1,BIN2=0;
}

void GO_Left(void) {
	AIN1=0,AIN2=1;
	BIN1=1,BIN2=0;
}

void GO_Right(void) {
	AIN1=1,AIN2=0;
	BIN1=0,BIN2=1;
}

void GO_Back(void) {
	AIN1=0,AIN2=1;
	BIN1=0,BIN2=1;
}

void Car_Init(void) {
	BUZZER=0;
	car.status=CHOOSE;
	car.key=POWER;
	GO_Stop();
	LSPEED=car.left_speed=0;
	RSPEED=car.right_speed=0;
}

void Car_Check(void) {
	BUZZER=1;              //·äÃùÆ÷Ïì
	GO_Right();
	LSPEED=car.left_speed=MAX_SPEED*0.5;
	RSPEED=car.right_speed=MAX_SPEED*0.5;
}

void Echo_Car(void) {
	double distance;
	int dir;
	LSPEED=car.left_speed=MAX_SPEED*0.5;
	RSPEED=car.right_speed=MAX_SPEED*0.5;
	while(1) {
		car.key=(KEY)Remote_Scan();
		if(car.key==POWER) {
			car.status=CHOOSE;
			Car_Init();
			return;
		}
		dir=rand()%2;
		Trig=1;
		delay_us(20);
		Trig=0;
		if(TIM2_CAP_STA&0x8000) {
			distance=TIM2_CAP_VAL;
			distance+=65536*(TIM2_CAP_STA&0x3FFF);
			distance/=1000000.0;
			distance=distance*17000;
			printf("%f cm \r\n",distance);
			TIM2_CAP_STA=0;
			if(distance<=20) {
				LSPEED=MAX_SPEED*0.7;
			    RSPEED=MAX_SPEED*0.7;
				//car.status=GOBACK;
				GO_Back();
				delay_ms(100);
				if(dir) {
					car.status=GORIGHT;
					GO_Right();
				} else {
					car.status=GOLEFT;
					GO_Left();
				}
				delay_ms(500);
				LSPEED=MAX_SPEED*0.5;
				RSPEED=MAX_SPEED*0.5;
			} else {
				car.status=GOUP;
				GO_Sharp();
			}
		}
	}
}

void Remote_Car(void) { 
	while(1) {
		car.key=(KEY)Remote_Scan();
		LSPEED=car.left_speed=MAX_SPEED*0.6;
		RSPEED=car.right_speed=MAX_SPEED*0.6;
		switch(car.key) {
			case UP:
				car.status=GOUP;
				GO_Sharp();
				break;
			case DOWN:
				car.status=GOBACK;
				GO_Back();
				break;
			case LEFT:
				car.status=GOLEFT;
				GO_Left();
				break;
			case RIGHT:
				car.status=GORIGHT;
				GO_Right();
				break;
			case POWER:
				BUZZER=1;
				delay_ms(100);
				BUZZER=0;
				car.status=CHOOSE;
				Car_Init();
				return;
			default:
				car.status=REMOTE;
				GO_Stop();
				break;
		}
	}
}

void Change_Status(void) {
	car.key=(KEY)Remote_Scan();
	if(!car.key) return;
	switch(car.key) {
		case POWER:
			BUZZER=1;
			delay_ms(100);
			BUZZER=0;
			car.status=CHOOSE;
			break;
		case ZERO:
			if(car.status!=CHOOSE) break;
			BUZZER=1;
			delay_ms(200);
			BUZZER=0;
			Car_Check();
			car.status=CHECK;
			break;
		case ONE:
			if(car.status!=CHOOSE) break;
			BUZZER=1;
			delay_ms(200);
			BUZZER=0;
			car.status=REMOTE;
			Remote_Car();
			break;
		case TWO:
			if(car.status!=CHOOSE) break;
			BUZZER=1;
			delay_ms(200);
			BUZZER=0;
			car.status=AVOID;
			printf("123\r\r\n");
			Echo_Car();
			break;
		case THREE:
			if(car.status!=CHOOSE) break;
			BUZZER=1;
			delay_ms(200);
			BUZZER=0;
			car.status=SEARCH;
			break;
		case FOUR:
			if(car.status!=CHOOSE) break;
			BUZZER=1;
			delay_ms(200);
			BUZZER=0;
			car.status=SEARCH_PID;
			break;
		case FIVE:
			if(car.status!=CHOOSE) break;
			BUZZER=1;
			delay_ms(200);
			BUZZER=0;
			car.status=POSITION_PID;
			break;
	}
}



