# Blip revision-one pin-map contract

Status: **header/GPIO contract agreed; KiCad controller identity requires correction in ticket 02**.

The required Blip Controller is the Seeed Studio XIAO ESP32-S3. Header-to-GPIO identities come from Seeed Studio's official XIAO ESP32-S3 pin map: <https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/>. The current KiCad symbol uses obsolete SAMD/RP2040-style MCU pin names and an ESP32-C3 footprint name; ticket 02 must replace those identities without changing the agreed pad positions.

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
| 3V3 | 3V3_OUT | `+3.3V` | Logic and display supply | Agreed; current budget remains open |
| GND | GND | `GND` | Common return | Agreed |
| 5V | VBUS | Unconnected | Not used | Required by USB-C-only power decision |

## MCP23017 matrix contract

The expander is on the shared I2C bus at `0x20`, with address pins A0–A2 grounded. GPA0–GPA3 drive `ROW0`–`ROW3`; GPB0–GPB2 read `COL0`–`COL2`. Firmware currently configures ports 0–3 as row outputs and ports 8–10 as pulled-up column inputs. The twelve switch/diode positions remain a 4×3 matrix.

## Display contract

| Component | Address | Supply | Bus role | Baseline status |
| --- | --- | --- | --- | --- |
| MCP23017 | `0x20` | 3.3 V | Matrix GPIO | Address agrees |
| DM-OLED096-636 | `0x3C` | 3.3 V | Persistent Status | Address is a locked requirement; physical strap/pull-ups remain to verify |
| NHD-0420CW-AB3 | `0x3D` | 3.3 V | Contextual Status | Locked requirement; SA0 and control-level corrections remain ticket 02 |

The graphical OLED connector is currently GND, 3.3 V, SCL, SDA on pins 1–4. The Newhaven baseline connects pin 1 VSS to GND; pin 2 VDD to 3.3 V; pin 3 REGVDD to 3.3 V; pin 7 SCL to SCL; pins 8 SDAIN and 9 SDAOUT to SDA; pin 16 RES through a 10 kΩ pull-up; pin 17 BS0 to GND; pin 18 BS1 to 3.3 V; pin 19 BS2 to GND; and hidden pin 20 VSS to GND. Pins 4–6 and 10–15 are unconnected in the baseline. These are observed connections, not approval: REGVDD, address selection, reset, interface selection, and unused-pin treatment remain explicit ticket-02 corrections/checks.

USB-C on the XIAO is the only power input and carries USB HID plus serial. No project path, secret, or host configuration crosses this hardware contract.
