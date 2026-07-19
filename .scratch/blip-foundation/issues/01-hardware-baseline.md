# 01 — Establish the authoritative Blip hardware baseline

**What to build:** Preserve the active KiCad project as Blip's single hardware source of truth and produce one reviewed XIAO ESP32-S3 pin-map contract covering every hardware and firmware connection.

**Blocked by:** None — can start immediately.

**Status:** ready-for-agent

- [ ] Only the active KiCad sources are authoritative; generated history, locks, preferences, and unrelated projects are excluded.
- [ ] Existing placement, routing, and matrix topology are preserved.
- [ ] One pin-map contract records the agreed controller target and every observed agreement or mismatch across inputs, displays, I2C, USB, power, firmware, and KiCad.
- [ ] Baseline ERC, DRC, parity, BOM, and mechanical findings are recorded without claiming the gate passed.
