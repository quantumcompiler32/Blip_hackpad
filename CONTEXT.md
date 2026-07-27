# Blip

Blip is a configurable physical control surface for macOS. Its embedded device captures physical input and presents glanceable status, while its host software performs computer- and service-dependent behavior.

## Language

**Blip Device**:
The physical macropad, including its XIAO controller, switches, encoders, and displays.
_Avoid_: Pad, controller, firmware

**Blip Controller**:
The Seeed Studio XIAO ESP32-S3 module that runs the Blip Device firmware.
_Avoid_: Seeeduino, generic XIAO, RP2040

**Blip Agent**:
The background macOS component that connects the Blip Device to host applications, services, and automations.
_Avoid_: Daemon, backend, macro agent

**Control Center**:
The macOS interface through which a person configures and observes Blip.
_Avoid_: Configuration UI, dashboard app

**Blip Configuration**:
The authoritative collection of profiles, actions, projects, integrations, and presentation preferences owned by the Blip Agent.
_Avoid_: Firmware config, settings file

**Profile**:
One of Blip’s three stable operating contexts—Main, Coding, or Research—that determines profile-dependent controls and Contextual Status.
_Avoid_: Layer, mode, workspace

**Fixed Control**:
A physical input whose mapping is identical across every profile. A Fixed Control does not necessarily work while the Blip Agent is unavailable.
_Avoid_: Permanent key, global key

**Offline Control**:
A physical input whose essential behavior is executed by the Blip Device when the Blip Agent is unavailable.
_Avoid_: Hardcoded macro

**Persistent Status**:
The always-relevant Pomodoro, media, and connection information shown on the small graphical display.
_Avoid_: Small-display page

**Active Media Source**:
The single Spotify, Telegram, or system media session that Blip currently presents and controls.
_Avoid_: Player, music mode

**Contextual Status**:
Information about the active profile, action, or workflow shown on the large character display.
_Avoid_: Large-display page

**Ambient Status**:
Useful environmental or decorative information shown only when space and attention are not needed by Persistent Status or Contextual Status.
_Avoid_: Screensaver, filler

**Context Browser**:
The left encoder interaction in which rotation previews profile-relevant choices and pressing activates the highlighted choice.
_Avoid_: Encoder menu, rotary menu

**Foundation Release**:
The first usable Blip milestone that proves the complete physical-input-to-macOS-action and status-feedback loop before external service integrations are added.
_Avoid_: MVP, prototype

**Hardware Validation Gate**:
The evidence required before Blip’s PCB is submitted for approval or manufacturing, showing that its design, component requirements, and firmware-facing pin map agree.
_Avoid_: Final check, PCB review

**Hardware Change Guide**:
The single visual, ordered record of required PCB-project corrections and the evidence that each correction passes the Hardware Validation Gate.
_Avoid_: Fix list, PCB notes
