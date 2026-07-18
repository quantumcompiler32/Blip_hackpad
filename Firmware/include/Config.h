#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>

constexpr uint8_t PROFILE_COUNT = 3;
constexpr uint8_t KEY_COUNT = 12;
constexpr size_t ACTION_LENGTH = 24;

struct Profile {
  char name[17];
  char actions[KEY_COUNT][ACTION_LENGTH];
};

class ConfigStore {
 public:
  void begin();
  bool save();
  void reset();
  String toJson() const;
  bool fromJson(const String& json, String& error);

  Profile profiles[PROFILE_COUNT];
  uint8_t activeProfile = 0;
  char deviceName[25] = "Blip Hackpad";

 private:
  void loadDefaults();
  Preferences preferences;
};

