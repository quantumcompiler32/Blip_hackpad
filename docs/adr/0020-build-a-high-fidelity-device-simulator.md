# Build a high-fidelity device simulator

The Foundation Release includes a clearly identified virtual Blip Device that emits switch down/up and encoder rotation/press events through the same Agent-side device-event boundary as serial hardware and renders both displays from the same display-state model. It supports connection and protocol failures, stale heartbeats, incompatible firmware, optional latency and action failure injection, Test and Dry Run behavior, and a CLI for automated event tests. Simulated state never masquerades as physical-device state or stores fake external credentials.
