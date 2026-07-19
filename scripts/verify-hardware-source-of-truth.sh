#!/bin/sh
set -eu

root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
pcb="$root/PCB"

test "$(find "$pcb" -maxdepth 1 -type f -name 'Hackpad.kicad_*' | wc -l | tr -d ' ')" = "3"
test -f "$pcb/Hackpad.kicad_pro"
test -f "$pcb/Hackpad.kicad_sch"
test -f "$pcb/Hackpad.kicad_pcb"

if find "$pcb" \( -name '~*.lck' -o -name '*-backups' -o -name '*.kicad_prl' -o -name fp-info-cache -o -name history -o -name Starter \) -print -quit | grep -q .; then
  echo "Generated or unrelated KiCad artifact found under PCB." >&2
  exit 1
fi

test -f "$root/docs/hardware/pin-map-contract.md"
test -f "$root/docs/hardware/baseline-findings.md"
rg -q 'XIAO ESP32-S3' "$root/docs/hardware/pin-map-contract.md"
rg -q 'not passed' "$root/docs/hardware/baseline-findings.md"

echo "Hardware source-of-truth policy is satisfied."
