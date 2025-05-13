#ifndef SYS_RUN_H
#define SYS_RUN_H

#include "TFT_Display.h"
#include "Servo_esp.h"
#include "Read_Radar.h"

extern int distance;
extern int lastDistance ; 
extern int angle;
extern int lastAngle ;

void updateRadar(int angle);
void Radar_spin();


#endif