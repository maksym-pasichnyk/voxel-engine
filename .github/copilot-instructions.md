# Shared Agent Rules

These rules apply to all agents in this workspace.

## Unified Transition Contract

Only transition work when the current agent has reached the limit of its responsibility or when another role clearly owns the next step.

There are two valid transition modes in this workspace:
- Direct handoff: allowed only for implementation-oriented agents that still have explicit handoff capability.
- Caller-side escalation: required for internal-only agents that do not have direct delegation capability.

Before any transition:
- finish the current agent's minimum required output
- make assumptions explicit
- make risks explicit
- make unresolved questions explicit
- state why the next role is needed
- state what the next role should focus on
- avoid vague or partially framed transitions

Every transition must include this exact structure:

Transition target
- <name of the next agent or role>

Transition mode
- <direct handoff | caller-side escalation>

Why transition is needed
- <why this work no longer belongs to the current agent>

Completed context
- <what has already been established, decided, implemented, or verified>

Required context for next role
- <facts, constraints, boundaries, dependencies, and relevant assumptions the next role must preserve>

Open questions
- <unknowns that still need resolution>

Priority focus for next role
- <the highest-value next step the receiving role should take>

Risk focus for next role
- <the main risks the receiving role must evaluate or protect against>

Expected output from next role
- <what concrete output the next role should produce>

## Cross-Agent Rules
- Do not transition work as a substitute for thinking. First complete the current agent's minimum required output, then transition only the remaining domain-owned work.
- If the task is still ambiguous, route to `research` before moving to implementation, review, or validation roles.
- Preserve the receiving role's ownership boundaries. Do not prescribe implementation details to another agent unless those details are required as constraints.
- Keep transition context technical, concise, and evidence-based.
- Internal-only agents must use caller-side escalation semantics.
- Direct handoff semantics are reserved for implementation-oriented agents and approved experimental flows.

## Ownership Matrix
- `research` owns fact-finding, dependency analysis, constraints, and uncertainty reduction. It does not own boundary decisions or production implementation.
- `architect` owns boundaries, invariants, authority rules, lifecycle rules, and design-level tradeoffs. It does not own production implementation.
- `core` owns world-state source of truth: chunk data, block storage, coordinate logic, chunk lifecycle, simulation boundaries, and serialization-facing mechanics.
- `rendering` owns render-side representation: meshing, invalidation, visibility-related logic, rebuild scheduling, and simulation-to-render data flow.
- `gameplay` owns player-visible rules: interactions, tools, item behavior, failure states, and feature policy built on world state.
- `proof-of-concept` owns feasibility experiments and prototype evidence. It does not own silent promotion of experimental code into production-ready implementation.
- `performance-review` owns runtime-cost, scaling, allocation, and measurement analysis. It does not own correctness review or validation design.
- `code-review` owns implementation correctness, maintainability, boundary preservation, and hidden coupling review. It does not own test-plan authorship or requirements traceability.
- `qa` owns validation strategy, reproducibility, scenario coverage, and evidence planning. It does not own primary correctness review or requirements/design alignment decisions.
- `conformance-review` owns traceability across task, design, code, tests, and documentation. It does not own primary validation design, code-quality review, or performance analysis.
- `delivery` owns branch, commit, PR, and readiness packaging. It does not own implementation, review, QA, or technical design.

## Shared Artifact Contract
All agent outputs should include these fields whenever they are relevant and available:
- Assumptions
- Risks
- Open questions
- Evidence status
- Recommended next role

When an agent transitions work, these fields should be preserved and updated rather than restated from scratch.

## Agent Invocation Policy
- The following agents are internal-only and must not be directly user-invocable: `research`, `architect`, `orchestrator`, `code-review`, `qa`, `conformance-review`, `performance-review`, `delivery`.
- User-invocable agents are limited to implementation-oriented workflows: `core`, `rendering`, `gameplay`, and `proof-of-concept`.
- If a user asks for an internal-only agent explicitly, route through the active assistant flow and apply that agent's rules without exposing direct invocation.
