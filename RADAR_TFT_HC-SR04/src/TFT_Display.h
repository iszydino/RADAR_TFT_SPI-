#ifndef TFT_DISPLAY_H
#define TFT_DISPLAY_H

#define TRIG_PIN 25
#define ECHO_PIN 26


#include <TFT_eSPI.h>
#include <SPI.h>
void TFT_Init();

// Vẽ lưới radar
void drawRadarGrid();

// Vẽ và xóa đường quét
void drawScanLine(int angle);
void eraseScanLine(int angle);

// Vẽ đối tượng phát hiện được
void drawObject(int angle, int distance);
void drawRadarGrid();

// Hiển thị thông tin góc và khoảng cách
void showInfo(int angle, int distance);

// Hàm mới: Chuyển đổi màu RGB sang định dạng 16-bit
uint16_t convertRGB(uint8_t r, uint8_t g, uint8_t b);

// Hàm mới: Hiệu ứng sóng khi phát hiện đối tượng
void drawDetectionWave(int x, int y);

// Hàm mới: Hiệu ứng khởi động radar
void startupAnimation();
#endif