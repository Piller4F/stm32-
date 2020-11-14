#ifndef __PID_H__
#define __PID_H__

typedef struct PID{
	double kp;
	double ki;
	double kd;
}PID;

double PositionPID(double err,PID pid);

#endif

