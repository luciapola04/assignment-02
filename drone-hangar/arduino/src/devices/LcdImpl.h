#ifndef __LCDIMPL__
#define __LCDIMPL__

#include "Lcd.h"
#include <LiquidCrystal_I2C.h>

class LcdImpl: public Lcd {
public:
  LcdImpl(uint8_t addr, uint8_t cols, uint8_t rows);
  void print(const String& msg);
  void setCursor(int col, int row);
  void clear();

private:
  LiquidCrystal_I2C lcd;
};

#endif