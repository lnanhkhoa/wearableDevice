/*
 * WDsDisplay.cpp
 *
 *  Created on: Sep 5, 2017
 *      Author: AppsTI
 */



#include "libDisplay/bluetooth_enabled.h"
#include "libDisplay/battery.h"
#include "libDisplay/heart.h"
#include "libDisplay/stepcount.h"

#include <stdio.h>
#include <string.h>
#include <OledDisplay/WDsDisplay.h>

/* BLE icon's location*/


WDsDisplay::WDsDisplay(int8_t rst): Adafruit_SSD1306(rst)
{
    colon_status = footsteps_status = false;
    heart_status = 0;
}


WDsDisplay::~WDsDisplay()
{

}


void WDsDisplay::Bluetooth_icon(bool enable){
    clear_16x16(BLEicon_x, BLEicon_y, 0);
    if(enable)
        drawBitmap(BLEicon_x, BLEicon_y, bluetooth_enabled , BLEicon_width, BLEicon_height, 1, 0);
}

void WDsDisplay::Battery_charging(void){
    clear_16x16(BATicon_x, BATicon_y, 0);
    drawBitmap(BATicon_x, BATicon_y, battery_charging, BATicon_width, BATicon_height, 1, 0);
}


void WDsDisplay::Battery_set(uint8_t percent, bool charge){
    clear_16x16(BATicon_x, BATicon_y, 0);
    if(percent>100) percent = 100;
    unsigned int per= percent/10;
    switch(per)
    {
    case 10: ;
    case 9: drawBitmap(BATicon_x, BATicon_y, battery_full, BATicon_width, BATicon_height, 1, 0); break;
    case 8: drawBitmap(BATicon_x, BATicon_y, battery_90p, BATicon_width, BATicon_height, 1, 0); break;
    case 7: drawBitmap(BATicon_x, BATicon_y, battery_80p, BATicon_width, BATicon_height, 1, 0); break;
    case 6: drawBitmap(BATicon_x, BATicon_y, battery_70p, BATicon_width, BATicon_height, 1, 0); break;
    case 5:
        if((unsigned int)(percent%10) >5){
            drawBitmap(BATicon_x, BATicon_y, battery_60p, BATicon_width, BATicon_height, 1, 0); break;
        }else{
            drawBitmap(BATicon_x, BATicon_y, battery_50p, BATicon_width, BATicon_height, 1, 0); break;
        }

    case 4: drawBitmap(BATicon_x, BATicon_y, battery_40p, BATicon_width, BATicon_height, 1, 0); break;
    case 3: drawBitmap(BATicon_x, BATicon_y, battery_30p, BATicon_width, BATicon_height, 1, 0); break;
    case 2: drawBitmap(BATicon_x, BATicon_y, battery_20p, BATicon_width, BATicon_height, 1, 0); break;
    case 1: drawBitmap(BATicon_x, BATicon_y, battery_10p, BATicon_width, BATicon_height, 1, 0); break;
    default:
        if(charge){
            drawBitmap(BATicon_x, BATicon_y, battery_low, BATicon_width, BATicon_height, 1, 0); break;
        }else{
            drawBitmap(BATicon_x, BATicon_y, battery_warning, BATicon_width, BATicon_height, 1, 0); break;
        }
    }

}


void WDsDisplay::Clock_set(uint8_t hh, uint8_t mm){
//    colon_status = true;
    char buffer[5];
    setTextSize(clock_textSize);
    setTextColor(clock_textColor);
    setCursor(clock_cursor[0], clock_cursor[1]);
    if(hh>24 || hh<0 || mm>60 || mm<0) return;

    if(hh<10)   sprintf(buffer, "0%d", hh);
    else        sprintf(buffer, "%d", hh);

    if(colon_status)
         buffer[2] = ':';
    else buffer[2] = ' ';

    if(mm<10)   sprintf(&buffer[3], "0%d", mm);
    else        sprintf(&buffer[3], "%d", mm);

    print((uint8_t*)buffer);
}


void WDsDisplay::Colon_toogle(void){
    colon_status = not colon_status;
    setTextSize(clock_textSize);
    setTextColor(clock_textColor);
    setCursor(clock_cursor[0] + 6*clock_textSize*2, clock_cursor[1]);
    clear_18x24(clock_cursor[0] +6*clock_textSize*2, clock_cursor[1], 0);
    if(colon_status) print(':');
    else print(' ');
}

void WDsDisplay::Heartrate_status(void){
    clear_heart(heart_x, heart_y, 0);

    switch(heart_status){
    case 0: drawBitmap(heart_x, heart_y, cardiogram , sizeBigHeart[0], sizeBigHeart[1], 1, 0); break;
    case 1: drawBitmap(heart_x, heart_y, cardiogram1, sizeBigHeart[0], sizeBigHeart[1], 1, 0); break;
    case 2: drawBitmap(heart_x, heart_y, cardiogram2, sizeBigHeart[0], sizeBigHeart[1], 1, 0); break;
    case 3: drawBitmap(heart_x, heart_y, cardiogram3, sizeBigHeart[0], sizeBigHeart[1], 1, 0); break;
    default: heart_status=0; break;
    }
    heart_status++;
    heart_status = heart_status%4;
}


void WDsDisplay::Heartrate_number(int8_t number){ // -1 is wait
    char buffer[3] = "";
    uint8_t padding = 10;
    clear_character(heart_x + sizeBigHeart[0] + padding, heart_y + padding, 4, 3, 0);
    setTextSize(clock_textSize);
    setTextColor(clock_textColor);

    if (number<0){
        buffer[0] = buffer[1] = buffer[2] = '_';
        setCursor(heart_x + sizeBigHeart[0] + padding, heart_y + padding);
        print((uint8_t*)buffer);
        return;
    }
    sprintf(buffer, "%d", number);
    int8_t stringlen = strlen(buffer);
    switch(stringlen){
    case 1: setCursor(heart_x + sizeBigHeart[0] + padding + (3*font_width)*1, heart_y + padding); break;
    case 2: setCursor(heart_x + sizeBigHeart[0] + padding + (3*font_width)*1, heart_y + padding); break;
    case 3: setCursor(heart_x + sizeBigHeart[0] + padding, heart_y + padding); break;
    default: break;
    }
    print((uint8_t*)buffer);
    return;
}

void WDsDisplay::Footsteps_status(void){
    footsteps_status = not footsteps_status;
    clear_heart(footsteps_x, footsteps_y, 0);
    if(footsteps_status)
        drawBitmap(footsteps_x, footsteps_y, footsteps , sizeFootsteps[0], sizeFootsteps[1], 1, 0);
    else
        drawBitmap(footsteps_x, footsteps_y, footsteps1, sizeFootsteps[0], sizeFootsteps[1], 1, 0);
}


void WDsDisplay::Footsteps_number(uint32_t number){
    char buffer[6] = "";
    sprintf(buffer, "%d", number);
    uint8_t padding = 10;
    uint8_t padding_y = 20;
    clear_character(heart_x + sizeBigHeart[0] + padding, heart_y + padding_y, 4, 3, 0);
    setTextSize(2);
    setTextColor(clock_textColor);

    int8_t stringlen = strlen(buffer);
    switch(stringlen){
        case 1: setCursor(heart_x + sizeBigHeart[0] + padding + (2*font_width)*2, heart_y + padding_y); break;
        case 2: setCursor(heart_x + sizeBigHeart[0] + padding + (2*font_width)*2, heart_y + padding_y); break;
        case 3: setCursor(heart_x + sizeBigHeart[0] + padding + (2*font_width)*1, heart_y + padding_y); break;
        case 4: setCursor(heart_x + sizeBigHeart[0] + padding + (2*font_width)*1, heart_y + padding_y); break;
        case 5: setCursor(heart_x + sizeBigHeart[0] + padding, heart_y + padding_y); break;
        default: break;
    }
    print((uint8_t*)buffer);
}


void WDsDisplay::Display_head(void){
    display(0, 1);
}


void WDsDisplay::Display_body(void){
    display(2, 7);
}


void WDsDisplay::Clear_head(void){
    uint8_t* pointer =  getbuffer();
    memset(pointer, 0, SSD1306_LCDWIDTH*2);
}


void WDsDisplay::Clear_body(void){
    uint8_t* pointer =  getbuffer();
    memset(pointer+SSD1306_LCDWIDTH*2, 0, (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8) - SSD1306_LCDWIDTH*2);
}




/* Private class */

void WDsDisplay::clear_16x16(int16_t x, int16_t y, uint16_t color){
    fillRect(x, y, 16, 16, color);
}

void WDsDisplay::clear_18x24(int16_t x, int16_t y, uint16_t color){
    fillRect(x, y, 18, 24, color);
}

void WDsDisplay::clear_heart(int16_t x, int16_t y, uint16_t color){
    fillRect(x, y, 48, 48, color);
}

void WDsDisplay::clear_character(int16_t x, int16_t y, uint16_t fontsize, uint16_t length, uint16_t color){
    uint8_t fontSize[2] = {6, 8};
    fillRect(x, y, fontSize[0]*fontsize*length, fontSize[1]*fontsize, color);

}
