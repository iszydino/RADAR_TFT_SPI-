#include "Read_Radar.h"
void Init_radar()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}
int readUltrasonic()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    int duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout 30ms
    if (duration == 0)
        return 201; // Nếu timeout, trả về giá trị lớn hơn giới hạn

    int distance = duration * 0.034 / 2; // Tính khoảng cách (cm)
    return min(distance, (int)100);      // Giới hạn tối đa 200cm
}