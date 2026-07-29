# Blip Foundation bounded implementation runbook

## Batch bounds

- Run exactly one bounded batch per orchestration task, then stop.
- Implement no more than two GitHub issues with no more than two implementation agents.
- Never start the next batch automatically.
- Use actual GitHub issue numbers for branches, worktrees, comments, and reports.
- Treat a valid user-owned issue as in progress; never edit or delegate it. Independently preserve any user work found in the primary checkout.

## Orientation and scheduling

- Read `AGENTS.md`, `CONTEXT.md`, `CONTEXT-MAP.md` when present, relevant ADRs, the Foundation specification, issue-tracker rules, all complete `ready-for-agent` issue records, and the implement/TDD/code-review skills.
- Confirm local tickets are published as individual GitHub issues and maintain the local-to-GitHub mapping.
- Build the complete dependency graph. Select only open, ready, unowned tickets whose blockers are completed and integrated, whose work is not waiting on human hardware validation, and whose files can be safely isolated.
- Prefer Foundation critical-path tickets that unlock downstream work, have automatic verification, have little overlap, and fit one fresh agent context.
- Do not replace a blocked ticket with one of its dependents. A freed slot may be used only for another already-unblocked ticket without exceeding the two-ticket batch limit.

## Batch preflight and approval gate

Every future batch begins with a mandatory read-only preflight. Before creating worktrees, spawning sub-agents, editing files, changing branches, or modifying GitHub:

1. Refresh the repository state, orchestrator status, GitHub issues, comments, labels, and blocking relationships.
2. Select the proposed batch, but do not execute it.
3. Present a plain-language batch proposal containing:
   - The selected local ticket numbers and actual GitHub issue numbers.
   - The purpose of each ticket.
   - Exactly what behavior or capability will be created.
   - How each ticket moves the Foundation Release forward.
   - Why each ticket is eligible now.
   - Proof that its blockers are completed and integrated.
   - Which downstream tickets it will unlock.
   - The expected implementation areas and likely file overlap.
   - The acceptance criteria that will be satisfied.
   - The tests and verification expected.
   - Risks, uncertainty, and required human actions.
   - Estimated complexity: small, medium, or large.
4. Recommend the primary orchestrator model for the execution task:
   - `gpt-5.6-terra` with high reasoning for routine, isolated, well-specified work.
   - `gpt-5.6-sol` with high reasoning for architecture, hardware/firmware, protocol, security-sensitive behavior, difficult integration, hard debugging, or release-gate work.
5. Recommend the model and reasoning level for each ticket sub-agent.
6. Explain why the recommended models are sufficient and what using Sol instead of Terra would materially improve.
7. State whether the tickets should run concurrently or sequentially and explain why.
8. End by asking: “Do you approve this batch and model plan?”

Do not execute the batch until the user explicitly approves it.

## Repository safety

- Inspect status, branches, and worktrees before creating branches.
- Preserve unrelated and uncommitted user changes. Never stash, discard, reset, overwrite, or include them.
- Give every ticket its own worktree and branch named `codex/issue-<github-number>-<short-slug>`.
- Base each worktree on the latest integrated prerequisite commit. Never share a checkout, worktree, or branch between agents.
- Stop if safe isolation cannot be established.

## Agent routing and execution

- Use `gpt-5.6-terra` with high reasoning for routine, well-specified tickets.
- Use `gpt-5.6-sol` with high reasoning only for difficult architecture, cross-system integration, hardware/firmware reasoning, or hard debugging.
- Assign exactly one GitHub issue and one isolated worktree to each agent.
- Agents must verify blockers, follow the implementation skill, use one red-green TDD slice at a time at agreed public seams, satisfy every acceptance criterion with evidence, run focused and complete relevant checks, perform separate Standards and Spec review axes, commit ticket-scoped work, and report evidence and risks.
- Agents must not merge, push, close issues, change dependencies, refactor speculatively, or edit another worktree.

## Review and integration

- Inspect every agent diff and commit against every acceptance criterion and reject unrelated changes.
- Review code quality and specification compliance separately and independently rerun relevant checks.
- Check logical and textual overlap between batch tickets. Require material findings to be corrected before integration.
- Integrate in dependency order. Use the merge-conflict skill for simple conflicts and stop for product or architecture decisions.
- Run the combined relevant suite after integration.
- Push only when authentication and branch policy permit.
- Update and close a GitHub issue only after integration and verified acceptance. Never claim human-only criteria passed without human evidence.

## Human-only stops

Stop and ask one precise question for physical hardware validation, PCB manufacturing approval, visual or tactile hardware inspection, credentials or external authorization, interactive macOS permission dialogs, destructive or externally visible actions, missing product decisions, or criteria that cannot be honestly automated.

## Checkpoint

After the batch, update `orchestrator-status.md` with the date and batch, issue mapping, integrated and in-progress tickets, blockers, branches and SHAs, tests, human actions, newly unblocked frontier, and next-batch recommendations. Then produce the read-only preflight proposal for the next batch, but do not launch it. Store no credentials or secrets.
