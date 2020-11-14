#include "car.h"

Car car; 

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