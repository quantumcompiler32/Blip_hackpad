# 02 — Apply minimal controller, power, and display corrections

**What to build:** Make only the evidence-backed schematic and PCB changes required for the exact XIAO ESP32-S3, USB-C-only power, both OLEDs, the MCP23017, and repairable assembly.

**Blocked by:** 01 — Establish the authoritative Blip hardware baseline.

**Status:** electrical corrections applied; manufacturing verification remains open in ticket 03

- [x] The XIAO ESP32-S3 symbol, official header-center geometry, pins, BOM identity, and firmware contract agree.
- [x] Newhaven 3.3 V I2C control levels and `0x3D` address are defined; the graphical OLED remains `0x3C`.
- [ ] Pull-ups and connector parts are defined, but worst-case current/thermal behavior, connector orientation and height, USB access, and decoupling performance still require the Hardware Validation Gate.
- [x] Existing routing and matrix topology remain unless a documented defect requires change.
