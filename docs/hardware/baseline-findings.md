# Hardware baseline findings

Recorded 2026-07-18. The Hardware Validation Gate has **not passed**. This is a preservation audit, not manufacturing approval.

## Preserved sources

| File | SHA-256 |
| --- | --- |
| `Hackpad.kicad_pro` | `d8c1aa42e53af8127d99fc2cb34fe868f9aad859bb383313cd3f733fd328e3e2` |
| `Hackpad.kicad_sch` | `2a603a45c626644701ca69ce53ecbd3906184bcae56ee97f0529a81a8c31a7d6` |
| `Hackpad.kicad_pcb` | `38016ccee050912f7b6a07835543e346bc84e0e7174babc7ce56f270c00cc676` |

The hashes match Moksh's active external project at migration time, proving that ticket 01 did not alter placement, routing, or matrix topology. They are immutable migration evidence, not a permanent test of the editable authoritative sources.

## Baseline observations

- Controller mismatch: the schematic embeds `XIAO_RP2040:MOUDLE-SEEEDUINO-XIAO`, exposes SAMD-style `PA...` pin names, and assigns `RF_Module:MCU_Seeed_ESP32C3`. The BOM and firmware require XIAO ESP32-S3. Ticket 02 must replace and verify this identity without moving the header pads.
- ERC evidence supplied before migration showed zero errors and three warnings: GND/VSS aliasing on the Newhaven display, missing XIAO symbol library, and an unattached no-connect flag. Four tests were ignored. Each warning and exclusion still needs resolution or justification.
- DRC evidence supplied before migration showed zero violations and zero unconnected items, but schematic parity was not run and five tests were ignored. A green DRC screenshot alone is not a gate pass.
- The board preserves the 4×3 matrix, two encoders, MCP23017, two I2C displays, and USB-powered XIAO header arrangement expected by the product contract.
- BOM controller text says XIAO ESP32-S3 but lists an ESP32-C3 footprint. Socket/header items and mechanical installed heights still require audit.
- KiCad CLI is not available in the current shell, so ERC, DRC, parity, and fresh mechanical export were not rerun in this ticket.

## Gate state

| Check | Baseline state |
| --- | --- |
| Authoritative source migration | Passed by matching hashes |
| Routed placement preservation | Passed by byte-identical PCB source |
| XIAO ESP32-S3 identity | Failed; correction required |
| Firmware/header-position agreement | Passed against Seeed's official D0–D10 to GPIO map |
| ERC | Not passed; warnings/exclusions open |
| DRC | Not passed for the gate; prior clean result has exclusions |
| Schematic-to-PCB parity | Not run |
| BOM/socket consistency | Not passed |
| Current budget and pull-up calculation | Not performed |
| Mechanical review and fresh STEP | Not performed |
| Human manufacturing approval | Not granted |

Next work is ticket 02, applying the smallest evidence-backed controller, power, and display corrections.
