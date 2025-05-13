
#include "SYS_RUN.h"
int angle = 0;

void setup()
{
  TFT_Init();
  Init_radar();
  Servo_init();
}

void loop()
{
  Radar_spin();
}