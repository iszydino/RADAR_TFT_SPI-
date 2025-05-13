#include <ESP32Servo.h>
#include "Servo_esp.h"

Servo servo;

void Servo_init() {
  servo.attach(SERVO_PIN);  // chỉnh chân servo phù hợp
  servo.write(90);
}
