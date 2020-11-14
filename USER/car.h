#ifndef __CAR_H__
#define __CAR_H__
#include "remote.h"

#define MAX_SPEED 6000

typedef enum {
	CHOOSE,CHECK,REMOTE,AVOID,SEARCH,SEARCH_PID,POSITION_PID
}CarStatus;

typedef enum {
	CHECK_PICTURE,UP_PICTURE
}CarPicture;

typedef struct {
	CarStatus status;       //�ٿ�״̬
	KEY key;                //���ⰴ��
	CarPicture cp;          //88ledͼ����ʾ
	u16 left_speed;         //�����ٶ�       
	u16 right_speed; 
}Car;

extern Car car;

void Change_Status(void);
void Car_Check(void);
void Car_Init(void);
void GO_Sharp(void);
void GO_Left(void);
void GO_Right(void);
void GO_Back(void);


#endif
