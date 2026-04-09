# Diagrams README

This folder contains architecture diagrams for the POWTEX Geant4 simulation code.

## Included diagrams

1. simulation_data_flow.md
- Purpose: User-friendly overview of simulation control flow and data flow.
- Focus: How main setup, detector construction, primary generation, messenger command routing, and analysis outputs connect.
- Primary-generation note: Includes both command-line input-file setup and macro command routing via messenger.

2. class_uml.md
- Purpose: UML-style class relationship diagram.
- Focus: Inheritance from Geant4 base classes and key ownership/usage relationships in project classes.
- Primary-generation note: Includes PrimaryGeneratorMessenger and input-file lock/preference behavior in class members and relations.

3. output_analysis_pipeline.md
- Purpose: End-to-end output artifact and analysis pipeline map.
- Focus: Generated simulation text files, ROOT macros, intermediate ROOT files, and optional CSV export.
- Primary-generation note: Shows runtime configuration path from command-line and macro command into the primary generator.

## Visual conventions

- Orange nodes/classes: Explicitly coded in this repository.
- Blue nodes/classes: Geant4 default framework/package components.
- Yellow nodes/classes: Generated data artifacts (used in the output-analysis diagram).

## Notes and scope

- The diagrams are intentionally high-level and show the core architecture.
- They do not enumerate every Geant4 helper type used in implementation.
- They focus on the detector workflow implemented in this codebase (as of 09.04.2026).

## Maintenance guidelines

- Update diagrams when class names, ownership, or major data products change.
- Update diagrams when primary-input command names, command-line flags, or precedence behavior changes.
- Keep color semantics consistent across all diagram files.
- If a new detector subsystem is added, extend both the flow and UML diagrams.
- Automatic creation and adjustment through, e.g., Copilot.

## Rendering

- Open these Markdown files in VS Code Markdown Preview to render Mermaid diagrams.
- Need a dedicated extension to render Mermaid diagrams, e.g., Markdown Preview Mermaid Support.
