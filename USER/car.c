#include "car.h"
#include "buzzer.h"
#include "88led.h"
#include "tb6612.h"
#include "delay.h"

Car car; 

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
	car.cp=CHECK_PICTURE;
	car.status=CHOOSE;
	car.key=POWER;
	GO_Sharp();
	LSPEED=car.left_speed=0;
	RSPEED=car.right_speed=0;
}

void Car_Check(void) {
	BUZZER=1;              //·äÃùÆ÷Ïì
	GO_Right();
	car.cp=CHECK_PICTURE;
	LSPEED=car.left_speed=MAX_SPEED*0.5;
	RSPEED=car.right_speed=MAX_SPEED*0.5;
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
			BUZZER=1;
			delay_ms(100);
			BUZZER=0;
			Car_Check();
			car.status=CHECK;
			break;
		case ONE:
			BUZZER=1;
			delay_ms(100);
			BUZZER=0;
			car.status=REMOTE;
			break;
		case TWO:
			BUZZER=1;
			delay_ms(100);
			BUZZER=0;
			car.status=AVOID;
			break;
		case THREE:
			BUZZER=1;
			delay_ms(100);
			BUZZER=0;
			car.status=SEARCH;
			break;
		case FOUR:
			BUZZER=1;
			delay_ms(100);
			BUZZER=0;
			car.status=SEARCH_PID;
			break;
		case FIVE:
			BUZZER=1;
			delay_ms(100);
			BUZZER=0;
			car.status=POSITION_PID;
			break;
	}
}



