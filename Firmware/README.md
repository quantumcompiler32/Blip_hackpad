# Blip firmware

This folder is a self-contained PlatformIO project. Source files are in `src/`,
headers are in `include/`, and the build configuration is `platformio.ini`.

## Controls

- Left/volume encoder: rotate for volume, press for mute.
- Right/profile encoder: rotate through three profiles, press to save/select.
- The macropad communicates and receives configuration only through USB.

Key actions and profile names are saved in ESP32 flash and can be changed over
USB serial without rebuilding the firmware. Wi-Fi and Bluetooth are disabled.

## Build and flash

1. Install [Visual Studio Code](https://code.visualstudio.com/) and the
   PlatformIO extension.
2. Open the repository's `Firmware` folder as the PlatformIO project.
3. Connect the XIAO ESP32-S3 by USB.
4. Run **PlatformIO: Upload**.
5. Open the serial monitor at 115200 baud for diagnostics.

Command-line users can run:

```sh
cd Firmware
pio run
pio run --target upload
pio device monitor
```

## Hardware requirements and limitations

- Newhaven pin 16 (`/RES`) must be pulled up to 3.3 V. The current schematic
  ties it to ground, which permanently resets the display.
- Newhaven pin 4 (`SA0`) must be tied to 3.3 V, selecting address `0x3D`.
  The 128x64 module remains at `0x3C`. Two devices at the same I2C address
  cannot be separated by firmware or by automatic address detection.
- This firmware requires a XIAO ESP32-S3 for native USB HID support.

## Serial commands

- `status` — rerun the I2C and peripheral report
- `config` — print the stored JSON configuration
- `set-config <JSON>` — validate and save a complete configuration
- `reset-config` — restore the example profiles
- `reboot` — restart the controller
