#ifndef RADAR_SENSOR_H
#define RADAR_SENSOR_H

#define TRIG_PIN 25
#define ECHO_PIN 26
#include <Arduino.h>
#include <math.h>
#include "TFT_Display.h"
#include "Servo_esp.h"
void Init_radar();
int readUltrasonic();

#endif