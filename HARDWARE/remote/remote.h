#ifndef __REMOTE_H__
#define __REMOTE_H__
#include "sys.h"

#define RDATA PAin(1)

typedef enum {
	POWER=162,PLAY=2,UP=98,RIGHT=194,LEFT=34,DOWN=168,
	VOL1=224,VOL2=144,ZERO=66,ONE=104,TWO=152,THREE=176,
	FOUR=48,FIVE=24
}KEY;


void Remote_Init(void);
u8   Remote_Scan(void);
extern u8 RmtCnt;

#endif

