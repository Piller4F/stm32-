#include "car.h"
#include "buzzer.h"
#include "88led.h"
#include "tb6612.h"
#include "delay.h"

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



