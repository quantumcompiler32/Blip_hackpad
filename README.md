# Blip_hackpad

An ESP32-C3 powered custom wireless macropad project featuring a 3x4 key matrix, rotary encoder input, and a 20x4 OLED display. This project documents the complete hardware design journey from matrix planning to finalized PCB layout.

## Features

- 12-key (3x4) mechanical switch matrix
- 1N4148 diode matrix for anti-ghosting
- Seeed Studio XIAO ESP32C3 controller
- MCP23017 I2C GPIO expander for additional IO
- Newhaven 20x4 character OLED display
- Dual rotary encoder support in hardware design
- Compact custom PCB + enclosure workflow in KiCad

## Project Journey

### Created the Keyboard Matrix (June 9, 2026)
- Set up KiCad and imported required footprints/libraries
- Built initial macropad schematic
- Learned and implemented keyboard matrix scanning design
- Added per-key diodes to prevent ghost inputs

### Schematic Update with New Constraints (June 9, 2026)
- Expanded requirements to include encoders and additional display support
- Ran out of MCU pins and introduced MCP23017 for expansion
- Rebuilt schematic around new IO constraints
- Resolved ERC issues by adding required pull-ups/decoupling/power-flag fixes

### Wiring + PCB Iterations (June 9, 2026)
- Reorganized dense PCB with many components
- Adjusted key spacing using proper center-to-center switch pitch
- Solved routing congestion with vias and iterative placement
- Cleared DRC errors and finalized manufacturable board

### Display + Remodel Revision (June 13, 2026)
- Simplified design direction to one primary display + rotary controls
- Updated schematic and PCB to match revised UX goals
- Re-routed and validated cleanly with DRC

## Repository Structure

```text
Blip_hackpad/
├── CAD/
├── Firmware/
├── Images/
├── PCB/
├── Hackpad_final_BOM.csv
└── README.md
```

## Bill of Materials (BOM)

| Name | Purpose | Qty | Unit Cost (USD) | Total Cost (USD) | Distributor | Manufacturer | MPN |
|---|---|---:|---:|---:|---|---|---|
| Seeed Studio XIAO ESP32C3 | Main microcontroller module | 1 | 4.99 | 4.99 | DigiKey | Seeed Studio | 113991054 |
| Newhaven 20x4 Character OLED Display | Primary 20x4 character display | 1 | 36.27 | 36.27 | DigiKey | Newhaven Display International | NHD-0420CW-AB3 |
| 0.96-inch 128x64 I2C OLED Module | Secondary compact graphics display | 1 | 4.90 | 4.90 | DisplayModule | DisplayModule | DM-OLED096-636 |
| MCP23017 16-bit I2C GPIO Expander, SPDIP-28 | Expands available GPIO pins | 1 | 1.69 | 1.69 | DigiKey | Microchip Technology | MCP23017-E/SP |
| Alps Alpine EC11E Rotary Encoder with Push Switch | Rotary input controls | 2 | 5.00 | 10.00 | DigiKey | Alps Alpine | EC11E09244AQ |
| Cherry MX PCB-mount Mechanical Keyswitch | 3x4 key matrix switches | 12 | 1.75 | 21.00 | DigiKey | Cherry Americas | MX2A-E1NW |
| 1N4148 Through-hole Switching Diode | Prevents matrix ghosting | 12 | 0.10 | 1.20 | DigiKey | onsemi | 1N4148 |
| 4.7 kΩ 1% 0805 Resistor | I2C pull-up resistors | 2 | 0.10 | 0.20 | DigiKey | YAGEO | RC0805FR-074K7L |
| 10 kΩ 1% 0805 Resistor | Display/control pull-up resistor | 1 | 0.10 | 0.10 | DigiKey | YAGEO | RC0805FR-0710KL |
| 100 nF 50 V X7R 0805 Ceramic Capacitor | 3.3V decoupling | 1 | 0.11 | 0.11 | DigiKey | YAGEO | CC0805KRX7R9BB104 |

> Full sourcing, references, footprints, and links are available in [`Hackpad_final_BOM.csv`](./Hackpad_final_BOM.csv).

## Folders and What Goes Where

- **CAD/**: Enclosure files (STEP/STL), mechanical revisions, printable parts
- **PCB/**: KiCad project files, schematics, PCB layout, gerbers, drill files, assembly outputs
- **Firmware/**: Firmware source code, keymap/configuration, flashing/build instructions
- **Images/**: Project photos, renders, wiring diagrams, and build screenshots

## Suggested Next Uploads

1. Put KiCad files in `PCB/` (`.kicad_pro`, `.kicad_sch`, `.kicad_pcb`, fabrication outputs)
2. Put case and mechanical files in `CAD/`
3. Put firmware source + README in `Firmware/`
4. Move all journal screenshots into `Images/` and update image links

## Build Media (add paths once uploaded)

- `Images/schematic.png`
- `Images/pcb-front.png`
- `Images/pcb-back.png`
- `Images/build-1.png`
- `Images/build-2.png`

## License

Add a `LICENSE` file when you decide how you want others to use this work.
