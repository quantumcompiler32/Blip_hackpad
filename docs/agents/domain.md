# Domain Docs

How engineering skills should consume this repository’s domain documentation.

## Before exploring, read these

- `CONTEXT.md` at the repository root.
- `CONTEXT-MAP.md` if it exists, followed by each relevant context document.
- ADRs under `docs/adr/` that affect the work.

If these files do not exist, proceed silently. Domain-modeling skills create them lazily as terms and decisions are resolved.

## File structure

This is a single-context repository:

```
/
├── CONTEXT.md
├── docs/adr/
└── source directories
```

## Use the glossary’s vocabulary

Use domain terms as defined in `CONTEXT.md`. Avoid synonyms the glossary explicitly rejects. If a necessary concept is absent, reconsider the new terminology or note the gap for `/domain-modeling`.

## Flag ADR conflicts

Explicitly identify output that contradicts an existing ADR instead of silently overriding it.
