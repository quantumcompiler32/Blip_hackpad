import {
  DEVICE_PROTOCOL_VERSION,
  type AgentObservation,
  type DeviceEvent,
  type DeviceEventBoundary,
  type DeviceEventEnvelope,
  type EncoderControl,
  type SwitchControl,
} from "../shared/device-events.ts";

export class VirtualBlipDevice {
  #sequence = 0;
  readonly #boundary: DeviceEventBoundary;
  readonly #now: () => number;
  readonly #startedAtMs: number;

  constructor(
    boundary: DeviceEventBoundary,
    now: () => number = Date.now,
  ) {
    this.#boundary = boundary;
    this.#now = now;
    this.#startedAtMs = now();
  }

  switchDown(control: SwitchControl): AgentObservation {
    return this.#emit({
      kind: "switch",
      control,
      phase: "down",
    });
  }

  switchUp(control: SwitchControl): AgentObservation {
    return this.#emit({
      kind: "switch",
      control,
      phase: "up",
    });
  }

  rotateEncoder(
    control: EncoderControl,
    delta: number,
  ): AgentObservation {
    if (!Number.isInteger(delta) || delta === 0) {
      throw new Error("Encoder rotation delta must be a non-zero integer");
    }

    return this.#emit({
      kind: "encoder-rotation",
      control,
      delta,
    });
  }

  pressEncoder(control: EncoderControl): AgentObservation {
    return this.#emit({
      kind: "encoder-press",
      control,
      phase: "down",
    });
  }

  releaseEncoder(control: EncoderControl): AgentObservation {
    return this.#emit({
      kind: "encoder-press",
      control,
      phase: "up",
    });
  }

  #emit(event: DeviceEvent): AgentObservation {
    const envelope: DeviceEventEnvelope = {
      protocolVersion: DEVICE_PROTOCOL_VERSION,
      sequence: ++this.#sequence,
      timestampMs: this.#now() - this.#startedAtMs,
      device: {
        id: "virtual-blip",
        label: "Virtual Blip Device",
        origin: "virtual",
      },
      event,
    };

    return this.#boundary.acceptDeviceEvent(envelope);
  }
}
