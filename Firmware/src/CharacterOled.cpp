#include "CharacterOled.h"

void CharacterOled::command(uint8_t value) {
  bus->beginTransmission(i2cAddress);
  bus->write(0x00);
  bus->write(value);
  if (bus->endTransmission() != 0) available = false;
  delayMicroseconds(40);
}

void CharacterOled::data(uint8_t value) {
  bus->beginTransmission(i2cAddress);
  bus->write(0x40);
  bus->write(value);
  if (bus->endTransmission() != 0) available = false;
}

bool CharacterOled::begin(TwoWire& wire, uint8_t address) {
  bus = &wire;
  i2cAddress = address;
  bus->beginTransmission(address);
  available = bus->endTransmission() == 0;
  if (!available) return false;

  delay(100);
  // US2066 initialization for 20x4 character OLED, I2C mode.
  struct InitStep { uint8_t value; bool isData; };
  const InitStep sequence[] = {
      {0x2A, false}, {0x71, false}, {0x00, true},  {0x28, false},
      {0x08, false}, {0x2A, false}, {0x79, false}, {0xD5, false},
      {0x70, false}, {0x78, false}, {0x09, false}, {0x06, false},
      {0x72, false}, {0x00, true},  {0x2A, false}, {0x79, false},
      {0xDA, false}, {0x10, false}, {0xDC, false}, {0x00, false},
      {0x81, false}, {0x7F, false}, {0xD9, false}, {0xF1, false},
      {0xDB, false}, {0x40, false}, {0x78, false}, {0x28, false},
      {0x01, false}, {0x80, false}, {0x0C, false}};
  for (const InitStep& step : sequence) {
    if (step.isData) data(step.value);
    else command(step.value);
    if (!available) return false;
    delay(2);
  }
  return true;
}

void CharacterOled::clear() {
  if (available) {
    command(0x01);
    delay(2);
  }
}

void CharacterOled::setCursor(uint8_t column, uint8_t row) {
  static const uint8_t offsets[] = {0x00, 0x20, 0x40, 0x60};
  if (available && row < 4 && column < 20) command(0x80 | (offsets[row] + column));
}

void CharacterOled::printPadded(uint8_t row, const String& text) {
  if (!available || row > 3) return;
  setCursor(0, row);
  for (uint8_t i = 0; i < 20; ++i) data(i < text.length() ? text[i] : ' ');
}
