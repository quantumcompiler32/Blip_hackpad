# Debug & ensure everything runs properly

- [x] Step 1: Fix/verify HID keycode handling in `src/main.cpp` (including F13-F24 USB translation).
- [x] Step 2: Improve diagnostics in `src/main.cpp` to distinguish no I2C ACK from init failure.
- [x] Step 3: Fix reliability issues in `src/CharacterOled.cpp` (US2066 data/command init sequence).
- [x] Step 4: Use ArduinoJson 7's `JsonDocument` and apply parsed configuration atomically.
- [x] Step 5: Clean-build successfully in PlatformIO for `seeed_xiao_esp32s3`.
- [ ] Step 6: Flash and runtime test: startup diagnostics, HID inputs, serial commands.
