# 13 — Render semantic scenes on the physical displays

**What to build:** Make both physical OLEDs render the same semantic scenes and timing behavior already proven by the simulator.

**Blocked by:** 02 — Apply minimal controller, power, and display corrections; 09 — Compose prioritized status for both displays; 12 — Connect physical firmware and Offline Controls.

**Status:** ready-for-agent

- [ ] The graphical OLED renders Persistent Status at `0x3C` and the Newhaven renders Contextual Status at `0x3D`.
- [ ] Firmware renders built-in animations locally and direct diagnostic text remains available.
- [ ] Unknown scenes fall back safely and update-size/rate limits protect the device.
- [ ] Reconnect, offline scenes, reduced motion, and simulator parity are verified on hardware.
