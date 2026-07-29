#!/bin/sh
set -eu

root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
pcb="$root/PCB/Hackpad.kicad_pcb"
bom="$root/BOM/Hackpad_final_BOM.csv"

test -s "$pcb"
test -s "$bom"

for footprint in \
  'Blip:XIAO_ESP32S3_Socket' \
  'DM-OLED096-636:MODULE_DM-OLED096-636' \
  'NHD-0420CW-AB3:LCD_NHD-0420CW-AB3' \
  'Package_DIP:DIP-28_W7.62mm' \
  'Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder'; do
  grep -q "(footprint \"$footprint\"" "$pcb" || {
    printf 'missing PCB footprint: %s\n' "$footprint" >&2
    exit 1
  }
done

test "$(grep -c '(footprint "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB"' "$pcb")" -eq 12
test "$(grep -c '(footprint "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal"' "$pcb")" -eq 12
test "$(grep -c '(footprint "Rotary_Encoder:RotaryEncoder_Alps_EC11E-Switch_Vertical_H20mm_CircularMountingHoles"' "$pcb")" -eq 2
test "$(grep -c '(footprint "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder"' "$pcb")" -eq 2

ruby -rcsv -e '
  rows = CSV.read(ARGV.fetch(0), headers: true)
  abort "BOM has malformed rows" unless rows.all? { |row| row.headers.length == row.fields.length }
  abort "BOM must contain 16 assembly lines" unless rows.length == 16
  required = ["U1", "DS1", "U2", "U3", "SW13, SW14", "SW1-SW12", "D1-D12", "R1, R2", "C1"]
  refs = rows.map { |row| row["PCB/Schematic References"] }
  missing = required - refs
  abort "BOM is missing references: #{missing.join(", ")}" unless missing.empty?
  total = rows.sum { |row| row["Total Cost (USD)"].to_f }
  abort "BOM total is not $97.95" unless (total - 97.95).abs < 0.001
  puts "BOM matches the populated PCB footprint counts and assembly references (#{format("$%.2f", total)} before PCB fabrication and enclosure hardware)."
' "$bom"
