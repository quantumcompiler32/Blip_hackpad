# Blip

Blip is a custom macropad I am designing for macOS. It combines a 4-row by
3-column mechanical-key matrix, two rotary encoders, a XIAO ESP32-S3, and two
displays. The goal is to make a small physical control surface that can trigger
useful actions and show status without feeling like a tiny touchscreen.

## Features

- 12-key (3x4) mechanical switch matrix
- 1N4148 diode matrix for anti-ghosting
- Seeed Studio XIAO ESP32-S3 controller for native USB HID
- MCP23017 I2C GPIO expander for additional IO
- Newhaven 20x4 character OLED display
- Two rotary encoders with push switches
- USB-C-only power through the removable XIAO module
- A removable MCP23017 GPIO expander and removable display connections

## Bill of materials

The full, quoted CSV is in [`BOM/Hackpad_final_BOM.csv`](./BOM/Hackpad_final_BOM.csv).
It matches the current PCB: 12 keys, 12 diodes, two encoders, two 4.7 kΩ
pull-ups, one 100 nF capacitor, the XIAO ESP32-S3, the MCP23017, and both
display modules. The removable socket and header parts are listed separately
because they are assembly parts rather than separate KiCad footprints.

The current component total is **$97.95 USD** using the listed prices. That
number does not include PCB fabrication, an enclosure, keycaps, encoder knobs,
or mounting hardware. Prices are planning numbers, not a final purchase quote.

## Repository Structure

```text
Blip_hackpad/
├── BOM/                Bill of materials
├── Images/             Schematic and PCB reference images
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

## Hardware submission status

The KiCad project in `PCB/` is the hardware source of truth. Automated ERC,
DRC, schematic-parity, source-policy, and BOM checks are recorded in
`docs/hardware/`. The Hardware Validation Gate is not a manufacturing approval:
I still need to inspect the current STEP export, confirm connector and encoder
clearances with the real parts, review USB-C access, and sign the final
manufacturing checklist in [`docs/hardware/hardware-change-guide.md`](./docs/hardware/hardware-change-guide.md).
The complete repository-versus-Macondo checklist is in
[`docs/hardware/macondo-submission-checklist.md`](./docs/hardware/macondo-submission-checklist.md).

To rerun the repository checks:

```sh
sh scripts/verify-hardware-source-of-truth.sh
sh scripts/verify-minimal-pcb-corrections.sh
sh scripts/verify-bom-against-pcb.sh
sh scripts/verify-hardware-validation-gate.sh
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
