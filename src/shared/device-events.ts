export const DEVICE_PROTOCOL_VERSION = 1 as const;

export type DeviceOrigin = "virtual" | "physical";
export type SwitchControl =
  | "SW01"
  | "SW02"
  | "SW03"
  | "SW04"
  | "SW05"
  | "SW06"
  | "SW07"
  | "SW08"
  | "SW09"
  | "SW10"
  | "SW11"
  | "SW12";
export type EncoderControl = "left" | "right";
export type InputPhase = "down" | "up";

export type DeviceEvent =
  | {
      kind: "switch";
      control: SwitchControl;
      phase: InputPhase;
    }
  | {
      kind: "encoder-rotation";
      control: EncoderControl;
      delta: number;
    }
  | {
      kind: "encoder-press";
      control: EncoderControl;
      phase: InputPhase;
    };

export interface DeviceEventEnvelope {
  protocolVersion: typeof DEVICE_PROTOCOL_VERSION;
  sequence: number;
  timestampMs: number;
  device: {
    id: string;
    label: string;
    origin: DeviceOrigin;
  };
  event: DeviceEvent;
}

export interface ActionResult {
  actionId: string;
  mode: "dry-run";
  status: "succeeded";
  description: string;
  executed: false;
}

export interface AuditEntry {
  id: string;
  origin: DeviceOrigin;
  deviceId: string;
  sequence: number;
  eventKind: DeviceEvent["kind"];
  control: SwitchControl | EncoderControl;
  result: "dry-run-succeeded";
  timestampMs: number;
}

export interface SemanticDisplays {
  small: {
    role: "persistent";
    connection: DeviceOrigin;
    title: string;
    status: string;
  };
  large: {
    role: "contextual";
    title: string;
    detail: string;
    message: string;
    status: "success" | "idle";
  };
}

export interface AgentObservation {
  event: DeviceEventEnvelope;
  actionResult: ActionResult | null;
  auditEntry: AuditEntry | null;
  displays: SemanticDisplays;
}

/**
 * The transport-independent Agent seam. Physical serial and the virtual
 * Blip Device both deliver validated envelopes through this boundary.
 */
export interface DeviceEventBoundary {
  acceptDeviceEvent(event: DeviceEventEnvelope): AgentObservation;
}
