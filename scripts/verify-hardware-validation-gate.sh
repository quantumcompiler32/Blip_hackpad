#!/bin/sh
set -eu

root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
guide="$root/docs/hardware/hardware-change-guide.md"
step="$root/CAD/Blip-PCB-Assembly.step"
erc="$root/docs/hardware/evidence/erc.rpt"
drc="$root/docs/hardware/evidence/drc.rpt"
results="$root/docs/hardware/evidence/validation-results.txt"

fail() {
  printf '%s\n' "hardware validation gate failed: $1" >&2
  exit 1
}

"$root/scripts/verify-minimal-pcb-corrections.sh" >/dev/null
sh "$root/scripts/verify-hardware-source-of-truth.sh" >/dev/null

for file in "$guide" "$step" "$erc" "$drc" "$results"; do
  test -s "$file" || fail "missing ${file#"$root/"}"
done

grep -q 'ERC PASS' "$guide" || fail "guide does not record the ERC disposition"
grep -q 'DRC PASS' "$guide" || fail "guide does not record the DRC disposition"
grep -q 'Schematic parity PASS' "$guide" || fail "guide does not record schematic parity"
grep -q 'Unconnected items PASS' "$guide" || fail "guide does not record unconnected items"
grep -q 'HUMAN SIGN-OFF REQUIRED' "$guide" || fail "human manufacturing decision is not reserved"
grep -Eq 'Found 0 violations|ERC messages: 0  Errors 0  Warnings 0' "$erc" || fail "ERC report is not clean"
grep -q 'Found 0 DRC violations' "$drc" || fail "DRC report is not clean"
grep -q 'Found 0 unconnected pads' "$drc" || fail "DRC report does not prove zero unconnected items"
grep -q 'KiCad GUI schematic parity: 0' "$results" || fail "machine-readable parity result is missing"

sed -n '/^SHA-256 source\/artifact binding:/,$p' "$results" | sed '1d' | (cd "$root" && shasum -a 256 -c -) >/dev/null || fail "evidence is stale relative to its bound source files"

printf '%s\n' 'Hardware Validation Gate evidence package is internally consistent; human sign-off remains required.'
