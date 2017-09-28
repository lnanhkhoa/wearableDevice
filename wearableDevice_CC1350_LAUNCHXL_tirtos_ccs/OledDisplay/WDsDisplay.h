/*
 * WDsDisplay.h
 *
 *  Created on: Sep 5, 2017
 *      Author: AppsTI
 */

#ifndef OLEDDISPLAY_WDSDISPLAY_H_
#define OLEDDISPLAY_WDSDISPLAY_H_

#include <stdint.h>
#include <OledDisplay/Adafruit_SSD1306.h>

/* BLE icon's location */
#define BLEicon_x 0
#define BLEicon_y 0

#define BLEicon_width 16
#define BLEicon_height 16
/* Battery icon's location */
#define BATicon_x 112
#define BATicon_y 0

#define BATicon_width 16
#define BATicon_height 16

/* Font */
#define font_width  6
#define font_height 8


/* Clock */
#define clock_textSize   3
#define clock_textColor  WHITE
const uint8_t clock_cursor[2] = {19, 28};

/* Heart Rate */
#define heart_x 10
#define heart_y 16
const uint8_t sizeBigHeart[2] = {48, 48};
const uint8_t sizeSmallHeart[2] = {32, 32};

/* Foot Steps */
#define footsteps_x 10
#define footsteps_y 16
const uint8_t sizeFootsteps[2] = {48, 48};

class WDsDisplay : public Adafruit_SSD1306
{
public:
    WDsDisplay(int8_t rst);
    virtual ~WDsDisplay();
    void Bluetooth_icon(bool enable);
    void Battery_charging(void);
    void Battery_set(uint8_t percent, bool charge = false);
    void Clock_set(uint8_t hh, uint8_t mm);
    void Colon_toogle(void);
    void Heartrate_status(void);
    void Heartrate_number(int8_t number); // -1 is wait
    void Footsteps_status(void);
    void Footsteps_number(uint32_t number);

    void Display_head(void);
    void Display_body(void);
    void Clear_head(void);
    void Clear_body(void);
private:
    bool colon_status, footsteps_status;
    uint8_t heart_status;
    void clear_16x16(int16_t x, int16_t y, uint16_t color);
    void clear_18x24(int16_t x, int16_t y, uint16_t color);
    void clear_heart(int16_t x, int16_t y, uint16_t color);
    void clear_character(int16_t x, int16_t y, uint16_t fontsize, uint16_t length, uint16_t color);
};

#endif /* OLEDDISPLAY_WDSDISPLAY_H_ */
