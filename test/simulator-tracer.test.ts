import assert from "node:assert/strict";
import { spawnSync } from "node:child_process";
import { existsSync } from "node:fs";
import test from "node:test";

import { BlipAgent } from "../src/blip-agent/blip-agent.ts";
import { VirtualBlipDevice } from "../src/simulator/virtual-blip-device.ts";

test("a virtual switch action produces a safe dry-run result, audit, and both semantic displays", () => {
  const agent = new BlipAgent();
  const device = new VirtualBlipDevice(agent);

  const observation = device.switchDown("SW01");

  assert.deepEqual(observation.actionResult, {
    actionId: "foundation.open-control-center",
    mode: "dry-run",
    status: "succeeded",
    description: "Would open Control Center",
    executed: false,
  });
  assert.ok(observation.auditEntry);
  assert.equal(observation.auditEntry.origin, "virtual");
  assert.equal(observation.auditEntry.control, "SW01");
  assert.equal(observation.auditEntry.result, "dry-run-succeeded");
  assert.deepEqual(observation.displays.small, {
    role: "persistent",
    connection: "virtual",
    title: "BLIP VIRTUAL",
    status: "Connected",
  });
  assert.deepEqual(observation.displays.large, {
    role: "contextual",
    title: "DRY RUN COMPLETE",
    detail: "SW01",
    message: "Open Control Center",
    status: "success",
  });
});

test("virtual switch and encoder interactions all enter the Agent through the device-event boundary", () => {
  const agent = new BlipAgent();
  let nowMs = 1_000;
  const device = new VirtualBlipDevice(agent, () => nowMs);

  nowMs = 1_010;
  const switchObservation = device.switchUp("SW01");
  nowMs = 1_025;
  const rotationObservation = device.rotateEncoder("left", -1);
  nowMs = 1_040;
  const pressObservation = device.pressEncoder("right");
  nowMs = 1_055;
  const releaseObservation = device.releaseEncoder("right");

  const observations = [
    switchObservation,
    rotationObservation,
    pressObservation,
    releaseObservation,
  ];

  assert.deepEqual(
    observations.map(({ event }) => ({
      protocolVersion: event.protocolVersion,
      sequence: event.sequence,
      timestampMs: event.timestampMs,
      origin: event.device.origin,
      event: event.event,
    })),
    [
      {
        protocolVersion: 1,
        sequence: 1,
        timestampMs: 10,
        origin: "virtual",
        event: { kind: "switch", control: "SW01", phase: "up" },
      },
      {
        protocolVersion: 1,
        sequence: 2,
        timestampMs: 25,
        origin: "virtual",
        event: { kind: "encoder-rotation", control: "left", delta: -1 },
      },
      {
        protocolVersion: 1,
        sequence: 3,
        timestampMs: 40,
        origin: "virtual",
        event: { kind: "encoder-press", control: "right", phase: "down" },
      },
      {
        protocolVersion: 1,
        sequence: 4,
        timestampMs: 55,
        origin: "virtual",
        event: { kind: "encoder-press", control: "right", phase: "up" },
      },
    ],
  );
  assert.ok(observations.every(({ actionResult }) => actionResult === null));
});

test("the scenario CLI demonstrates the complete tracer path as machine-readable JSON", () => {
  const result = spawnSync(
    process.execPath,
    [
      "--experimental-strip-types",
      "src/simulator/scenario-cli.ts",
      "--format",
      "json",
    ],
    { cwd: process.cwd(), encoding: "utf8" },
  );

  assert.equal(result.status, 0, result.stderr);
  const scenario = JSON.parse(result.stdout);
  assert.equal(scenario.scenario, "foundation-switch-dry-run");
  assert.equal(scenario.device.label, "Virtual Blip Device");
  assert.equal(scenario.event.event.kind, "switch");
  assert.equal(scenario.event.event.control, "SW01");
  assert.equal(scenario.actionResult.executed, false);
  assert.equal(scenario.auditEntry.result, "dry-run-succeeded");
  assert.equal(scenario.displays.small.role, "persistent");
  assert.equal(scenario.displays.large.role, "contextual");
});

test("the Control Center diagnostic view labels simulation and shows action, audit, and display state", () => {
  const result = spawnSync(
    process.execPath,
    [
      "--experimental-strip-types",
      "src/simulator/scenario-cli.ts",
      "--format",
      "html",
    ],
    { cwd: process.cwd(), encoding: "utf8" },
  );

  assert.equal(result.status, 0, result.stderr);
  assert.match(result.stdout, /<title>Blip Control Center Diagnostic<\/title>/);
  assert.match(result.stdout, />Virtual Blip Device</);
  assert.match(result.stdout, />Simulation</);
  assert.match(result.stdout, />Would open Control Center</);
  assert.match(result.stdout, />dry-run-succeeded</);
  assert.match(result.stdout, /aria-label="Small display: Persistent Status"/);
  assert.match(result.stdout, /aria-label="Large display: Contextual Status"/);
  assert.match(result.stdout, />DRY RUN COMPLETE</);
});

test("all executable projects are centralized under the source root", () => {
  assert.equal(existsSync("Firmware/platformio.ini"), false);
  assert.equal(existsSync("Firmware/src/main.cpp"), false);
  assert.equal(existsSync("Firmware/include/Config.h"), false);
  assert.equal(existsSync("src/firmware/platformio.ini"), true);
  assert.equal(existsSync("src/blip-agent/blip-agent.ts"), true);
  assert.equal(existsSync("src/control-center/diagnostic-view.ts"), true);
  assert.equal(existsSync("src/simulator/scenario-cli.ts"), true);
});
