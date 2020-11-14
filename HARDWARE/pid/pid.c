#include "pid.h"

double PositionPID(double err,PID pid) {
	double pos_kp=pid.kp,pos_ki=pid.ki,pos_kd=pid.kd;
	static double Bias,Pwm,Integral_bias=0,Last_Bias;
	Bias=err;
	Integral_bias+=Bias;
	Pwm=pos_kp*Bias+pos_ki*Integral_bias+pos_kd*(Bias-Last_Bias);
	Last_Bias=Bias;
	return Pwm;
}








