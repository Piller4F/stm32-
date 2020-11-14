#ifndef __CAR_H__
#define __CAR_H__
#include "remote.h"

typedef enum {
	CHOOSE,CHECK,REMOTE,AVOID,SEARCH,SEARCH_PID,POSITION_PID
}CarStatus;

typedef struct {
	CarStatus status;       //�ٿ�״̬
	KEY key;                //���ⰴ��
}Car;

extern Car car;

void ChangeStatus(void);

#endif
