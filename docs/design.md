# Competitive Programming Reasoning Canvas

Design notes from the initial discussion. This document separates agreed direction from proposals and unresolved choices; it does not imply that features are implemented.

## Motivation

Competitive programming is primarily a source of enjoyment through thinking and solving puzzles. The user has over two years of experience, has solved more than 2,000 problems across six platforms, and is familiar with basic algorithms.

A previous spaced-repetition system helped retain fundamentals, but the current focus is developing reasoning beyond familiar patterns. Contest results, ratings, solved counts, and algorithm tags are not the main measures of interest.

The tool should help examine how reasoning develops on an individual problem, particularly when observations accumulate without producing new insight. Explanations should make causal and logical connections explicit instead of hiding them behind phrases such as “it is obvious.”

Building the tool is also a personal learning project. The user wants to enjoy implementing it, with assistance for design discussions, explanations, and code review rather than having the entire project built for them by default.

## Agreed product direction

- A local browser-based application, with a possible Tauri desktop shell.
- One visual reasoning canvas per problem.
- Direct editing on the canvas rather than a separate document editor with an automatically generated graph.
- Typed content and mathematical notation; no handwriting requirement.
- Typst is the preferred direction for mathematical authoring.
- A node displays source while being edited and rendered content afterward, in the same place. A simultaneous live preview is not required.
- The map is built during solving and refined after the problem is solved.
- The map represents the actual investigation, including branches, failed conjectures, dead ends, unresolved questions, and hints. It is not merely a polished linear proof.
- The user writes detailed solutions in their own words.
- Initial scope is individual problems. Cross-problem review will be requested from the assistant when wanted, rather than implemented as a dashboard now.
- Problems carry searchable metadata for required techniques, platform, and platform-specific clist difficulty ratings.

## Problem metadata and search

Tags and metadata help find previous problems and their reasoning maps. They remain secondary to the reasoning itself.

- **Required techniques:** technique tags describing the algorithms or methods needed for a problem.
- **Platform:** the platform the problem belongs to.
- **clist difficulty rating:** the problem's rating associated with that platform, rather than a universal difficulty score.

Search and filtering should support these fields. Ratings must retain their platform context when displayed or used for filtering and sorting. A numeric rating on one platform is not treated as equivalent to the same number on another; there is no combined cross-platform difficulty scale.

Whether metadata is entered manually or imported, and how unavailable clist ratings are handled, remain open implementation decisions.

## Reasoning model

Observations, conjectures, and deductions form nodes. Connections express how thoughts relate and how one step led to another. The reasoning map must accommodate branching rather than enforce a fixed sequence.

Two relationships deserve distinct treatment:

- **Motivation:** what prompted a conjecture or made a direction worth exploring.
- **Justification:** what establishes a claim as true.

An example can motivate a conjecture without proving it. Similarly, a correct proof can leave the motivation for discovering it unexplained. The representation should not confuse these relationships or require every exploratory choice to follow deductively.

Stuck points are valuable records. Later reflection might reveal an unsupported assumption, an observation that did not address the missing claim, or a useful question that was never asked. It is also acceptable for the reason for getting stuck to remain unknown.

## Working workflow

### During an attempt

Capture thoughts directly on the canvas, connect them to earlier thoughts, and explore multiple branches. Capture should be lightweight enough to support thinking without requiring detailed classification of every entry.

Retain unsuccessful approaches instead of deleting them merely because they did not lead to the solution. Record outside hints so that later review can distinguish independent progress from assisted progress.

### When stuck

The assistant can guide the user from their existing reasoning through targeted questions, revealing examples, or small hints. Guidance should help the user continue their own investigation rather than force reconstruction of an editorial's unexplained jumps.

Assistant claims and proofs still require scrutiny; solving every problem correctly is not guaranteed. An integrated assistant interface has not been requested or selected.

### After solving

Refine the map, clarify arguments, resolve conjectures where possible, and write the detailed solution. Preserve the exploration that preceded the solution, including where hints helped.

Later explanations should be distinguishable from what was known during the attempt. How to preserve that distinction—annotations, history, snapshots, or another mechanism—is still open.

### Later review

Review individual maps with the assistant when desired. A readable export or another way to share the underlying map would support this; its format and implementation are undecided.

## Proposed interactions

These are starting proposals, not finalized interaction specifications:

- Double-click an empty canvas area to create a node.
- Edit node content in place, then render it when editing ends.
- Move nodes freely on the canvas.
- Drag from an existing node to create a connected thought.
- Label connections with the reasoning behind a transition.
- Optionally mark thoughts as stuck, disproved, or resolved.
- Rearrange and annotate the graph after solving without losing the original exploration.

Node classification should be optional during an attempt. A deduction may require several premises; how the UI represents their joint support remains to be designed.

## Technical direction

The user has some React experience and also knows Rust. React with TypeScript for the interface and Tauri for a desktop shell has been proposed, but the stack has not been finalized.

Typst authoring is preferred, but the rendering approach and supported scope of Typst syntax still need investigation. Local persistence is needed for a useful personal tool; storage format and save behavior have not yet been chosen.

No graph library or custom canvas implementation has been selected.

## Proposed first milestone

Create a small working canvas where the user can create a text node, edit it, and drag it around. This provides a concrete foundation before adding Typst rendering, connections, and persistence.

This milestone is a proposal for incremental learning, not authorization to implement the whole application immediately.

## Deferred scope

- Contest participation and personal rating tracking, including virtual-contest tracking. Problem difficulty ratings are included as search metadata.
- Algorithm-tag analytics and spaced-repetition scheduling.
- Cross-problem dashboards, automated skill scores, or recurring-bottleneck classification.
- Hosted deployment and cross-device synchronization.
- Handwriting input.

## Open design decisions

1. Use a graph library for canvas interactions, or build those interactions to learn the underlying mechanics?
2. Begin with a browser app, or introduce Tauri from the start?
3. How should multiple premises, connection labels, and optional node states be represented?
4. How should original exploration and later refinement remain distinguishable?
5. Where does the detailed final solution live in relation to the map?
6. How should Typst rendering, local saving, and sharing maps for review work?
