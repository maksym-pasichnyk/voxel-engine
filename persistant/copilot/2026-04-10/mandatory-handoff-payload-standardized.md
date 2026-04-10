# Title
Mandatory handoff payload standardized across all agents

## Source prompt summary
User requested adding a unified Mandatory Handoff Payload template to all agents so transitions between roles are fully standardized.

## Output body
Added the same `## Mandatory Handoff Payload` section to all 13 agent definitions in `.github/agents/`.
The section includes one exact transition template with the same required fields in each file:
- Transition target
- Transition mode
- Why transition is needed
- Completed context
- Required context for next role
- Open questions
- Priority focus for next role
- Risk focus for next role
- Expected output from next role

Verified coverage:
- Agent files found: 13
- `## Mandatory Handoff Payload` matches: 13

## Assumptions
- The single template text should be identical across all agent files.
- The transition structure from shared rules is the source of truth.

## Risks
- Future manual edits may drift individual agent templates unless periodically checked.

## Open questions
- Should this be enforced with a CI validation check to prevent drift?

## Evidence status
implemented and verified

## Recommended next role
none

## Timestamp (UTC)
2026-04-10T00:00:00Z
