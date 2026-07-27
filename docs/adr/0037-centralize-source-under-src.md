# Centralize executable source under src

All Blip executable source lives under one root `src/` directory: Control Center, Blip Agent, shared Rust crates, TypeScript bindings, the later Telegram sidecar, and XIAO firmware. Hardware and reviewer artifacts remain in focused top-level `PCB/`, `CAD/`, `BOM/`, `docs/`, `feature-requests/`, and `scripts/` locations. Root documentation maps these paths explicitly for Macondo and Stardance reviewers, and the cohesive product continues to use one root domain context.
