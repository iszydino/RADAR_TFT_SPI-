#include "TFT_Display.h"
#include "SYS_RUN.h"
TFT_eSPI tft = TFT_eSPI();

#define GRID_COLOR TFT_DARKGREEN
#define SCANLINE_COLOR TFT_GREEN
#define OBJECT_COLOR TFT_RED
#define TEXT_COLOR TFT_GREEN
#define SCANLINE_FADE1 0x0360
#define SCANLINE_FADE2 0x01E0
#define SCANLINE_FADE3 0x00A0
#define BACKGROUND_COLOR TFT_BLACK
#define HIGHLIGHT_COLOR 0xFFE0

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 160;
const int RADAR_CENTER_X = SCREEN_WIDTH / 2;
const int RADAR_CENTER_Y = SCREEN_HEIGHT * 2 / 3;
const int RADAR_RADIUS = 50;

int lastObjectX = -1;
int lastObjectY = -1;
bool objectDrawn = false;
uint16_t fadeColors[RADAR_RADIUS];

uint16_t convertRGB(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void TFT_Init()
{
    Serial.begin(115200);
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(BACKGROUND_COLOR);

    for (int i = 0; i < RADAR_RADIUS; i++)
    {
        uint8_t intensity = map(i, 0, RADAR_RADIUS - 1, 255, 30);
        fadeColors[i] = convertRGB(0, intensity, intensity / 3);
    }

    startupAnimation();
    drawRadarGrid();

    tft.drawRect(0, 0, SCREEN_WIDTH, 40, GRID_COLOR);
    tft.setTextColor(TEXT_COLOR, BACKGROUND_COLOR);
    tft.setTextSize(1);
    tft.setCursor((SCREEN_WIDTH - 11 * 6) / 2, 3);
    tft.print("RADAR SYSTEM");
}

void drawObject(int angle, int distance)
{
    if (distance <= 0 || distance > 100)
        return;

    if (objectDrawn)
    {
        for (int a = lastAngle - 15; a <= lastAngle + 15; a++)
        {
            float rad = radians(a);
            int r = map(lastDistance, 0, 100, 0, RADAR_RADIUS);
            int x = RADAR_CENTER_X + r * cos(rad);
            int y = RADAR_CENTER_Y - r * sin(rad);
            tft.fillCircle(x, y, 2, TFT_BLACK);
        }
        objectDrawn = false;
    }

    for (int a = angle - 15; a <= angle + 15; a++)
    {
        if (a < 0 || a > 180)
            continue;

        float rad = radians(a);
        int r = map(distance, 0, 100, 0, RADAR_RADIUS);
        int x = RADAR_CENTER_X + r * cos(rad);
        int y = RADAR_CENTER_Y - r * sin(rad);
        tft.fillCircle(x, y, 2, OBJECT_COLOR);
    }

    lastObjectX = -1;
    lastObjectY = -1;
    lastAngle = angle;
    lastDistance = distance;
    objectDrawn = true;
}

void eraseScanLine(int angle)
{
    int sweep = 15;
    int startAngle = max(0, angle - sweep);
    int endAngle = min(180, angle + sweep);

    for (int r = 0; r <= RADAR_RADIUS; r++)
    {
        for (int a = startAngle; a <= endAngle; a++)
        {
            float rad = radians(a);
            int x = RADAR_CENTER_X + r * cos(rad);
            int y = RADAR_CENTER_Y - r * sin(rad);
            tft.drawPixel(x, y, TFT_BLACK);
        }
    }
}

void drawScanLine(int angle)
{
    int sweep = 15;
    int startAngle = max(0, angle - sweep);
    int endAngle = min(180, angle + sweep);

    for (int r = 0; r <= RADAR_RADIUS; r++)
    {
        for (int a = startAngle; a <= endAngle; a++)
        {
            float rad = radians(a);
            int x = RADAR_CENTER_X + r * cos(rad);
            int y = RADAR_CENTER_Y - r * sin(rad);
            tft.drawPixel(x, y, SCANLINE_COLOR);
        }
    }
    delay(50);
}

void drawRadarGrid()
{
    for (int r = 10; r <= RADAR_RADIUS; r += 10)
    {
        tft.drawCircle(RADAR_CENTER_X, RADAR_CENTER_Y, r, GRID_COLOR);

        if (r % 20 == 0)
        {
            int distance = map(r, 0, RADAR_RADIUS, 0, 200);
            tft.setTextColor(TEXT_COLOR, TFT_BLACK);
            tft.setTextSize(1);
            tft.setCursor(RADAR_CENTER_X + r - 12, RADAR_CENTER_Y + 2);
            tft.printf("%d", distance);
        }
    }

    for (int angle = 0; angle <= 180; angle += 30)
    {
        float rad = radians(angle);
        int x = RADAR_CENTER_X + RADAR_RADIUS * cos(rad);
        int y = RADAR_CENTER_Y - RADAR_RADIUS * sin(rad);
        tft.drawLine(RADAR_CENTER_X, RADAR_CENTER_Y, x, y, GRID_COLOR);

        if (angle > 0 && angle < 180)
        {
            int textX = RADAR_CENTER_X + (RADAR_RADIUS + 8) * cos(rad) - 6;
            int textY = RADAR_CENTER_Y - (RADAR_RADIUS + 8) * sin(rad) - 4;
            tft.setCursor(textX, textY);
            tft.printf("%d", angle);
        }
    }
}

void showInfo(int angle, int distance)
{
    tft.fillRect(1, 1, SCREEN_WIDTH - 2, 38, BACKGROUND_COLOR);

    tft.setTextColor(TEXT_COLOR, BACKGROUND_COLOR);
    tft.setTextSize(1);

    tft.setCursor((SCREEN_WIDTH - 11 * 6) / 2, 3);
    tft.print("RADAR SYSTEM");

    tft.drawFastHLine(10, 12, SCREEN_WIDTH - 20, GRID_COLOR);

    tft.setCursor(5, 16);
    tft.printf("Angle: %d", angle);

    tft.setCursor(5, 28);
    if (distance < 30)
    {
        if (millis() % 1000 < 500)
        {
            tft.setTextColor(HIGHLIGHT_COLOR, BACKGROUND_COLOR);
            tft.printf("ALERT: %d cm", distance);
        }
        else
        {
            tft.setTextColor(TFT_RED, BACKGROUND_COLOR);
            tft.printf("ALERT: %d cm", distance);
        }
    }
    else
    {
        tft.printf("Distance: %d cm", distance);
    }

    int signalBars = map(min(distance, 100), 0, 100, 5, 1);
    for (int i = 0; i < 5; i++)
    {
        if (i < signalBars)
        {
            tft.fillRect(SCREEN_WIDTH - 20 + i * 3, 28 - i * 2, 2, 2 + i * 2, TEXT_COLOR);
        }
        else
        {
            tft.drawRect(SCREEN_WIDTH - 20 + i * 3, 28 - i * 2, 2, 2 + i * 2, GRID_COLOR);
        }
    }

    tft.drawRect(0, 0, SCREEN_WIDTH, 40, GRID_COLOR);
}

void startupAnimation()
{
    for (int x = 0; x < SCREEN_WIDTH; x += 4)
    {
        tft.drawFastVLine(x, 0, SCREEN_HEIGHT, SCANLINE_COLOR);
        delay(10);
        tft.drawFastVLine(x, 0, SCREEN_HEIGHT, BACKGROUND_COLOR);
    }

    for (int r = 0; r <= RADAR_RADIUS + 10; r += 2)
    {
        tft.drawCircle(RADAR_CENTER_X, RADAR_CENTER_Y, r, TEXT_COLOR);
        delay(20);
        if (r < RADAR_RADIUS)
        {
            tft.drawCircle(RADAR_CENTER_X, RADAR_CENTER_Y, r, BACKGROUND_COLOR);
        }
    }

    tft.setTextColor(HIGHLIGHT_COLOR, BACKGROUND_COLOR);
    tft.setTextSize(1);
    tft.setCursor((SCREEN_WIDTH - 12 * 6) / 2, RADAR_CENTER_Y);
    tft.print("SYSTEM READY");
    delay(1000);

    tft.fillRect((SCREEN_WIDTH - 12 * 6) / 2, RADAR_CENTER_Y, 12 * 6, 8, BACKGROUND_COLOR);
    tft.fillScreen(BACKGROUND_COLOR);
}

void radar_loop_example()
{
    static int currentAngle = 0;
    static unsigned long lastUpdateTime = 0;
    static bool objectDetected = false;
    static int objectAngle = 0;
    static int objectDistance = 0;

    if (millis() - lastUpdateTime > 100)
    {
        eraseScanLine(currentAngle);

        currentAngle = (currentAngle + 5) % 181;

        if (random(100) < 5 && !objectDetected)
        {
            objectDetected = true;
            objectAngle = random(30, 150);
            objectDistance = random(20, 90);
        }

        if (objectDetected && random(100) < 2)
        {
            objectDetected = false;
        }

        if (objectDetected)
        {
            drawObject(objectAngle, objectDistance);
        }

        drawScanLine(currentAngle);

        lastUpdateTime = millis();
    }
}
