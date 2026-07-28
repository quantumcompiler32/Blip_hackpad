import type { FoundationScenarioResult } from "../simulator/foundation-scenario.ts";

function escapeHtml(value: string | number): string {
  return String(value)
    .replaceAll("&", "&amp;")
    .replaceAll("<", "&lt;")
    .replaceAll(">", "&gt;")
    .replaceAll('"', "&quot;")
    .replaceAll("'", "&#39;");
}

export function renderDiagnosticView(
  scenario: FoundationScenarioResult,
): string {
  if (!scenario.actionResult || !scenario.auditEntry) {
    throw new Error("The diagnostic tracer requires an action result and audit");
  }

  const { actionResult, auditEntry, device, displays, event } = scenario;

  return `<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Blip Control Center Diagnostic</title>
  <style>
    :root {
      color-scheme: light dark;
      font-family: -apple-system, BlinkMacSystemFont, "SF Pro Text", sans-serif;
      --background: #ece8df;
      --surface: #f8f5ee;
      --graphite: #242523;
      --muted: #666760;
      --line: #c9c4b9;
      --amber: #d88a1d;
      --success: #217a58;
    }
    * { box-sizing: border-box; }
    body {
      margin: 0;
      background: var(--background);
      color: var(--graphite);
    }
    main {
      width: min(920px, calc(100% - 32px));
      margin: 32px auto;
      border: 1px solid var(--line);
      background: var(--surface);
      box-shadow: 0 18px 45px rgb(36 37 35 / 12%);
    }
    header {
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 22px 24px;
      border-bottom: 1px solid var(--line);
    }
    h1, h2, p { margin: 0; }
    h1 { font-size: 21px; letter-spacing: -0.02em; }
    h2 {
      margin-bottom: 12px;
      color: var(--muted);
      font-size: 12px;
      letter-spacing: 0.09em;
      text-transform: uppercase;
    }
    .badge {
      border: 1px solid var(--amber);
      color: #7b4806;
      padding: 5px 9px;
      font-size: 12px;
      font-weight: 650;
    }
    .content {
      display: grid;
      grid-template-columns: 1.2fr 1fr;
    }
    section { padding: 24px; }
    section + section { border-left: 1px solid var(--line); }
    .displays { display: grid; gap: 14px; }
    .display {
      min-height: 112px;
      padding: 16px;
      border: 1px solid #55564f;
      background: #171815;
      color: #f2b245;
      font-family: "SF Mono", ui-monospace, monospace;
    }
    .display.large { color: #c9eee2; }
    .display strong, .display span { display: block; }
    .display span { margin-top: 9px; color: #d8d6cf; font-size: 13px; }
    dl {
      display: grid;
      grid-template-columns: max-content 1fr;
      gap: 10px 14px;
      margin: 0;
      font-size: 13px;
    }
    dt { color: var(--muted); }
    dd { margin: 0; font-family: "SF Mono", ui-monospace, monospace; }
    .result {
      margin-bottom: 22px;
      padding-left: 12px;
      border-left: 3px solid var(--success);
    }
    .result p { margin-top: 5px; color: var(--muted); font-size: 13px; }
    @media (max-width: 680px) {
      .content { grid-template-columns: 1fr; }
      section + section {
        border-top: 1px solid var(--line);
        border-left: 0;
      }
    }
    @media (prefers-color-scheme: dark) {
      :root {
        --background: #191a18;
        --surface: #242522;
        --graphite: #f0ede5;
        --muted: #aaa9a2;
        --line: #44453f;
      }
      .badge { color: #f2b245; }
    }
  </style>
</head>
<body>
  <main>
    <header>
      <h1>${escapeHtml(device.label)}</h1>
      <span class="badge">Simulation</span>
    </header>
    <div class="content">
      <section>
        <h2>Semantic display state</h2>
        <div class="displays">
          <div class="display" aria-label="Small display: Persistent Status">
            <strong>${escapeHtml(displays.small.title)}</strong>
            <span>${escapeHtml(displays.small.status)}</span>
          </div>
          <div class="display large" aria-label="Large display: Contextual Status">
            <strong>${escapeHtml(displays.large.title)}</strong>
            <span>${escapeHtml(displays.large.detail)} · ${escapeHtml(displays.large.message)}</span>
          </div>
        </div>
      </section>
      <section>
        <h2>Latest diagnostic</h2>
        <div class="result">
          <strong>${escapeHtml(actionResult.description)}</strong>
          <p>Safe Dry Run · no host action executed</p>
        </div>
        <dl>
          <dt>Event</dt>
          <dd>${escapeHtml(event.event.kind)}</dd>
          <dt>Sequence</dt>
          <dd>${escapeHtml(event.sequence)}</dd>
          <dt>Origin</dt>
          <dd>${escapeHtml(auditEntry.origin)}</dd>
          <dt>Audit result</dt>
          <dd>${escapeHtml(auditEntry.result)}</dd>
          <dt>Audit ID</dt>
          <dd>${escapeHtml(auditEntry.id)}</dd>
        </dl>
      </section>
    </div>
  </main>
</body>
</html>
`;
}
