#include "Config.h"

#include <cstring>

#include <Arduino.h>

namespace {
const char* DEFAULT_ACTIONS[PROFILE_COUNT][KEY_COUNT] = {
    {"CTRL_C", "CTRL_V", "CTRL_X", "CTRL_Z", "CTRL_Y", "CTRL_A",
     "ENTER", "ESC", "TAB", "ARROW_LEFT", "ARROW_DOWN", "ARROW_RIGHT"},
    {"F13", "F14", "F15", "F16", "F17", "F18",
     "F19", "F20", "F21", "F22", "F23", "F24"},
    {"MEDIA_PREVIOUS", "MEDIA_PLAY_PAUSE", "MEDIA_NEXT", "MEDIA_STOP", "MUTE", "VOLUME_UP",
     "HOME", "PAGE_UP", "END", "ARROW_LEFT", "ARROW_DOWN", "ARROW_RIGHT"}};
const char* DEFAULT_NAMES[PROFILE_COUNT] = {"Editing", "Shortcuts", "Media"};

void copyText(char* destination, size_t size, const char* source) {
  strlcpy(destination, source ? source : "NONE", size);
}
}  // namespace

void ConfigStore::loadDefaults() {
  activeProfile = 0;
  copyText(deviceName, sizeof(deviceName), "Blip Hackpad");
  for (uint8_t p = 0; p < PROFILE_COUNT; ++p) {
    copyText(profiles[p].name, sizeof(profiles[p].name), DEFAULT_NAMES[p]);
    for (uint8_t k = 0; k < KEY_COUNT; ++k) {
      copyText(profiles[p].actions[k], ACTION_LENGTH, DEFAULT_ACTIONS[p][k]);
    }
  }
}

void ConfigStore::begin() {
  loadDefaults();
  preferences.begin("blip", false);
  String stored = preferences.getString("config", "");
  if (!stored.isEmpty()) {
    String error;
    if (!fromJson(stored, error)) Serial.printf("Config load failed: %s\n", error.c_str());
  }
}

bool ConfigStore::save() {
  return preferences.putString("config", toJson()) > 0;
}

void ConfigStore::reset() {
  preferences.clear();
  loadDefaults();
  save();
}

String ConfigStore::toJson() const {
  JsonDocument doc;

  doc["deviceName"] = deviceName;
  doc["activeProfile"] = activeProfile;

  JsonArray outputProfiles = doc["profiles"].to<JsonArray>();
  for (uint8_t p = 0; p < PROFILE_COUNT; ++p) {
    JsonObject profile = outputProfiles.add<JsonObject>();
    profile["name"] = profiles[p].name;

    JsonArray actions = profile["actions"].to<JsonArray>();
    for (uint8_t k = 0; k < KEY_COUNT; ++k) actions.add(profiles[p].actions[k]);
  }

  String output;
  serializeJson(doc, output);
  return output;
}

bool ConfigStore::fromJson(const String& json, String& error) {
  JsonDocument doc;
  DeserializationError result = deserializeJson(doc, json);
  if (result) {
    error = result.c_str();
    return false;
  }

  JsonArray inputProfiles = doc["profiles"].as<JsonArray>();
  if (inputProfiles.size() != PROFILE_COUNT) {
    error = "exactly 3 profiles are required";
    return false;
  }

  char newDeviceName[sizeof(deviceName)];
  Profile newProfiles[PROFILE_COUNT];
  copyText(newDeviceName, sizeof(newDeviceName), doc["deviceName"] | "Blip Hackpad");
  uint8_t newActiveProfile = constrain(doc["activeProfile"] | 0, 0, PROFILE_COUNT - 1);

  for (uint8_t p = 0; p < PROFILE_COUNT; ++p) {
    JsonObject profile = inputProfiles[p];
    JsonArray actions = profile["actions"].as<JsonArray>();
    if (actions.size() != KEY_COUNT) {
      error = "each profile must contain exactly 12 actions";
      return false;
    }

    copyText(newProfiles[p].name, sizeof(newProfiles[p].name), profile["name"] | "Profile");
    for (uint8_t k = 0; k < KEY_COUNT; ++k) {
      if (!actions[k].is<const char*>()) {
        error = "every action must be a string";
        return false;
      }
      copyText(newProfiles[p].actions[k], ACTION_LENGTH, actions[k].as<const char*>());
    }
  }

  copyText(deviceName, sizeof(deviceName), newDeviceName);
  activeProfile = newActiveProfile;
  memcpy(profiles, newProfiles, sizeof(profiles));

  return true;
}
