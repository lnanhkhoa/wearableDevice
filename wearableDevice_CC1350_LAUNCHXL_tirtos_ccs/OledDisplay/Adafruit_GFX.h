#ifndef _AdafruitGFX_H
#define _AdafruitGFX_H

#include <stdint.h>
#include "gfxfont.h"

#define boolean bool

  void AdafruitGFX_init(int16_t w, int16_t h); // Constructor

  void (*P_drawPixel)(int16_t , int16_t , uint16_t );

  // TRANSACTION API / CORE DRAW API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.

  /* Pointer to function*/
  void (*P_startWrite)(void);
  void (*P_writePixel)(int16_t, int16_t , uint16_t );
  void (*P_writeFillRect)(int16_t , int16_t , int16_t , int16_t , uint16_t );
  void (*P_writeFastVLine)(int16_t , int16_t , int16_t , uint16_t );
  void (*P_writeFastHLine)(int16_t , int16_t , int16_t , uint16_t );
  void (*P_writeLine)(int16_t , int16_t , int16_t , int16_t , uint16_t );
  void (*P_endWrite)(void);

  void AdafruitGFX_startWrite(void);
  void AdafruitGFX_writePixel(int16_t x, int16_t y, uint16_t color);
  void AdafruitGFX_writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void AdafruitGFX_writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  void AdafruitGFX_writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  void AdafruitGFX_writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  void AdafruitGFX_endWrite(void);

  // CONTROL API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  void (*PsetRotation)(uint8_t);
  void AdafruitGFX_setRotation(uint8_t r);
//  virtual void invertDisplay(boolean i);

  // BASIC DRAW API
  // These MAY be overridden by the subclass to provide device-specific

  /* Pointer to function*/
  void
    (*P_drawFastVLine)(int16_t , int16_t , int16_t , uint16_t ),
    (*P_drawFastHLine)(int16_t , int16_t , int16_t , uint16_t ),
    (*P_fillRect)(int16_t , int16_t , int16_t , int16_t , uint16_t ),
    (*P_fillScreen)(uint16_t ),
    (*P_drawLine)(int16_t , int16_t , int16_t , int16_t , uint16_t ),
    (*P_drawRect)(int16_t , int16_t , int16_t , int16_t , uint16_t );
  // optimized code.  Otherwise 'generic' versions are used.
  void
    // It's good to implement those, even if using transaction API
    AdafruitGFX_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
    AdafruitGFX_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
    AdafruitGFX_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
    AdafruitGFX_fillScreen(uint16_t color),
    // Optional and probably not necessary to change
    AdafruitGFX_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
    AdafruitGFX_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

  // These exist only with AdafruitGFX (no subclass overrides)
  void
    AdafruitGFX_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    AdafruitGFX_drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color),
    AdafruitGFX_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    AdafruitGFX_fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color),
    
    AdafruitGFX_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color),
    AdafruitGFX_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color),
    AdafruitGFX_drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color),
    AdafruitGFX_fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color),
    
    (*P_drawBitmap)(int16_t , int16_t , const unsigned char *, int16_t , int16_t , uint16_t );

    AdafruitGFX_drawBitmap(int16_t x, int16_t y, const unsigned char bitmap[], int16_t w, int16_t h, uint16_t color),
    // AdafruitGFX_drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg),
    // AdafruitGFX_drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color),
    // AdafruitGFX_drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg),
    // AdafruitGFX_drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color),
    
    AdafruitGFX_drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h),
    // AdafruitGFX_drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h),
    // AdafruitGFX_drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h),
    // AdafruitGFX_drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h),
    
    AdafruitGFX_drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h),
    // AdafruitGFX_drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h),
    // AdafruitGFX_drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], const uint8_t mask[], int16_t w, int16_t h),
    // AdafruitGFX_drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h),
    
    (*P_setCursor)(int16_t , int16_t ),
    (*P_setTextColor)(uint16_t ),
    (*P_setTextSize)(uint8_t ),


    AdafruitGFX_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size),
    AdafruitGFX_setCursor(int16_t x, int16_t y),
    AdafruitGFX_setTextColor(uint16_t c),
    AdafruitGFX_setTextSize(uint8_t s),
    AdafruitGFX_setTextWrap(boolean w),
    AdafruitGFX_cp437(boolean x=true),
    AdafruitGFX_setFont(const GFXfont *f = 0),
    AdafruitGFX_getTextBounds(char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
   // AdafruitGFX_charBounds(const __FlashStringHelper *s, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);

    void (*P_write)(uint8_t);
    void AdafruitGFX_write(uint8_t c);

    void AdafruitGFX_print(uint8_t *str);
    // void AdafruitGFX_print(char c);
    int16_t AdafruitGFX_height(void) const;
    int16_t AdafruitGFX_width(void) const;

    uint8_t AdafruitGFX_getRotation(void) const;

    // get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
    int16_t AdafruitGFX_getCursorX(void) const;
    int16_t AdafruitGFX_getCursorY(void) const;

    void AdafruitGFX_charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);
  

#endif // _AdafruitGFX_H
