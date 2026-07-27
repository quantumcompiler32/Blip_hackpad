# Standardize on XIAO ESP32-S3

Blip hardware revision one supports the Seeed Studio XIAO ESP32-S3 as its sole controller. The schematic symbol, footprint, BOM, firmware environment, pin map, USB HID and serial behavior, and documentation must all identify that exact module rather than a generic XIAO, the original SAMD21 Seeeduino XIAO, or the XIAO RP2040. This resolves the repository’s current conflict between ESP32-S3 firmware and BOM data, SAMD21-style schematic pin names, and an ERC reference to a missing RP2040 symbol library.
