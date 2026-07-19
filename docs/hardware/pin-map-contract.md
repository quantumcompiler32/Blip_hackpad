# Blip revision-one pin-map contract

Status: **ticket-02 electrical contract applied; fresh KiCad gate verification remains required**.

The required Blip Controller is the Seeed Studio XIAO ESP32-S3. Header-to-GPIO identities come from Seeed Studio's official XIAO ESP32-S3 pin map: <https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/>. Ticket 02 replaced the obsolete controller identities with an embedded ESP32-S3 symbol and a 14-contact, 2.54 mm through-hole socket footprint. The official 15.24 mm row spacing moved each baseline row inward 0.88 mm; functional nets and their route endpoints were preserved.

| XIAO header | ESP32-S3 GPIO | Board net | Blip role | Contract state |
| --- | --- | --- | --- | --- |
| D0 | GPIO1 | `ENC_C` | Left encoder A | Agreed; later net name `LEFT_ENC_A` |
| D1 | GPIO2 | `ENC_D` | Left encoder B | Agreed; later net name `LEFT_ENC_B` |
| D2 | GPIO3 | `ENC_SW1` | Left encoder press | Agreed; later net name `LEFT_ENC_SW` |
| D3 | GPIO4 | Unconnected | Reserved | Intentionally unused; no-connect disposition remains to verify |
| D4 | GPIO5 | `SDA` | Shared I2C data | Agreed by PCB, firmware, and Seeed pin map |
| D5 | GPIO6 | `SCL` | Shared I2C clock | Agreed by PCB, firmware, and Seeed pin map |
| D6 | GPIO43 | Unconnected | Reserved UART TX | Intentionally unused |
| D7 | GPIO44 | Unconnected | Reserved UART RX | Intentionally unused |
| D8 | GPIO7 | `ENC_SW` | Right encoder press | Agreed; later net name `RIGHT_ENC_SW` |
| D9 | GPIO8 | `ENC_A` | Right encoder A | Agreed; later net name `RIGHT_ENC_A` |
| D10 | GPIO9 | `ENC_B` | Right encoder B | Agreed; later net name `RIGHT_ENC_B` |
| 3V3 | 3V3_OUT | `+3.3V` | Logic and display supply | Agreed; preliminary external maximum is 170.8 mA |
| GND | GND | `GND` | Common return | Agreed |
| 5V | VBUS | Unconnected | Not used | Required by USB-C-only power decision |

## MCP23017 matrix contract

The expander is on the shared I2C bus at `0x20`, with address pins A0–A2 grounded. GPA0–GPA3 drive `ROW0`–`ROW3`; GPB0–GPB2 read `COL0`–`COL2`. Firmware currently configures ports 0–3 as row outputs and ports 8–10 as pulled-up column inputs. The twelve switch/diode positions remain a 4×3 matrix.

## Display contract

| Component | Address | Supply | Bus role | Baseline status |
| --- | --- | --- | --- | --- |
| MCP23017 | `0x20` | 3.3 V | Matrix GPIO | A0–A2 grounded; address agrees |
| DM-OLED096-636 | `0x3C` | 3.3 V | Persistent Status | Manufacturer-default fixed address verified |
| NHD-0420CW-AB3 | `0x3D` | 3.3 V | Contextual Status | SA0 tied to 3.3 V; address agrees |

The graphical OLED connector is GND, 3.3 V, SCL, SDA on pins 1–4. Its module includes 4.7 kΩ pull-ups; together with R1/R2 this gives about 2.35 kΩ effective pull-up resistance. The Newhaven now connects VSS pins 1/20 to GND, VDD pin 2 to 3.3 V, REGVDD pin 3 to GND, SA0 pin 4 to 3.3 V, I2C NC pins 5–6 and 10–15 to GND, pin 7 to SCL, pins 8/9 together to SDA, /RES pin 16 directly to 3.3 V, BS0/BS2 pins 17/19 to GND, and BS1 pin 18 to 3.3 V. This selects 3.3 V I2C operation and address `0x3D`. Direct reset-high replaced the baseline R3 pull-up because R3 had no PCB footprint and no controlled reset is required.

The preliminary maximum external 3.3 V load is 170.8 mA: 135 mA Newhaven, 32 mA graphical OLED, 1 mA MCP23017, and 2.8 mA with both I2C lines low through the effective pull-ups. See [component research](component-research.md) for primary sources and limitations. The 700 mA Seeed regulator-output statement leaves nominal margin, but radio/CPU peaks, thermal behavior, bus rise time, display brightness, and assembled measurements remain Hardware Validation Gate checks.

USB-C on the XIAO is the only power input and carries USB HID plus serial. No project path, secret, or host configuration crosses this hardware contract.
