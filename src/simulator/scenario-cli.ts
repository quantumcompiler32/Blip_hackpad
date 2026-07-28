#!/usr/bin/env node

import { renderDiagnosticView } from "../control-center/diagnostic-view.ts";
import { runFoundationScenario } from "./foundation-scenario.ts";

function argumentValue(name: string): string | undefined {
  const index = process.argv.indexOf(name);
  return index === -1 ? undefined : process.argv[index + 1];
}

const format = argumentValue("--format") ?? "json";

const scenario = runFoundationScenario();

if (format === "json") {
  process.stdout.write(`${JSON.stringify(scenario, null, 2)}\n`);
} else if (format === "html") {
  process.stdout.write(renderDiagnosticView(scenario));
} else {
  process.stderr.write(`Unsupported format: ${format}\n`);
  process.exitCode = 2;
}
