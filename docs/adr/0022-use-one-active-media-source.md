# Use one Active Media Source

Blip manages one Active Media Source at a time: Spotify, Telegram, or the current system media session. Starting one Blip-managed source pauses the other when control is available, and right-encoder USB HID play/pause targets the Mac’s current media session; Telegram playback integrates with macOS media controls. Missing media fails gracefully with `NO MEDIA`, Persistent Status identifies the source and item, the last source is remembered without automatically resuming after Agent restart, and Focus Mode reports when its configured source cannot take control.
