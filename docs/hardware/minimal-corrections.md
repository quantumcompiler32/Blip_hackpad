# Ticket 02 minimal hardware corrections

Applied 2026-07-18. This records edits, not manufacturing approval. Primary-source electrical evidence is in [component research](component-research.md).

## Controller and assembly

- U1 now identifies only the Seeed Studio XIAO ESP32-S3 in the schematic, PCB, BOM, and firmware-facing pin contract.
- U1 now uses Seeed's official seven-by-two center pattern: 2.54 mm contact pitch and 15.24 mm row spacing, rotated into the existing board orientation. Correcting the old 17.0 mm spacing moved each row inward 0.88 mm. The existing route endpoints remain inside the 2.2 mm carrier pads, so no functional net or matrix route needed to move.
- The obsolete ESP32-C3 underside land pattern and 3D-model reference were removed. U1 5V/VBUS remains intentionally unconnected; the XIAO USB-C connector is the only power input.
- The BOM now includes exact XIAO male/female header parts, an exact 0.3-inch MCP23017 DIP socket, and exact Samtec male/female 2.54 mm connector pairs for both displays. Installed-height, enclosure, and hand-access validation remain part of the ticket-03 mechanical review.

## Newhaven display

The Newhaven NHD-0420CW-AB3 is corrected to the manufacturer's 3.3 V I2C wiring:

| Pins | Applied net | Reason |
| --- | --- | --- |
| 1, 3, 5, 6, 10–15, 17, 19, 20 | GND | VSS, REGVDD, I2C-mode NC, and BS0/BS2 requirements |
| 2, 4, 18 | 3.3 V | VDD, SA0 high, and BS1 high |
| 7 | SCL | Shared I2C clock |
| 8–9 | SDA | Required tied SDAIN/SDAOUT |
| 16 | Direct 3.3 V | Active-low reset held inactive; removes the baseline R3 part that had no PCB footprint |

SA0 high selects seven-bit address `0x3D`, leaving the fixed graphical module at `0x3C` and MCP23017 at `0x20`. New display straps use short B.Cu segments beside the existing header row; no matrix trace, switch, encoder, or component origin moved. The only U1 geometry change is the documented 0.88 mm inward movement of each header row.

## Power, pull-ups, and decoupling

- Seeed documents up to 700 mA from 3V3. The researched **external-load subtotal** is 170.8 mA. It excludes the XIAO's CPU/radio peak, regulator thermal behavior, and USB source/cable, so the worst-case current-budget criterion remains open for ticket 03.
- R1/R2 remain 4.7 kΩ. The exact graphical module has onboard 4.7 kΩ pull-ups, producing about 2.35 kΩ effective resistance and about 1.4 mA sink per low bus line. Keeping carrier pull-ups avoids making repairability depend on the graphical module.
- C1 is the existing 100 nF local rail bypass beside the MCP23017/controller area. Both display assemblies are manufacturer modules rather than bare OLED panels. No speculative bulk capacitor or display-module redesign was added; maximum-brightness, Wi-Fi, rail-noise, and I2C-rise-time measurements remain required at the Hardware Validation Gate.

## Verification still required

KiCad 10.0.1 is installed inside the macOS app bundle. Ticket-02 verification produced:

- ERC: 0 errors and 0 warnings; four project checks remain intentionally ignored and require ticket-03 disposition.
- PCB Editor DRC before the final official XIAO row-spacing correction showed 0 violations and 0 unconnected items; five checks remained ignored. The post-correction CLI DRC still aborts with exit 134, so a fresh GUI DRC is explicitly required by ticket 03.
- Schematic parity: not passed. KiCad continued to retain a stale `Missing footprint R3` marker after R3 was removed in favor of direct reset-high wiring, and the CLI DRC process aborted with exit 134 before writing a report. Ticket 03 must clear/rebuild the project cache and rerun parity from a fresh session.

Ticket 03 must also inspect the new B.Cu display straps, verify pin-one/USB/display orientation and installed heights, export a fresh STEP model, and capture the human visual approval. Do not manufacture from ticket 02 alone.
