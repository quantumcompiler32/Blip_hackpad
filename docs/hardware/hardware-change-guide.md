# Blip Hardware Validation Gate

Status: **AUTOMATION COMPLETE — HUMAN SIGN-OFF REQUIRED**

This is the single manufacturing-review guide for the Macondo/Stardance submission. It records the minimum changes made to the existing board, the reproducible checks, and the remaining physical decisions. Do not order the PCB until the final checklist is signed.

## Gate result

| Check | Result | Evidence / disposition |
| --- | --- | --- |
| ERC | **ERC PASS** | `evidence/erc.rpt`: 0 violations |
| Board rules | **DRC PASS** | `evidence/drc.rpt`: 0 violations |
| PCB/schematic parity | **Schematic parity PASS** | KiCad DRC run with “Test for parity” enabled: 0 |
| Connectivity | **Unconnected items PASS** | KiCad DRC: 0 unconnected pads |
| BOM and pin map | **PASS** | `scripts/verify-hardware-source-of-truth.sh` and `scripts/verify-minimal-pcb-corrections.sh` |
| Mechanical export | **PARTIAL / HUMAN GATE** | `../../CAD/Blip-PCB-Assembly.step` is fresh, but it is not a complete assembly model. KiCad could not resolve stock switch and encoder models, and the XIAO/display modules have no embedded product models. Installed heights require the checklist below. |

The five names under “Ignored checks” in `evidence/drc.rpt` are project-level disabled rule classes, not hidden marker exclusions. Their dispositions are explicit:

- **Footprint has no courtyard:** not used as manufacturing proof. U1 has a checked courtyard; all other physical envelope/height decisions remain in the unsigned mechanical checklist.
- **Track endpoint not centered on via:** legacy routing-style check only; connectivity, clearance, shorts, and unconnected-item checks remain enabled and clean.
- **Tuning profile track geometries:** Blip has no tuned high-speed pairs or tuning profiles.
- **Footprint does not match symbol filters:** library metadata check; BOM/pin identity is enforced by the source-of-truth scripts.
- **Footprint component type does not match pads:** metadata check; every fitted part/pad mapping remains covered by ERC, DRC, BOM, and pin-map verification.

There are no excluded DRC markers. If a reviewer requires these global classes enabled, that is a separate board-policy change; this gate does not imply they ran.

## What changed

```text
Before at e0d795d                    After
XIAO rows: already 15.24 mm          XIAO rows: 15.24 mm, unchanged
Header pads: 2.20 / 1.00 mm          Header pads: 1.524 / 0.889 mm
Routes relied on oversized pads      Routes terminate on official pad centers
Stale display power trace present   Stale trace removed
U1-area routing no longer aligned   Only U1/display-area tracks rerouted
DRC after official geometry: 29     DRC: 0 errors, 0 unconnected, 0 parity
```

Review the [annotated before/after composite](evidence/before-after-annotated.svg), backed by the raw [before — e0d795d](evidence/board-before-e0d795d.svg) and [after — validated](evidence/board-after.svg) exports. Callout A marks the official XIAO pad geometry, B marks the removed stale display-power segment, and C marks the localized SDA/ENC_D reroute. The git diff against `e0d795d` is the coordinate-level record; no switches, encoders, display outline, mounting holes, or board outline were recreated.

`evidence/validation-results.txt` binds both source files and exported evidence by SHA-256 so a later PCB/schematic edit makes the verifier fail instead of accepting stale reports.

## Ordered KiCad operations

1. Open `PCB/Hackpad.kicad_pro`, then open the PCB Editor.
2. Confirm U1 uses `Blip:XIAO_ESP32S3_Socket`, rotated −90°, with USB-C facing the edge labelled `USB-C ACCESS`.
3. In Footprint Properties, verify 14 through-hole pads, 2.54 mm pitch along each row, 15.24 mm between rows, 1.524 mm pad diameter, and 0.889 mm drill.
4. Preserve the existing XIAO pin order. Do not renumber pads or swap the two rows.
5. Inspect the localized U1/display reroute. The SDA bridge changes layers to avoid the SCL/power barriers; ENC_D routes below the 3.3 V via and the display mounting hole.
6. Refill zones. Open Inspect → Design Rules Checker, enable both zone refill and schematic parity, then run DRC.
7. Require 0 DRC violations, 0 unconnected items, and 0 parity differences. Do not accept a U1 library-copy warning; the library and placed footprint are synchronized.
8. In Schematic Editor, run ERC and require 0 violations.
9. Export a fresh STEP file after every footprint or outline change.

## Locked electrical mapping

| Function | XIAO signal |
| --- | --- |
| Encoder 1 A / B / push | D0 / D1 / D2 |
| I²C SDA / SCL | D4 / D5 |
| Encoder 2 push / A / B | D8 / D9 / D10 |
| Logic power | XIAO 3V3 output only |
| Board power | USB-C into XIAO only; never inject external 3.3 V |

The MCP23017 remains at address `0x20` with A0–A2 grounded. The 3×4 switch matrix remains COL0–COL2 and ROW0–ROW3 with one diode per key.

## Mechanical and manufacturing checklist

- [ ] Open the current STEP and visually confirm the PCB outline and all mounting holes.
- [ ] Fit the real XIAO/socket combination and record total installed height.
- [ ] Confirm the XIAO USB-C plug is reachable with the enclosure assembled.
- [ ] Fit both physical displays and confirm header pitch, pin 1, orientation, and clearance above the XIAO/socket.
- [ ] Fit both encoders; confirm shaft, body, mounting-tab, and knob clearance.
- [ ] Confirm encoder clockwise direction with continuity testing or first-article firmware.
- [ ] Confirm no metal display or enclosure feature contacts XIAO antenna/components.
- [ ] Review the fabrication Gerbers in the board house viewer after export.
- [ ] Measure first-article USB current before enabling both displays at full brightness.
- [ ] Confirm no external 3.3 V supply is connected while USB-C is attached.
- [ ] Moksh signs and dates the manufacturing decision below.

**HUMAN SIGN-OFF REQUIRED**

- [ ] Approved for manufacture by Moksh: __________________  Date: __________
