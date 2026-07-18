#pragma once

#include <Arduino.h>
#include <Wire.h>

class CharacterOled {
 public:
  bool begin(TwoWire& wire, uint8_t address);
  void clear();
  void setCursor(uint8_t column, uint8_t row);
  void printPadded(uint8_t row, const String& text);
  bool ready() const { return available; }

 private:
  void command(uint8_t value);
  void data(uint8_t value);
  TwoWire* bus = nullptr;
  uint8_t i2cAddress = 0;
  bool available = false;
};

