# 12 — Connect physical firmware and Offline Controls

**What to build:** Connect the XIAO ESP32-S3 through the shared protocol and prove that physical input ownership matches simulator behavior without duplicate actions.

**Blocked by:** 01 — Establish the authoritative Blip hardware baseline; 02 — Apply minimal controller, power, and display corrections; 04 — Prove the simulator-to-Agent tracer bullet; 06 — Configure and persist the three stable Profiles.

**Status:** ready-for-agent

- [ ] Versioned JSONL handshake, sequencing, timestamps, heartbeats, acknowledgements, limits, reconnect, and malformed handling are verified.
- [ ] Right encoder performs volume and play/pause offline and is never re-executed by the Agent.
- [ ] SW11 cycles device-owned Profiles offline and reconciles without duplicate actions.
- [ ] SW12 and Agent-owned inputs clearly report offline behavior; manual PlatformIO flashing is documented.
