# 05 — Prove the bundled Agent lifecycle and private IPC

**What to build:** Make Control Center communicate with a long-running Blip Agent through private local IPC while presenting clear, reversible lifecycle controls.

**Blocked by:** 04 — Prove the simulator-to-Agent tracer bullet.

**Status:** ready-for-agent

- [ ] Current-user-only versioned Unix-domain IPC carries the tracer scenario.
- [ ] The Agent continues operating after Control Center closes and reconnects when it returns.
- [ ] Login-item consent, session-only operation, restart, disconnect, and incompatible-version states are visible.
- [ ] The unsigned Apple Silicon development build runs on the target Mac.
