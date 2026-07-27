# Blip Hardware Validation and Foundation Release Specification

## Problem Statement

Blip has a promising routed PCB, early firmware, and a detailed product direction, but they do not yet form one buildable, reviewable system. The PCB has not been manufactured. Its current KiCad project identifies an inconsistent XIAO family, has unresolved ERC warnings, has not recorded schematic-to-PCB parity, and has not yet proven that display wiring, power, pin assignments, BOM entries, and firmware assumptions agree. Submitting it now would risk approval delays or manufactured hardware that needs rework.

At the same time, waiting for fabricated hardware would unnecessarily block the macOS product. The existing firmware performs too much host-dependent behavior directly and does not provide the stable device-event boundary needed by the Blip Agent, Control Center, or automated tests. Blip therefore needs an urgent, minimal-change Hardware Validation Gate and a parallel simulator-first Foundation Release that converge on the same protocol and pin map.

The result must also be understandable and honestly reproducible by Moksh for Macondo and Stardance. Technical assistance cannot substitute for his authorship, review, journals, or final manufacturing decision.

## Solution

Deliver Blip through two coordinated workstreams.

The hardware workstream migrates only the authoritative KiCad sources into the repository, audits them against primary component documentation, and applies the smallest corrections needed for the exact XIAO ESP32-S3, the two displays, the MCP23017, the switch matrix, the encoders, USB-C-only power, socketed assembly, and firmware-facing pin map. It produces a single visual Hardware Change Guide containing ordered edits, before-and-after evidence, warning dispositions, automated validation results, mechanical checks, and a final human review checklist. Manufacturing remains blocked until this gate passes.

The software workstream builds a high-fidelity virtual Blip Device first. Simulated and physical input enter the same versioned device-event boundary in the Blip Agent. That Agent owns configuration, persistence, host actions, Pomodoro state, display composition, audit history, and private communication with Control Center. Firmware retains scanning, Offline Controls, display rendering, device state, and protocol transport. This makes the complete Foundation Release testable before the PCB exists, then allows physical hardware to replace the simulator without changing product behavior.

Both workstreams use two release-level acceptance seams:

1. Authoritative KiCad project to automated electrical and consistency evidence to the Hardware Change Guide and final human visual review.
2. Simulated or physical device event through the shared Blip Agent pipeline to a dry-run action result, semantic display state, and Control Center observation.

## User Stories

1. As Moksh, I want the Hardware Validation Gate prioritized, so that I can seek Hack Club approval without waiting for the complete macOS application.
2. As Moksh, I want software development to proceed with a virtual Blip Device, so that fabrication lead time does not block progress.
3. As Moksh, I want one authoritative KiCad project in the repository, so that reviewers and tools inspect the same design.
4. As Moksh, I want automatic backups, lock files, local preferences, and unrelated starter projects excluded, so that submission evidence is unambiguous.
5. As Moksh, I want existing placement, routing, and matrix topology preserved unless a concrete defect is found, so that validation does not become a PCB redesign.
6. As Moksh, I want every required hardware correction collected in one Hardware Change Guide, so that I do not have to reconstruct decisions from chat history.
7. As Moksh, I want annotated before-and-after visuals for hardware corrections, so that I can understand and verify each change in KiCad.
8. As Moksh, I want exact ordered KiCad operations in the guide, so that I can reproduce or inspect each correction myself.
9. As a Hack Club reviewer, I want the controller identified consistently as the XIAO ESP32-S3, so that the design does not mix incompatible XIAO variants.
10. As a firmware developer, I want a single verified pin map shared by schematic, PCB, firmware, and documentation, so that electrical connections match software behavior.
11. As Moksh, I want every ERC warning fixed or explicitly justified, so that ignored problems do not masquerade as a passing design.
12. As Moksh, I want DRC run with zero unexplained violations and zero unconnected items, so that layout defects are found before manufacturing.
13. As Moksh, I want schematic-to-PCB parity run and recorded, so that a clean DRC cannot hide stale layout data.
14. As Moksh, I want ignored ERC and DRC tests reviewed individually, so that the gate records whether each exclusion is safe.
15. As a hardware builder, I want the Newhaven display configured for 3.3-volt I2C operation with defined control pins, so that it powers up predictably.
16. As a firmware developer, I want the graphical OLED at `0x3C` and the Newhaven display at `0x3D`, so that both can share the bus without an address collision.
17. As a hardware builder, I want combined I2C pull-up strength verified, so that multiple module pull-ups do not create an unreliable bus.
18. As a hardware builder, I want a worst-case 3.3-volt current budget, so that the XIAO regulator can safely power both displays and the expander.
19. As a hardware builder, I want local decoupling and bulk capacitance reviewed, so that display and input activity do not destabilize the device.
20. As a hardware builder, I want USB-C to be the only power source, so that revision one avoids unsafe or ambiguous power paths.
21. As a hardware builder, I want a warning against externally driving 3.3 volts while USB is connected, so that assembly and debugging remain safe.
22. As a hardware builder, I want the XIAO, MCP23017, and both displays removable, so that expensive or programmable parts can be repaired or replaced.
23. As a hardware builder, I want socket and header orientations, installed heights, and USB-C access checked, so that the assembled parts physically fit and remain usable.
24. As Moksh, I want left and right encoders clearly identified in schematic, silkscreen, firmware, and documentation, so that control mappings do not depend on ambiguous A/B/C/D net names.
25. As a user, I want encoder direction invertible in configuration, so that clockwise behavior can be corrected without PCB rework.
26. As a firmware developer, I want the existing matrix diode direction and row/column arrangement retained when continuity and scan logic agree, so that cosmetic changes do not introduce risk.
27. As Moksh, I want a fresh PCB assembly STEP export after corrections, so that mechanical evidence reflects the actual validated board.
28. As Moksh, I want mounting holes, display placement, encoder clearance, sockets, and USB access reviewed now, so that deferring the enclosure does not hide board-level mechanical defects.
29. As Moksh, I want a final manufacturing checklist and an explicit human sign-off, so that automation never silently authorizes fabrication.
30. As Moksh, I want the evidence package suitable for both Macondo and Stardance, so that the same truthful technical record can support both programs.
31. As Moksh, I want to write journals in my own words with authentic screenshots, photos, and time records, so that the submissions preserve my authorship.
32. As a reviewer, I want no fabricated evidence or double-counted work hours, so that the project record is trustworthy.
33. As a user, I want one macOS installation containing Control Center and the Blip Agent, so that Blip behaves like one product.
34. As a user, I want the Blip Agent to continue operating when Control Center is closed, so that physical controls and timers remain reliable.
35. As a user, I want an explicit, reversible login-item setup, so that background operation happens only with my consent.
36. As a user, I want to choose a physical or virtual Blip Device during setup, so that I can use the product before hardware arrives.
37. As a developer, I want virtual and physical device events to enter the same Agent boundary, so that simulator success predicts hardware behavior.
38. As a developer, I want the simulator to emit switch down/up, encoder rotation, and encoder press events, so that complete interactions can be tested.
39. As a developer, I want the simulator to model latency, stale heartbeats, malformed messages, incompatible versions, disconnections, and action failures, so that failure behavior is testable.
40. As a developer, I want a simulator CLI, so that end-to-end acceptance tests can run without manually operating the UI.
41. As a user, I want simulated state clearly labeled, so that I never mistake it for a connected physical device.
42. As a device developer, I want versioned newline-delimited JSON with handshakes, sequence numbers, timestamps, heartbeats, and acknowledgements, so that firmware and Agent communication is observable and recoverable.
43. As a user, I want malformed or unknown device messages handled safely, so that one bad packet cannot execute an unintended action.
44. As a user, I want Offline Controls executed exactly once by firmware, so that the Agent does not duplicate volume or media commands.
45. As a user, I want right-encoder rotation to control volume and its normal press to toggle play/pause, so that essential media controls work without the Agent.
46. As a user, I want an active Pomodoro alarm to temporarily make right-encoder press dismiss the alarm, so that dismissal is immediate and physical.
47. As a user, I want SW11 to cycle Main, Coding, and Research even while offline, so that the current Profile remains available.
48. As a user, I want SW12 and profile-dependent actions to report clearly when the Agent is offline, so that failed host actions are not silent.
49. As a user, I want the left encoder to browse contextual choices without activating them until pressed, so that rotation cannot accidentally launch an action.
50. As a user, I want the Context Browser configurable or disableable per Profile, so that each workflow can use the left encoder appropriately.
51. As a user, I want exactly three stable Profiles with editable presentation and mappings, so that configuration remains flexible without making physical Profile switching unpredictable.
52. As a user, I want SW11 and SW12 mappings locked, so that imports cannot remove recovery and navigation controls.
53. As a user, I want the Blip Agent to own the authoritative Blip Configuration in SQLite, so that all interfaces see consistent state.
54. As a user, I want validated JSON import and export without credentials, so that I can back up configuration safely.
55. As a user, I want unknown imported action types preserved but disabled, so that forward-compatible data is not silently discarded or executed.
56. As a user, I want integration secrets and sessions stored in Keychain, so that backups and logs do not leak them.
57. As a user, I want Control Center to communicate with the Agent through a current-user-only Unix-domain socket, so that no network port is exposed.
58. As a user, I want clear Agent restart and diagnostic recovery, so that a broken background connection is understandable.
59. As a user, I want code-defined, versioned action types with schemas, validation, permissions, dry runs, and documentation, so that configurable actions remain safe.
60. As a user, I want imported or newly configured risky actions previewed and approved before physical execution, so that configuration cannot surprise me.
61. As a user, I want destructive or externally visible actions to require confirmation, so that a hardware press cannot accidentally publish or destroy work.
62. As a user, I want every action execution recorded in a redacted structured audit, so that I can understand what Blip did.
63. As a user, I want dry-run and Test modes to show intended effects without performing them, so that mappings can be verified safely.
64. As a user, I want the small graphical display to show Persistent Status, so that Pomodoro, media, and connection state remain glanceable.
65. As a user, I want the large character display to show Contextual Status, so that active Profiles, choices, progress, results, and errors are readable.
66. As a user, I want safety errors, alarms, progress, Context Browser, normal status, and Ambient Status to follow one priority system, so that important information is not overwritten.
67. As a user, I want useful Ambient Status such as time, date, weather, current project, and connection health only when space is available, so that idle displays remain useful without becoming distracting.
68. As a user, I want Open-Meteo weather for a manually selected city with caching, units, attribution, and stale markers, so that weather works without location permission or an API key.
69. As a user, I want semantic display scenes rendered locally by firmware, so that animation remains smooth without streaming frames over serial.
70. As a user, I want unknown animation identifiers to fall back to static content, so that protocol-version differences remain usable.
71. As a user, I want reduced-motion and animation timing controls, so that display motion matches my preference.
72. As a user, I want Pomodoro state to survive Control Center closure, Agent restart, device disconnection, and Mac sleep, so that the timer remains trustworthy.
73. As a user, I want pause, reset, Add 5 Minutes, and explicit work/break transitions, so that I control session timing.
74. As a user, I want a macOS notification and configurable one-shot alarm at each phase transition, so that I notice completion away from the device.
75. As a user, I want Test Alarm and notification-permission guidance, so that I can verify alerts before relying on them.
76. As a user, I want physical displays to show timer completion even when Focus suppresses a Mac notification, so that DND cannot hide the event entirely.
77. As a user, I want a guided Apple Shortcut integration for Focus/DND, so that Blip uses supported macOS automation rather than private APIs.
78. As a user, I want Focus Mode to compose Pomodoro, media, workspace opening, and optional DND automation with partial-failure reporting, so that one failed step does not cancel everything.
79. As a user, I want one Active Media Source at a time, so that Spotify, Telegram, and system playback do not compete.
80. As a non-Premium Spotify user, I want Blip to open saved Spotify links and rely on system media controls, so that the initial integration works without unavailable Web API playback.
81. As an Arc user, I want explicit current-tab Research capture with clipboard restoration and permission-aware screenshot support, so that Blip does not depend on Chrome.
82. As a user, I want operational logs rotated and sensitive data redacted, so that diagnostics do not become indefinite surveillance.
83. As a user, I want storage reported and clearable by category, so that cache removal does not accidentally delete configuration or audits.
84. As a user, I want a hardware-centered Control Center with live display previews and direct key-map Test/Edit modes, so that configuration matches the physical object.
85. As a user, I want a polished macOS visual language with purposeful motion and coherent profile colors, so that Control Center feels intentionally designed rather than generically generated.
86. As a user, I want system light/dark appearance and accessible reduced motion, so that Control Center respects macOS preferences.
87. As a developer, I want a throwaway interactive UI prototype reviewed against real workflows before production implementation, so that visual decisions are tested cheaply.
88. As a developer, I want all executable code centralized under one source root while hardware and review artifacts stay in focused top-level areas, so that the repository is easy to navigate.
89. As a firmware developer, I want manual PlatformIO flashing in the Foundation Release, so that configuration work is not coupled to risky firmware update machinery.
90. As a user, I want firmware and protocol compatibility displayed clearly, so that an incompatible device produces setup guidance instead of undefined behavior.
91. As a user, I want permissions requested only when I enable dependent features, so that basic Blip operation does not demand unnecessary access.
92. As a developer, I want Foundation scope completed before Telegram, full Spotify Web API, Codex Feature Forge, and deep Research extraction, so that external integrations do not obscure the core loop.

## Implementation Decisions

- Work is divided into an urgent Hardware Validation Gate and a parallel simulator-first Foundation Release. Both converge on one reviewed hardware pin map and one versioned device protocol.
- The Hardware Validation Gate is a hard prerequisite for PCB approval or manufacturing. Simulator software is not blocked by that gate.
- Only the three active KiCad source documents become authoritative. Generated history, backups, locks, local settings, and unrelated projects are excluded.
- Hardware revision one uses the exact Seeed Studio XIAO ESP32-S3. Symbol, footprint, pin names, BOM, firmware target, USB behavior, and documentation must agree.
- PCB corrections are minimal and evidence-driven. Existing placement, routing, matrix topology, and diode orientation remain unless validation demonstrates a concrete defect.
- The hardware audit covers power, I2C addressing and pull-ups, display mode/control pins, decoupling, worst-case current, matrix continuity, encoder identity, connector orientation, sockets, clearances, USB-C access, mounting features, and silkscreen.
- The Newhaven display uses 3.3-volt I2C operation at `0x3D`; its supply, reset, regulator-selection, and interface-selection pins receive documented fixed levels. The graphical OLED uses `0x3C` on the shared bus.
- Revision one is powered only from the XIAO USB-C connection. Both displays and the MCP23017 use its regulated 3.3-volt rail. Battery, charging, external power input, and Wi-Fi dependence are excluded.
- The XIAO uses female headers, the MCP23017 uses a DIP socket, and both displays use removable connections. Switches, diodes, encoders, and passives are normally soldered.
- The Hardware Change Guide is the single ordered hardware record. It contains the defect or warning, primary evidence, smallest correction, exact KiCad interaction, annotated visuals, before/after values, automated result, and human verification status.
- A fresh assembly STEP export and final visual checklist are required after electrical corrections. Enclosure design remains parallel and non-blocking.
- The Blip Device scans inputs, provides Offline Controls, stores hardware settings and current Profile, transports events, renders semantic scenes, and maintains offline display behavior.
- The Blip Agent owns Blip Configuration, SQLite, Keychain access, serial connection, action execution, Pomodoro, display composition, logs, audits, and long-running state.
- Control Center edits and observes Agent state through versioned private local IPC. Closing its window does not stop the Agent.
- Control Center and the Agent ship as one macOS product with explicit login-item consent. The first target is Apple Silicon on macOS 14 or newer, with primary validation on macOS 26.4.1 build 25E253.
- Physical serial and simulated transport adapt into one device-event interface. Downstream Profile, action, Pomodoro, display, audit, and UI behavior is transport-independent.
- Device serial uses versioned newline-delimited JSON with capability handshake, firmware and protocol versions, monotonic sequence numbers, device-relative timestamps, heartbeats, acknowledgement where delivery matters, bounded message sizes, and safe unknown-message handling.
- The simulator supports all physical input types, display output, connection identity, protocol negotiation, configurable latency, stale heartbeats, malformed input, version mismatch, disconnection, and injected action failure. It exposes a CLI for automated scenarios.
- Offline Controls execute once in firmware and are reported observationally to the Agent. The right encoder owns volume and play/pause; an active Pomodoro alarm temporarily overrides press for dismissal.
- SW11 cycles the device-owned stable Profiles `main`, `coding`, and `research`. SW12 and all profile-dependent controls require the Agent. Profile identities, count, and ordering are immutable in revision one.
- Both left-encoder inputs are configurable Agent controls. The default Context Browser previews on rotation, activates on press, and expires back to normal status. Direction inversion is supported for both encoders.
- The Agent’s SQLite database is authoritative. JSON import/export is validated interchange, not a second live source. Keychain-backed data is never exported or sent to firmware.
- Actions come from a code-defined versioned registry. Each type declares its stable identity, schema, defaults, validation, permissions, dry-run behavior, executor, display feedback, and documentation.
- Imported configurations never execute during import. Unknown types remain visible but disabled. Risky new actions require approval; destructive and externally visible steps require confirmation and structured audit records.
- One display composer owns priority, expiry, replacement, coalescing, acknowledgement, and reserved regions. Firmware receives compact semantic scenes and renders display-specific animation locally.
- Persistent Status belongs on the graphical OLED. Contextual Status belongs on the Newhaven character OLED. Ambient Status appears only below higher-priority content.
- Pomodoro state is reconstructed from persisted phase, remaining duration, and target time. Sleep counts as elapsed time, but restart never silently cascades through several automatic phases.
- Pomodoro transitions create a notification and optional one-shot alarm. DND setup uses user-created Apple Shortcuts and supported notification settings; failures degrade independently and remain visible on the device.
- Only basic macOS actions, Pomodoro, display state, Profile control, simulator behavior, configuration, logs, permissions, and the complete event loop are required for Foundation acceptance.
- Local non-Premium Spotify links, Arc-first explicit Research capture, Open-Meteo Ambient Status, Codex CLI execution, Feature Forge, and Telegram are modular follow-on slices. Only Open-Meteo Ambient Status and simple system media behavior may enter Foundation if they do not delay the core acceptance seam.
- Codex integration, when implemented, uses stable non-interactive CLI JSONL output, existing CLI-managed authentication, read-only planning/review, workspace-scoped implementation, explicit cancellation, and redacted audits.
- Feature Forge, when implemented, uses isolated Git worktrees and separate approvals for commit and publication. It does not force-push, auto-merge, or modify the user’s active working tree.
- Operational logs rotate at 30 days or 100 MB by default. Sensitive credentials, phone numbers, clipboard/page contents, and sensitive URL parameters are redacted. Storage categories have separate clearing controls.
- The Control Center uses a hardware-instrument macOS design language with restrained graphite and warm off-white foundations, stable amber/blue/teal Profile accents, coherent line icons, system typography, accessible motion, and no decorative AI-generated visual tropes.
- Executable source is centralized beneath one source root in separate Control Center, Agent, shared library, protocol, storage, macOS integration, sidecar, and firmware modules. Hardware, CAD, BOM, documentation, feature requests, and scripts remain reviewer-facing top-level concerns.
- Foundation firmware is installed manually with PlatformIO. Configuration never compiles or flashes firmware. Any in-app updater is deferred.

## Testing Decisions

- Tests assert externally observable behavior rather than private implementation details. A passing unit test cannot substitute for either release-level acceptance seam.
- The primary hardware seam starts with the authoritative KiCad project and ends with generated ERC, DRC, schematic-parity, source-consistency, mechanical-export, and Hardware Change Guide evidence plus a named human visual sign-off.
- Hardware automation must fail on unexplained ERC/DRC findings, unconnected items, skipped parity, mismatched controller identity, conflicting pin maps, missing required BOM/socket items, missing evidence, or a stale STEP export.
- Datasheet-derived electrical facts are recorded with source and revision. Tests verify the project values against the recorded facts rather than scraping mutable web pages during every run.
- Hardware checks include I2C addresses, bus pull-ups, Newhaven control levels, 3.3-volt power topology, current-budget inputs, decoupling inventory, matrix nets and diode orientation, encoder identity, and no-connect dispositions.
- Visual and mechanical review remains human because automated KiCad checks cannot prove connector accessibility, assembly orientation, readable silkscreen, or practical installed clearance.
- The primary Foundation seam drives the same scenario first through the simulator and later through physical serial: connect, negotiate protocol, emit an input, resolve ownership and Profile, validate or dry-run an action, produce an audit result, compose both displays, and observe state through Control Center IPC.
- The Foundation acceptance suite runs headlessly through the simulator CLI. Physical-device runs reuse scenario definitions and replace only the transport adapter.
- Contract tests cover protocol negotiation, sequences, acknowledgements, reconnects, heartbeat expiry, message limits, malformed JSON, unknown messages, unsupported capabilities, and incompatible versions.
- Behavioral tests verify that Offline Controls are never re-executed by the Agent, Agent-owned actions never execute in firmware, and reconnection reconciles Profile state without duplicate actions.
- Persistence tests use real SQLite boundaries to verify transactional configuration changes, migration rollback, import preview, unknown disabled actions, Pomodoro reconstruction, current Profile, and retained Active Media Source without automatic playback.
- IPC tests start the real Agent boundary and verify current-user access, Control Center disconnect/reconnect, version mismatch, restart recovery, and clean diagnostics.
- Action tests execute through the registry boundary in Test or Dry Run mode. They assert validation, permissions, confirmation requirements, approved roots, redaction, result state, display feedback, and audit records.
- Display tests assert semantic scenes and priority transitions rather than pixel-perfect implementation. Adapter snapshots verify the small graphical and large character representations, unknown-animation fallback, expiry, acknowledgement, coalescing, and reduced motion.
- Pomodoro tests use a controllable clock and cover pause, reset, add-time, sleep/wake, Agent restart, disconnection/reconnection, phase completion, alarm override, notification denial, DND suppression warnings, and avoidance of multi-phase cascades.
- Simulator fault tests verify that simulated identity is visible and that latency, failure, stale heartbeat, disconnect, and incompatibility states produce the same recovery experience expected from physical hardware.
- Control Center workflow tests cover onboarding, Agent state, simulator selection, hardware dashboard, Profile mapping, Test/Edit modes, display previews, permission guidance, import/export preview, logs, storage clearing, and diagnostic export.
- Accessibility and visual review cover light/dark mode, keyboard navigation, VoiceOver labels, contrast, reduced motion, layout at supported window sizes, animation purpose, and adherence to the approved visual language.
- Packaging smoke tests cover first launch, login-item consent, Control Center closure while Agent continues, Agent disable/re-enable, uninstall guidance, manual firmware-version guidance, and operation on the primary target Mac.
- The current repository provides firmware compilation as prior art, but it does not yet contain the shared simulator/Agent seam. That seam is intentionally introduced once and becomes the principal test boundary for all later integrations.
- Later Spotify, Telegram, Arc, weather, Codex, and Feature Forge slices must pass their provider-contract tests and then reuse the Foundation acceptance seam; they must not create parallel input, action, or display pipelines.

## Out of Scope

- Manufacturing the PCB before the Hardware Validation Gate and human review pass.
- Recreating the PCB, aesthetic rerouting, speculative topology changes, or replacing validated existing circuitry without evidence.
- A finished enclosure as a prerequisite for initial PCB approval; only board-level mechanical readiness is required.
- Battery power, charging, external power input, wireless operation, or Wi-Fi-dependent core behavior.
- Controllers other than the XIAO ESP32-S3 and Intel Mac support in revision one.
- Custom Profile creation, deletion, duplication, or ordering.
- In-app firmware building, flashing, background updating, or silent firmware updates.
- Runtime executable plugins or configuration-injected code.
- Full Spotify Web API playback without Premium, Telegram production access, a full browser extension, and automated full-page browser extraction in the Foundation Release.
- Feature Forge publication, force-push, auto-merge, or modification of the user’s active Git worktree in the Foundation Release.
- Private macOS Focus APIs, UI scraping for DND, hidden background persistence, or bypassing system permissions.
- General-purpose display widgets, arbitrary streamed animation frames, fake external credentials, or fake simulator state presented as hardware.
- Signing, notarization, production distribution, and automatic updates before the unsigned Apple Silicon development build is proven.
- AI-written journals, fabricated screenshots, fabricated hours, or automated final approval on Moksh’s behalf.

## Further Notes

- Recommended delivery order is: preserve and migrate authoritative hardware sources; establish the reviewed pin-map contract; build the Hardware Change Guide; resolve or disposition ERC/DRC/parity findings; export mechanical evidence; complete human review; and only then release manufacturing files.
- In parallel, establish the shared protocol and semantic event/display contracts; implement the simulator and Agent acceptance harness; build persistence, actions, Pomodoro, and IPC; add firmware transport/rendering; then build and visually validate Control Center.
- The simulator is not a throwaway mock. It is a supported virtual Blip Device and the permanent highest-level software test adapter.
- The UI prototype is throwaway. Its validated design decisions enter the production design system, while prototype code is discarded.
- Hardware evidence should distinguish automated pass, documented engineering justification, and required human observation. A screenshot of a green dialog alone is not sufficient evidence if tests were excluded or parity was not run.
- Any discovery that materially changes the locked controller, power source, display topology, matrix, physical control ownership, or two-seam architecture requires a new architectural decision before implementation continues.
