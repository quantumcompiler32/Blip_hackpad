# Component research for minimal PCB corrections

This note records only first-party manufacturer facts needed by ticket 02. It is not a substitute for a fresh ERC, DRC, schematic/PCB parity check, assembled-board test, or human manufacturing approval.

## Decision summary

| Part | Evidence-backed ticket 02 constraint |
| --- | --- |
| Seeed Studio XIAO ESP32-S3 | Use the official XIAO ESP32-S3 symbol/footprint and its 14 castellated/header pins. D4/GPIO5 is SDA, D5/GPIO6 is SCL, and 3V3 is a regulated output rated by Seeed for 700 mA. Keep 5V/VBUS unconnected on the carrier for the locked USB-C-only design. |
| Newhaven NHD-0420CW-AB3 | In I2C mode it is 3.3 V only: VDD=3.3 V and REGVDD=0 V. Tie SDAIN and SDAOUT together, set BS0=0, BS1=1, BS2=0, and use SA0 high to select 7-bit address `0x3D`. |
| DisplayModule DM-OLED096-636 | The four-pin breakout is GND, VCC_IN, SCL, SDA and accepts 2.8–5.5 V at VCC_IN. Its documented default control byte `0x78` is 7-bit address `0x3C`. The module schematic already includes 4.7 kΩ I2C pull-ups. |
| Microchip MCP23017 | Operates from 1.8–5.5 V, draws at most 1 mA under the specified 1 MHz condition, and uses 7-bit address `0100 A2 A1 A0`; biasing A2:A0 low selects `0x20`. A0:A2 and RESET must not float. |

## Seeed Studio XIAO ESP32-S3

- The standard board is **21 mm × 17.8 mm**. Seeed publishes the authoritative DXF, KiCad project, symbol library, footprint library, and 3D model from the product wiki. The PCB should import/use those official assets instead of retaining the current ESP32-C3 footprint or the SAMD-style symbol. [Seeed XIAO ESP32-S3 hardware overview and resources](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/)
- The official header mapping relevant to Blip is: D0=GPIO1, D1=GPIO2, D2=GPIO3, D4/SDA=GPIO5, D5/SCL=GPIO6, D8=GPIO7, D9=GPIO8, and D10=GPIO9. The same page identifies 5V as VBUS, 3V3 as `3V3_OUT`, and GND as ground. [Seeed pin map](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/#xiao-esp32-s3-front)
- Seeed describes the 3V3 pin as the onboard regulator's output and says **700 mA may be drawn**. It describes the 5V pin as USB-port output and warns that external input on that pin requires a diode. Blip does not use external 5V injection, so the carrier's 5V/VBUS header pin should remain unconnected. [Seeed power-pin guidance](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/#power-pins)
- Seeed lists typical standard-board consumption as **5 V at 19 mA** during its unspecified baseline operating condition. This is not a radio-transmit peak and must not be used as a worst-case ESP32-S3 current figure. [Seeed specifications](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/#specification)
- The module is supplied without headers by default; Seeed instructs the user to solder headers for expansion-board use. A socketed Blip assembly therefore requires explicit matching 2.54 mm header/socket BOM items rather than assuming they ship with the XIAO. [Seeed solder-header guidance](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/#solder-header)
- Seeed's official `XIAO-ESP32-S3-DIP.kicad_mod` places the two through-hole header rows at `y=-7.62 mm` and `y=+7.62 mm`, for **15.24 mm row spacing**, and places seven contacts per row at 2.54 mm pitch. Blip rotates that exact center pattern 90 degrees to suit the existing board orientation. [Seeed official XIAO ESP32-S3 DIP footprint](https://github.com/Seeed-Studio/OPL_Kicad_Library/blob/master/Seeed%20Studio%20XIAO%20Series%20Library/XIAO-ESP32-S3-DIP.kicad_mod)

### XIAO remaining uncertainty

- The wiki's 700 mA statement is a regulator-output claim, but it does not give a complete thermal derating curve or a worst-case Wi-Fi/BLE current budget for the assembled Blip. It is suitable as an upper regulator limit, not proof that any arbitrary 700 mA peripheral load is safe.
- Header centers are now derived from Seeed's official footprint. USB-C, antenna, reset/boot, socket body, and enclosure clearances still require the ticket-03 2D/3D and physical-fit gate.

## Newhaven NHD-0420CW-AB3

The following facts come from revision 5 of Newhaven's official product specification. [NHD-0420CW-AB3 product specification](https://newhavendisplay.com/content/specs/NHD-0420CW-AB3.pdf)

### Required 3.3 V I2C wiring

| Pin | Signal | Required connection |
| ---: | --- | --- |
| 1 | VSS | GND |
| 2 | VDD | 3.3 V |
| 3 | REGVDD | GND; the datasheet requires 0 V for 3.3 V operation |
| 4 | SA0 | 3.3 V for 7-bit address `0x3D` |
| 5–6 | NC | GND, per datasheet |
| 7 | SCL | Shared I2C SCL |
| 8 | SDAIN | Shared I2C SDA |
| 9 | SDAOUT | Tie to pin 8 and shared I2C SDA |
| 10–15 | NC | GND, per datasheet |
| 16 | /RES | Active-low reset; a pull-up is acceptable if no MCU reset control is required |
| 17 | BS0 | GND (`0`) |
| 18 | BS1 | 3.3 V (`1`) |
| 19 | BS2 | GND (`0`) |
| 20 | VSS | GND |

Newhaven explicitly says I2C operation supports **3.3 V only**, with VDD=3.3 V and REGVDD=0 V; 5 V I2C mode is not supported. Its interface table selects I2C with BS0/BS1/BS2=`0/1/0`. The I2C pin-assignment summary uses SA0 as the address-select signal and requires SDAIN/SDAOUT to be tied together. [NHD-0420CW-AB3 product specification, pp. 4–5](https://newhavendisplay.com/content/specs/NHD-0420CW-AB3.pdf)

The module specification does not print the resulting hexadecimal addresses in its pin table, but Newhaven's official US2066 controller specification gives the 7-bit address as `0111100` or `0111101`, with SA0 as the low address bit. Therefore SA0 low is `0x3C` and SA0 high is `0x3D`. [Newhaven-hosted US2066 specification, section 5.1.4](https://newhavendisplay.com/content/app_notes/US2066.pdf)

### Current and logic levels

- VDD is specified as 2.7–3.6 V for nominal 3.3 V operation.
- Supply current is **70 mA typical, 135 mA maximum**; sleep current is 2 mA typical, 5 mA maximum.
- Input-high is at least 0.8×VDD and input-low is at most 0.2×VDD, so a 3.3 V XIAO and 3.3 V pull-ups are level-compatible.

## DisplayModule DM-OLED096-636

The manufacturer hosts both a product page and the official 2018 datasheet. [DisplayModule product page](https://www.displaymodule.com/products/0-96-inch-oled-graphic-display-128x64-with-i2c) [DM-OLED096-636 datasheet](https://cdn.shopify.com/s/files/1/0264/7629/files/DM-OLED096-636_Datasheet.pdf?v=1659336245)

- The breakout connector is pin 1 GND, pin 2 VCC_IN (**2.8–5.5 V**), pin 3 SCL, and pin 4 SDA. Feeding VCC_IN from Blip's 3.3 V rail is within the module specification.
- The module is a 128×64 I2C display using an SSD1315 controller and measures 26.0 × 26.0 × 2.7 mm.
- Operating current is **25 mA typical, 32 mA maximum** under the datasheet's stated condition (VDD=2.8 V, internally generated VCC=7.25 V, 100% display area on).
- The module schematic includes **4.7 kΩ pull-ups on both SDA and SCL**. Retaining Blip's existing 4.7 kΩ bus pull-ups would place each pair in parallel for about 2.35 kΩ effective pull-up resistance while this exact module is installed.
- The datasheet calls `0x78` the default I2C "slave address" and `0x7A` the alternate when the panel D/C#/SA0 pin is high. Those values include the R/W bit: their 7-bit forms are respectively **`0x3C` and `0x3D`**. The four-pin breakout does not expose D/C#/SA0, and its module schematic documents the default `0x78`, so Blip should treat this module as fixed at 7-bit **`0x3C`**.

### DM-OLED096-636 uncertainties

- The product page says "Power Supply: 2.8 V" while the module-connector table says VCC_IN accepts 2.8–5.5 V. The connector table and module schematic are the more specific breakout-level evidence; 3.3 V is inside both the controller logic maximum and the VCC_IN range.
- The 4-pin module cannot be field-strapped to `0x3D` without modifying its internal panel wiring. The Newhaven display should therefore take `0x3D`.

## Microchip MCP23017

The following facts come from Microchip's current DS20001952D datasheet. [MCP23017/MCP23S17 datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/MCP23017-Data-Sheet-DS20001952.pdf)

- Supply range is 1.8–5.5 V. Maximum supply current is **1 mA** at the datasheet's SCL=1 MHz test condition; standby current is at most 1 µA through +85 °C.
- The I2C address bits are `0100 A2 A1 A0`. Biasing A2, A1, and A0 low yields 7-bit address **`0x20`**. Microchip says all three address pins must be externally biased.
- RESET must also be externally biased; it must not float. The existing direct 3.3 V connection is a valid always-enabled choice if no controlled reset is required.
- At 3.3 V, 100 kHz and 400 kHz I2C modes are supported. The datasheet permits up to 400 pF bus capacitance at those speeds and specifies maximum rise times of 1000 ns at 100 kHz and 300 ns at 400 kHz. Pull-up selection must ultimately be checked against the assembled bus capacitance, not chosen from resistance alone.
- MCP23017's optional GPIO pull-ups are approximately 100 kΩ and apply to GPIO inputs, not SDA/SCL. They can pull up the three matrix column inputs in firmware, but they do not replace the external I2C pull-ups.

## Preliminary 3.3 V current budget

This is a conservative component subtotal, not a passed power-validation gate:

| Load | Maximum used |
| --- | ---: |
| Newhaven NHD-0420CW-AB3 | 135 mA |
| DM-OLED096-636 | 32 mA |
| MCP23017 core | 1 mA |
| Two I2C lines low through effective 2.35 kΩ pull-ups | 2.8 mA |
| **External subtotal** | **170.8 mA** |

This subtotal is below Seeed's stated 700 mA 3V3-output figure, but the remaining margin cannot be called validated until the XIAO ESP32-S3's own worst-case radio/CPU draw, regulator thermal behavior, actual display brightness patterns, and the connected USB source/cable are included and measured. A conservative firmware validation should test both displays at maximum intended brightness while Wi-Fi transmits.

## Minimal-change implications

1. Replace only U1's incorrect symbol/footprint/BOM identity with the XIAO ESP32-S3 contract. Use Seeed's official 2.54 mm pitch and 15.24 mm row spacing, rotated into Blip's existing orientation; preserve named functional nets and placement where possible.
2. Leave XIAO 5V/VBUS unconnected and power peripherals from 3V3.
3. Correct the Newhaven wiring: REGVDD to GND, SA0 to 3V3, BS0/BS2 to GND, BS1 to 3V3, all datasheet-designated I2C NC pins to GND, SDAIN+SDAOUT together, and /RES held high.
4. Keep the graphical module at 7-bit `0x3C`; use the Newhaven at `0x3D`; keep MCP23017 at `0x20`.
5. Existing 4.7 kΩ board pull-ups plus the graphical module's onboard 4.7 kΩ pull-ups produce about 2.35 kΩ effective pull-ups. This is electrically plausible at 3.3 V, but a fresh bus-capacitance/rise-time check and assembled waveform measurement remain required. Removing the carrier pull-ups would make the bus dependent on this exact graphical breakout, so retaining them is the more repairable minimal-change baseline unless measurement shows excessive sink current.
