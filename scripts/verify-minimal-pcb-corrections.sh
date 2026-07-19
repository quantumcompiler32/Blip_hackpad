#!/bin/sh
set -eu

root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
sch="$root/PCB/Hackpad.kicad_sch"
pcb="$root/PCB/Hackpad.kicad_pcb"
bom="$root/BOM/Hackpad_final_BOM.csv"
contract="$root/docs/hardware/pin-map-contract.md"
symbol_library="$root/PCB/Blip.kicad_sym"
footprint_library="$root/PCB/Blip.pretty/XIAO_ESP32S3_Socket.kicad_mod"

fail() {
  printf '%s\n' "hardware correction check failed: $1" >&2
  exit 1
}

for file in "$sch" "$pcb" "$bom" "$contract" "$symbol_library" "$footprint_library"; do
  test -f "$file" || fail "missing ${file#"$root/"}"
done

grep -q 'Blip:XIAO_ESP32S3_Socket' "$sch" || fail "schematic does not use the socketed XIAO ESP32-S3 footprint"
grep -q 'Blip:XIAO_ESP32S3_Socket' "$pcb" || fail "PCB does not use the socketed XIAO ESP32-S3 footprint"
grep -q 'Seeed Studio XIAO ESP32-S3' "$sch" || fail "schematic controller identity is not exact"
grep -q 'GPIO1_A0_D0' "$sch" || fail "schematic controller pins are not ESP32-S3 pins"

u1=$(awk '
  /\(footprint "Blip:XIAO_ESP32S3_Socket"/ { inside = 1 }
  inside { print }
  inside && /\(embedded_fonts no\)/ { exit }
' "$pcb")
test "$(printf '%s\n' "$u1" | grep -Ec '\(pad "([1-9]|1[0-4])" thru_hole')" -eq 14 ||
  fail "U1 must expose exactly fourteen through-hole socket contacts"
test "$(printf '%s\n' "$u1" | grep -Ec '\(at (-?7\.62) ' )" -eq 14 ||
  fail "U1 socket rows must use Seeed's official 15.24 mm center spacing"
printf '%s\n' "$u1" | grep -q '(pad "15"' && fail "obsolete XIAO underside pads remain"

ds1=$(awk '
  /\(footprint "NHD-0420CW-AB3:LCD_NHD-0420CW-AB3"/ { inside = 1 }
  inside { print }
  inside && /\(embedded_fonts no\)/ { exit }
' "$pcb")
printf '%s\n' "$ds1" | awk '
  /\(pad "3"/ { pin = 3 }
  /\(pad "4"/ { pin = 4 }
  /\(pad "16"/ { pin = 16 }
  pin == 3 && /\(net "GND"\)/ { regvdd = 1 }
  pin == 4 && /\(net "\+3.3V"\)/ { sa0 = 1 }
  pin == 16 && /\(net "\+3.3V"\)/ { reset_high = 1 }
  END { exit !(regvdd && sa0 && reset_high) }
' || fail "Newhaven REGVDD/SA0/reset nets are incorrect"

if grep -Eq 'XIAO_RP2040|MOUDLE-SEEEDUINO-XIAO|MCU_Seeed_ESP32C3|PA02_A0_D0' "$sch" "$pcb" "$bom"; then
  fail "obsolete XIAO identity remains"
fi

grep -q 'XIAO_ESP32S3_Socket' "$bom" || fail "BOM footprint does not match the socketed controller"
grep -q 'SSW-120-01-T-S' "$bom" || fail "Newhaven removable socket is missing from BOM"
grep -q 'TSW-120-07-T-S' "$bom" || fail "Newhaven mating header is missing from BOM"
grep -q 'SSW-104-01-T-S' "$bom" || fail "graphical OLED removable socket is missing from BOM"
grep -q 'TSW-104-07-T-S' "$bom" || fail "graphical OLED mating header is missing from BOM"
grep -q 'REGVDD.*GND' "$contract" || fail "REGVDD correction is not documented"
grep -q 'SA0.*3.3 V' "$contract" || fail "SA0 high / 0x3D correction is not documented"
grep -q 'DM-OLED096-636.*0x3C' "$contract" || fail "graphical OLED address contract is missing"
grep -q 'NHD-0420CW-AB3.*0x3D' "$contract" || fail "Newhaven OLED address contract is missing"

printf '%s\n' 'Minimal PCB correction contract is satisfied.'
