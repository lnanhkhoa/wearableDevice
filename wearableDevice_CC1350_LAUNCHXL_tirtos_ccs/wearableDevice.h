


#ifndef TASKS_WEARABLEDEVICE_H_
#define TASKS_WEARABLEDEVICE_H_

#include "stdint.h"


#define EVENT_ALL                           0xFFFFFFFF
#define EVENT_UPDATE_OLED                   (uint32_t)(1 << 0)

#define EVENT_SLEEPMODE_START               (uint32_t)(1 << 1)
#define EVENT_SLEEPMODE_COMPLETE            (uint32_t)(1 << 2)
#define EVENT_TURNON_DISPLAY                (uint32_t)(1 << 3)

#define OLED_RESET 4

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2



// #if (SSD1306_LCDHEIGHT != 64)
// #error("Height incorrect, please fix Adafruit_SSD1306.h!");
// #endif

struct eventchange{
    bool head;
    bool body;
}eventChange;

struct elementhead{
    bool bleIcon;
    char text[20];
    uint8_t batteryLevel;
}elementHead;


void wearableDevice_init();

#endif
