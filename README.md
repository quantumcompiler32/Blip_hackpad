# Blip_hackpad

A macropad project it has a 3x4 key matrix, rotary encoder input, and a 20x4 OLED display.

## Features

- 12-key (3x4) mechanical switch matrix
- 1N4148 diode matrix for anti-ghosting
- Seeed Studio XIAO ESP32-S3 controller for native USB HID
- MCP23017 I2C GPIO expander for additional IO
- Newhaven 20x4 character OLED display
- 2 rotary encoder support in hardware design

## Bill of Materials (BOM)

| Name | Purpose | Qty | Unit Cost (USD) | Total Cost (USD) | Distributor | Manufacturer | MPN |
|---|---|---:|---:|---:|---|---|---|
| Seeed Studio XIAO ESP32-S3 | Native USB HID microcontroller | 1 | 7.49 | 7.49 | DigiKey | Seeed Studio | 113991114 |
| Newhaven 20x4 Character OLED Display | Primary 20x4 character display | 1 | 36.27 | 36.27 | DigiKey | Newhaven Display International | NHD-0420CW-AB3 |
| 0.96-inch 128x64 I2C OLED Module | Secondary compact graphics display | 1 | 4.90 | 4.90 | DisplayModule | DisplayModule | DM-OLED096-636 |
| MCP23017 16-bit I2C GPIO Expander, SPDIP-28 | Expands available GPIO pins | 1 | 1.69 | 1.69 | DigiKey | Microchip Technology | MCP23017-E/SP |
| Alps Alpine EC11E Rotary Encoder with Push Switch | Rotary input controls | 2 | 5.00 | 10.00 | DigiKey | Alps Alpine | EC11E09244AQ |
| Cherry MX PCB-mount Mechanical Keyswitch | 3x4 key matrix switches | 12 | 1.75 | 21.00 | DigiKey | Cherry Americas | MX2A-E1NW |
| 1N4148 Through-hole Switching Diode | Prevents matrix ghosting | 12 | 0.10 | 1.20 | DigiKey | onsemi | 1N4148 |
| 4.7 kΩ 1% 0805 Resistor | I2C pull-up resistors | 2 | 0.10 | 0.20 | DigiKey | YAGEO | RC0805FR-074K7L |
| 10 kΩ 1% 0805 Resistor | Display/control pull-up resistor | 1 | 0.10 | 0.10 | DigiKey | YAGEO | RC0805FR-0710KL |
| 100 nF 50 V X7R 0805 Ceramic Capacitor | 3.3V decoupling | 1 | 0.11 | 0.11 | DigiKey | YAGEO | CC0805KRX7R9BB104 |

> Full BOM: [`BOM/Hackpad_final_BOM.csv`](./BOM/Hackpad_final_BOM.csv).

## Repository Structure

```text
Blip_hackpad/
├── BOM/        Bill of materials
├── Images/     Schematic, PCB, and build pictures
├── PCB/        KiCad design files and manufacturing outputs
├── Firmware/   Complete PlatformIO project, source code, headers, and docs
└── README.md   Project overview
```

Add future code and software files under `Firmware/`, BOM files under `BOM/`, pictures
under `Images/`, and all KiCad/Gerber/PCB-related files under `PCB/`.

## Build Media

- [`Images/PCBSchematic.png`](./Images/PCBSchematic.png)
- [`Images/PCBFront.png`](./Images/PCBFront.png)
- `Images/build-1.png`
- `Images/build-2.png`

## Firmware

PlatformIO firmware is included for the XIAO ESP32-S3. It provides USB HID,
three flash-backed profiles, volume/mute and profile encoder controls, both OLED
interfaces, I2C diagnostics, and configuration over USB serial. It uses neither
Wi-Fi nor Bluetooth.

Open `Firmware/` as the PlatformIO project. See
[`Firmware/README.md`](./Firmware/README.md) for flashing instructions and
important PCB corrections required for the Newhaven display.

## License

Add a `LICENSE` file when you decide how you want others to use this work.
