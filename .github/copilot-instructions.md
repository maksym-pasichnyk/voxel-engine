# Shared Agent Rules

These rules apply to all agents in this workspace.

## Unified Handoff Contract

Only hand off work when the current agent has reached the limit of its responsibility or when another agent clearly owns the next step.

Before any handoff:
- finish the current agent's minimum required output
- make assumptions explicit
- make risks explicit
- make unresolved questions explicit
- state why the next agent is needed
- state what the next agent should focus on
- avoid handing off vague or partially framed work

Every handoff must include this exact structure:

Handoff target
- <name of the next agent>

Why handoff is needed
- <why this work no longer belongs to the current agent>

Completed context
- <what has already been established, decided, implemented, or verified>

Required context for next agent
- <facts, constraints, boundaries, dependencies, and relevant assumptions the next agent must preserve>

Open questions
- <unknowns that still need resolution>

Priority focus for next agent
- <the highest-value next step the receiving agent should take>

Risk focus for next agent
- <the main risks the receiving agent must evaluate or protect against>

Expected output from next agent
- <what concrete output the receiving agent should produce>

## Cross-Agent Rules
- Do not hand off work as a substitute for thinking. First complete the current agent's minimum required output, then hand off only the remaining domain-owned work.
- If the task is still ambiguous, hand off to `research` before handing off to implementation, review, or validation agents.
- Preserve the receiving agent's ownership boundaries. Do not prescribe implementation details to another agent unless those details are required as constraints.
- Keep handoff context technical, concise, and evidence-based.
