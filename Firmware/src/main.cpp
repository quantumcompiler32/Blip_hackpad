#include <Adafruit_MCP23X17.h>
#include <Adafruit_SSD1306.h>
#include <USB.h>
#include <USBHIDConsumerControl.h>
#include <USBHIDKeyboard.h>
#include <Wire.h>

#include "CharacterOled.h"
#include "Config.h"

// PCB net mapping from Hackpad.kicad_sch.
constexpr uint8_t SDA_PIN = D4;
constexpr uint8_t SCL_PIN = D5;
constexpr uint8_t ENCODER1_A = D9;
constexpr uint8_t ENCODER1_B = D10;
constexpr uint8_t ENCODER1_SW = D8;
constexpr uint8_t ENCODER2_A = D0;
constexpr uint8_t ENCODER2_B = D1;
constexpr uint8_t ENCODER2_SW = D2;
constexpr uint8_t MCP_ADDRESS = 0x20;
constexpr uint8_t GRAPHIC_ADDRESS = 0x3C;
constexpr uint8_t CHARACTER_ADDRESS = 0x3D;
constexpr uint16_t DEBOUNCE_MS = 20;
constexpr uint16_t DISPLAY_REFRESH_MS = 150;
constexpr uint16_t LONG_PRESS_MS = 3000;

Adafruit_MCP23X17 mcp;
Adafruit_SSD1306 graphic(128, 64, &Wire, -1);
CharacterOled character;
ConfigStore config;
USBHIDKeyboard keyboard;
USBHIDConsumerControl consumer;

bool mcpReady = false;
bool graphicReady = false;
bool characterReady = false;
bool configDirty = false;
String lastAction = "Ready";
uint32_t lastDisplayRefresh = 0;

struct KeyState {
  bool raw = false;
  bool stable = false;
  uint32_t changedAt = 0;
};
KeyState keyStates[KEY_COUNT];

struct Encoder {
  Encoder(uint8_t a, uint8_t b, uint8_t button)
      : pinA(a), pinB(b), pinSwitch(button) {}

  uint8_t pinA;
  uint8_t pinB;
  uint8_t pinSwitch;
  uint8_t previousAB = 0;
  int8_t accumulator = 0;
  bool rawSwitch = false;
  bool stableSwitch = false;
  bool longPressHandled = false;
  uint32_t switchChangedAt = 0;
  uint32_t pressedAt = 0;
};
Encoder volumeEncoder{ENCODER1_A, ENCODER1_B, ENCODER1_SW};
Encoder profileEncoder{ENCODER2_A, ENCODER2_B, ENCODER2_SW};

const uint8_t KEY_CODES[] = {
    0, KEY_RETURN, KEY_ESC, KEY_TAB, ' ', KEY_BACKSPACE, KEY_DELETE,
    KEY_HOME, KEY_END, KEY_PAGE_UP, KEY_PAGE_DOWN, KEY_UP_ARROW,
    KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW};
const char* KEY_NAMES[] = {
    "NONE", "ENTER", "ESC", "TAB", "SPACE", "BACKSPACE", "DELETE",
    "HOME", "END", "PAGE_UP", "PAGE_DOWN", "ARROW_UP", "ARROW_DOWN",
    "ARROW_LEFT", "ARROW_RIGHT"};

bool i2cPresent(uint8_t address) {
  Wire.beginTransmission(address);
  return Wire.endTransmission() == 0;
}

String scanI2c() {
  String found;
  for (uint8_t address = 1; address < 127; ++address) {
    if (!i2cPresent(address)) continue;
    if (!found.isEmpty()) found += ", ";
    char value[7];
    snprintf(value, sizeof(value), "0x%02X", address);
    found += value;
  }
  return found.isEmpty() ? "none" : found;
}

void reportDiagnostics() {
  bool graphicAck = i2cPresent(GRAPHIC_ADDRESS);
  bool characterAck = i2cPresent(CHARACTER_ADDRESS);
  Serial.println("\n=== Blip startup diagnostics ===");
  Serial.printf("I2C devices: %s\n", scanI2c().c_str());
  Serial.printf("MCP23017 0x20: %s\n", mcpReady ? "OK" : "MISSING");
  Serial.printf("128x64 OLED 0x3C: %s\n",
                graphicReady ? "OK" : (graphicAck ? "ACK, INIT FAILED" : "NO ACK"));
  Serial.printf("20x4 OLED 0x3D: %s\n",
                characterReady ? "OK" : (characterAck ? "ACK, INIT FAILED" : "NO ACK"));
  if (graphicAck && !characterAck) {
    Serial.println("WARNING: Newhaven display may still be at 0x3C; set its SA0 pin to 3.3V.");
  }
  Serial.println("USB HID: enabled (XIAO ESP32-S3)");
  Serial.println("Wi-Fi and Bluetooth: disabled");
  Serial.println("Commands: status | config | set-config <JSON> | reset-config | reboot");
  Serial.println("================================\n");
}

void tapMedia(uint16_t usage, const char* label) {
  if (usage) { consumer.press(usage); consumer.release(); }
  lastAction = label;
}

uint8_t functionKeyCode(uint8_t number) {
  if (number >= 1 && number <= 12) return KEY_F1 + number - 1;
  if (number >= 13 && number <= 24) return KEY_F13 + number - 13;
  return 0;
}

void executeAction(const char* action) {
  String name(action);
  name.toUpperCase();
  lastAction = name;
  if (name == "NONE") return;

  bool mediaAction = true;
  if (name == "MUTE") { consumer.press(CONSUMER_CONTROL_MUTE); consumer.release(); }
  else if (name == "VOLUME_UP") { consumer.press(CONSUMER_CONTROL_VOLUME_INCREMENT); consumer.release(); }
  else if (name == "VOLUME_DOWN") { consumer.press(CONSUMER_CONTROL_VOLUME_DECREMENT); consumer.release(); }
  else if (name == "MEDIA_PLAY_PAUSE") { consumer.press(CONSUMER_CONTROL_PLAY_PAUSE); consumer.release(); }
  else if (name == "MEDIA_NEXT") { consumer.press(CONSUMER_CONTROL_SCAN_NEXT); consumer.release(); }
  else if (name == "MEDIA_PREVIOUS") { consumer.press(CONSUMER_CONTROL_SCAN_PREVIOUS); consumer.release(); }
  else if (name == "MEDIA_STOP") { consumer.press(CONSUMER_CONTROL_STOP); consumer.release(); }
  else mediaAction = false;
  if (mediaAction) return;

  if (name.length() == 1) {
    keyboard.write(static_cast<uint8_t>(name[0]));
    return;
  }
  if (name.startsWith("CTRL_") && name.length() == 6) {
    keyboard.press(KEY_LEFT_CTRL);
    keyboard.press(static_cast<uint8_t>(tolower(name[5])));
    delay(5);
    keyboard.releaseAll();
    return;
  }
  if (name[0] == 'F') {
    uint8_t code = functionKeyCode(name.substring(1).toInt());
    if (code) keyboard.write(code);
    return;
  }
  for (size_t i = 0; i < sizeof(KEY_CODES) / sizeof(KEY_CODES[0]); ++i) {
    if (name == KEY_NAMES[i]) {
      keyboard.write(KEY_CODES[i]);
      return;
    }
  }
  lastAction = "Unknown: " + name;
}

void selectProfile(int8_t direction) {
  int next = config.activeProfile + direction;
  if (next < 0) next = PROFILE_COUNT - 1;
  if (next >= PROFILE_COUNT) next = 0;
  config.activeProfile = next;
  configDirty = true;
  lastAction = String("Profile: ") + config.profiles[next].name;
}

void setupEncoder(Encoder& encoder) {
  pinMode(encoder.pinA, INPUT_PULLUP);
  pinMode(encoder.pinB, INPUT_PULLUP);
  pinMode(encoder.pinSwitch, INPUT_PULLUP);
  encoder.previousAB = (digitalRead(encoder.pinA) << 1) | digitalRead(encoder.pinB);
}

int8_t readEncoderRotation(Encoder& encoder) {
  static const int8_t transitions[16] = {0, -1, 1, 0, 1, 0, 0, -1,
                                          -1, 0, 0, 1, 0, 1, -1, 0};
  uint8_t current = (digitalRead(encoder.pinA) << 1) | digitalRead(encoder.pinB);
  encoder.accumulator += transitions[(encoder.previousAB << 2) | current];
  encoder.previousAB = current;
  if (encoder.accumulator >= 4) {
    encoder.accumulator = 0;
    return 1;
  }
  if (encoder.accumulator <= -4) {
    encoder.accumulator = 0;
    return -1;
  }
  return 0;
}

enum class SwitchEvent { NONE, PRESSED, RELEASED, LONG_PRESS };
SwitchEvent readEncoderSwitch(Encoder& encoder) {
  bool raw = digitalRead(encoder.pinSwitch) == LOW;
  uint32_t now = millis();
  if (raw != encoder.rawSwitch) {
    encoder.rawSwitch = raw;
    encoder.switchChangedAt = now;
  }
  if (raw != encoder.stableSwitch && now - encoder.switchChangedAt >= DEBOUNCE_MS) {
    encoder.stableSwitch = raw;
    if (raw) {
      encoder.pressedAt = now;
      encoder.longPressHandled = false;
      return SwitchEvent::PRESSED;
    }
    return SwitchEvent::RELEASED;
  }
  if (encoder.stableSwitch && !encoder.longPressHandled &&
      now - encoder.pressedAt >= LONG_PRESS_MS) {
    encoder.longPressHandled = true;
    return SwitchEvent::LONG_PRESS;
  }
  return SwitchEvent::NONE;
}

void scanMatrix() {
  if (!mcpReady) return;
  uint32_t now = millis();
  for (uint8_t row = 0; row < 4; ++row) {
    for (uint8_t r = 0; r < 4; ++r) mcp.digitalWrite(r, HIGH);
    mcp.digitalWrite(row, LOW);
    delayMicroseconds(50);
    for (uint8_t column = 0; column < 3; ++column) {
      uint8_t index = row * 3 + column;
      bool pressed = mcp.digitalRead(8 + column) == LOW;
      KeyState& state = keyStates[index];
      if (pressed != state.raw) {
        state.raw = pressed;
        state.changedAt = now;
      }
      if (pressed != state.stable && now - state.changedAt >= DEBOUNCE_MS) {
        state.stable = pressed;
        if (pressed) executeAction(config.profiles[config.activeProfile].actions[index]);
      }
    }
  }
  for (uint8_t r = 0; r < 4; ++r) mcp.digitalWrite(r, HIGH);
}

void updateDisplays(bool force = false) {
  uint32_t now = millis();
  if (!force && now - lastDisplayRefresh < DISPLAY_REFRESH_MS) return;
  lastDisplayRefresh = now;
  bool connected = true;

  if (characterReady) {
    character.printPadded(0, "BLIP // " + String(config.profiles[config.activeProfile].name));
    character.printPadded(1, "USB: CONNECTED");
    character.printPadded(2, "LAST: " + lastAction.substring(0, 14));
    character.printPadded(3, "CFG: USB SERIAL");
  }
  if (graphicReady) {
    graphic.clearDisplay();
    graphic.drawRoundRect(0, 0, 128, 64, 7, SSD1306_WHITE);
    graphic.fillRect(0, 0, 128, 14, SSD1306_WHITE);
    graphic.setTextColor(SSD1306_BLACK);
    graphic.setTextSize(1);
    graphic.setCursor(6, 3);
    graphic.print("BLIP STATUS");
    graphic.setTextColor(SSD1306_WHITE);
    graphic.setCursor(7, 21);
    graphic.print("PROFILE ");
    graphic.print(config.activeProfile + 1);
    graphic.print("/3");
    graphic.setCursor(7, 34);
    graphic.print(config.profiles[config.activeProfile].name);
    graphic.setCursor(7, 49);
    graphic.print("USB LINKED");
    // Small connection glyph.
    graphic.drawCircle(115, 48, 7, SSD1306_WHITE);
    if (connected) graphic.fillCircle(115, 48, 3, SSD1306_WHITE);
    graphic.display();
  }
}

void handleSerial() {
  if (!Serial.available()) return;
  String command = Serial.readStringUntil('\n');
  command.trim();
  if (command == "status") reportDiagnostics();
  else if (command == "config") Serial.println(config.toJson());
  else if (command.startsWith("set-config ")) {
    String error;
    if (config.fromJson(command.substring(11), error) && config.save())
      Serial.println("Configuration saved.");
    else Serial.println("Configuration failed: " + error);
  }
  else if (command == "reset-config") {
    config.reset();
    Serial.println("Defaults restored.");
  } else if (command == "reboot") ESP.restart();
  else Serial.println("Unknown command. Try: status | config | set-config <JSON> | reset-config | reboot");
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(20);
  delay(400);
  config.begin();
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  mcpReady = mcp.begin_I2C(MCP_ADDRESS, &Wire);
  if (mcpReady) {
    for (uint8_t row = 0; row < 4; ++row) {
      mcp.pinMode(row, OUTPUT);
      mcp.digitalWrite(row, HIGH);
    }
    for (uint8_t column = 0; column < 3; ++column) mcp.pinMode(8 + column, INPUT_PULLUP);
  }
  graphicReady = i2cPresent(GRAPHIC_ADDRESS) &&
                 graphic.begin(SSD1306_SWITCHCAPVCC, GRAPHIC_ADDRESS, false, false);
  characterReady = character.begin(Wire, CHARACTER_ADDRESS);

  setupEncoder(volumeEncoder);
  setupEncoder(profileEncoder);
  keyboard.begin();
  consumer.begin();
  USB.productName("Blip Hackpad");
  USB.manufacturerName("Blip");
  USB.begin();
  reportDiagnostics();
  updateDisplays(true);
}

void loop() {
  scanMatrix();

  int8_t volumeStep = readEncoderRotation(volumeEncoder);
  if (volumeStep > 0) tapMedia(CONSUMER_CONTROL_VOLUME_INCREMENT, "Volume up");
  else if (volumeStep < 0) tapMedia(CONSUMER_CONTROL_VOLUME_DECREMENT, "Volume down");
  SwitchEvent volumeSwitch = readEncoderSwitch(volumeEncoder);
  if (volumeSwitch == SwitchEvent::RELEASED && !volumeEncoder.longPressHandled) {
    tapMedia(CONSUMER_CONTROL_MUTE, "Mute");
  }

  int8_t profileStep = readEncoderRotation(profileEncoder);
  if (profileStep != 0) selectProfile(profileStep);
  SwitchEvent profileSwitch = readEncoderSwitch(profileEncoder);
  if (profileSwitch == SwitchEvent::RELEASED && !profileEncoder.longPressHandled) {
    lastAction = String("Selected: ") + config.profiles[config.activeProfile].name;
    if (configDirty) {
      config.save();
      configDirty = false;
    }
  } else if (profileSwitch == SwitchEvent::LONG_PRESS) lastAction = "USB config ready";

  handleSerial();
  updateDisplays();
  delay(1);
}
