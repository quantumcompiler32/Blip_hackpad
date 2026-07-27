# Use USB-C-only power

Blip hardware revision one is powered only through the socketed XIAO ESP32-S3 USB-C connection, which also carries USB HID and serial communication. The MCP23017 and both displays use the XIAO’s regulated 3.3 V output; revision one has no battery, charger, external power input, or Wi-Fi-dependent operating mode. The Hardware Validation Gate includes a worst-case current budget, display-brightness assumptions, local-decoupling and bulk-capacitance review, USB-C mechanical access, and a warning not to drive the 3.3 V rail externally while USB is connected.
