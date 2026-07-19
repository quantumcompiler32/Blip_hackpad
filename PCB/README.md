# Blip PCB source of truth

The three `Hackpad.kicad_*` files in this directory are the authoritative revision-one KiCad project. They were migrated unchanged from Moksh's active project on 2026-07-18.

Do not commit KiCad locks, automatic backups, local preferences, history folders, cached footprint data, or the unrelated Starter project. Hardware corrections must be made to these sources and documented in the Hardware Change Guide.

Run `sh scripts/verify-hardware-source-of-truth.sh` to verify the durable source policy. The migration hashes are historical evidence in the baseline findings; they are intentionally not enforced after approved PCB edits. A passing source-policy check does not mean the Hardware Validation Gate has passed.
