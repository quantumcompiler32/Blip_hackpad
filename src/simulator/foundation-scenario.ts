import { BlipAgent } from "../blip-agent/blip-agent.ts";
import type { AgentObservation } from "../shared/device-events.ts";
import { VirtualBlipDevice } from "./virtual-blip-device.ts";

export interface FoundationScenarioResult extends AgentObservation {
  scenario: "foundation-switch-dry-run";
  device: AgentObservation["event"]["device"];
}

export function runFoundationScenario(
  now: () => number = Date.now,
): FoundationScenarioResult {
  const agent = new BlipAgent();
  const device = new VirtualBlipDevice(agent, now);
  const observation = device.switchDown("SW01");

  return {
    scenario: "foundation-switch-dry-run",
    device: observation.event.device,
    ...observation,
  };
}
