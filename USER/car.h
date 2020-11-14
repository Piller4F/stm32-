#ifndef __CAR_H__
#define __CAR_H__
#include "remote.h"

typedef enum {
	CHOOSE,CHECK,REMOTE,AVOID,SEARCH,SEARCH_PID,POSITION_PID
}CarStatus;

typedef struct {
	CarStatus status;       //²Ù¿Ø×´Ì¬
	KEY key;                //ºìÍâ°´¼ü
}Car;

extern Car car;

void ChangeStatus(void);

#endif
