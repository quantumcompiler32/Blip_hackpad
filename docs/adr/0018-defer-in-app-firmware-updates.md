# Defer in-app firmware updates

Control Center configuration changes never rebuild or reflash the Blip Device. The Foundation Release reports firmware and protocol versions, warns about incompatibility, and relies on documented manual PlatformIO flashing over USB-C for installation and developer updates. Any future in-app updater must use a verified firmware image, explicit confirmation, progress and recovery guidance, and must never update silently during normal operation.
