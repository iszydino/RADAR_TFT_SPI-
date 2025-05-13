#include "SYS_RUN.h"
int lastAngle = 0;
int distance = 0;
int lastDistance = 0;

void Radar_spin()
{
    for (int angle = 0; angle <= 180; angle += 1)
    {
        updateRadar(angle);
    }

    // Quét từ 180° về 0°
    for (int angle = 180; angle >= 0; angle -= 1)
    {
        updateRadar(angle);
    }
}

void updateRadar(int angle)
{
    // Di chuyển servo
    servo.write(angle);

    // Đọc khoảng cách
    long distance = readUltrasonic();

    // Xóa vạch quét cũ
    eraseScanLine(lastAngle);

    drawRadarGrid();

    drawScanLine(angle);

    // Vẽ đối tượng (nếu phát hiện)
    if (distance < 200)
    {
        drawObject(angle, distance);
    }

    // Hiển thị thông tin
    showInfo(angle, distance);

    lastAngle = angle;

    delay(15);
}
