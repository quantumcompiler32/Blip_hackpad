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
├── BOM/                Bill of materials
├── Images/             Schematic, PCB, and build pictures
├── PCB/                KiCad design files and manufacturing outputs
├── docs/               Architecture, specifications, and validation evidence
├── src/
│   ├── blip-agent/     Background host event pipeline
│   ├── control-center/ Minimal diagnostic view
│   ├── firmware/       Complete PlatformIO project
│   ├── shared/         Device-event and semantic-state contracts
│   └── simulator/      Virtual Blip Device and scenario CLI
└── test/               Foundation acceptance tests
```

## Simulator tracer scenario

Node.js 22.6 or newer can run the Foundation tracer without installing
dependencies:

```sh
npm test
npm run --silent scenario
npm run --silent scenario:view > /tmp/blip-control-center-diagnostic.html
```

The scenario command emits machine-readable JSON. The HTML command renders the
same Agent observation as a minimal Control Center diagnostic view. Both outputs
label the device as virtual, and the scenario never executes a host action.
