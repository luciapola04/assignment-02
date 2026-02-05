#include "devices/LcdImpl.h"

LcdImpl::LcdImpl(uint8_t addr, uint8_t cols, uint8_t rows) : lcd(addr, cols, rows) {
  lcd.init();
  lcd.backlight();
}

void LcdImpl::print(const String& msg) {
  lcd.print(msg);
}

void LcdImpl::setCursor(int col, int row) {
  lcd.setCursor(col, row);
}

void LcdImpl::clear() {
  lcd.clear();
}