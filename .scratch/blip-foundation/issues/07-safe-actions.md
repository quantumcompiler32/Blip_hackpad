# 07 — Execute safe versioned actions end to end

**What to build:** Provide code-defined basic macOS actions that can be configured, tested, dry-run, approved, executed, audited, and displayed safely.

**Blocked by:** 06 — Configure and persist the three stable Profiles.

**Status:** ready-for-agent

- [ ] Each action type has a stable identity, schema, validation, permissions, dry-run behavior, and display feedback.
- [ ] Imports never execute actions and risky new mappings require approval.
- [ ] Destructive or externally visible actions require confirmation and unsafe force operations are unavailable.
- [ ] Redacted audits capture result, timing, Profile, and physical or simulated origin.
