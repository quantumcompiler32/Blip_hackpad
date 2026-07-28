import {
  DEVICE_PROTOCOL_VERSION,
  type AgentObservation,
  type DeviceEventBoundary,
  type DeviceEventEnvelope,
  type SemanticDisplays,
} from "../shared/device-events.ts";

export class BlipAgent implements DeviceEventBoundary {
  acceptDeviceEvent(envelope: DeviceEventEnvelope): AgentObservation {
    if (envelope.protocolVersion !== DEVICE_PROTOCOL_VERSION) {
      throw new Error(`Unsupported device protocol ${envelope.protocolVersion}`);
    }

    const event = envelope.event;
    const persistentDisplay: SemanticDisplays["small"] = {
      role: "persistent",
      connection: envelope.device.origin,
      title:
        envelope.device.origin === "virtual"
          ? "BLIP VIRTUAL"
          : "BLIP DEVICE",
      status: "Connected",
    };

    if (
      event.kind === "switch" &&
      event.control === "SW01" &&
      event.phase === "down"
    ) {
      return {
        event: envelope,
        actionResult: {
          actionId: "foundation.open-control-center",
          mode: "dry-run",
          status: "succeeded",
          description: "Would open Control Center",
          executed: false,
        },
        auditEntry: {
          id: `${envelope.device.id}:${envelope.sequence}`,
          origin: envelope.device.origin,
          deviceId: envelope.device.id,
          sequence: envelope.sequence,
          eventKind: event.kind,
          control: event.control,
          result: "dry-run-succeeded",
          timestampMs: envelope.timestampMs,
        },
        displays: {
          small: persistentDisplay,
          large: {
            role: "contextual",
            title: "DRY RUN COMPLETE",
            detail: "SW01",
            message: "Open Control Center",
            status: "success",
          },
        },
      };
    }

    return {
      event: envelope,
      actionResult: null,
      auditEntry: null,
      displays: {
        small: persistentDisplay,
        large: {
          role: "contextual",
          title: "MAIN",
          detail: event.control,
          message: "Ready",
          status: "idle",
        },
      },
    };
  }
}
