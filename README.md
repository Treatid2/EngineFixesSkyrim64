# Engine Fixes VR ObjectLOD and Shadow Map Crash Fix (obsolete)

> [!IMPORTANT]
> **This standalone downstream build is obsolete.** Its fixes are incorporated
> into [Engine Fixes VR 7.4.9](https://www.nexusmods.com/skyrimspecialedition/mods/62089)
> and later through
> [upstream PR #38](https://github.com/alandtse/EngineFixesSkyrim64/pull/38).

## Migration

1. Install or update to the official Engine Fixes VR 7.4.9 or newer, including
   its required Part 2 component.
2. Uninstall the standalone ObjectLOD/shadow-map patch.
3. Ensure the official `SKSE/Plugins/EngineFixes.dll` is no longer being
   overwritten by the archived downstream package.

Do not run the old standalone DLL over Engine Fixes VR 7.4.9 or later. The
official release already contains the ObjectLOD, streamed scene-object,
shadow-map, render-pass, and related VR reader safeguards from this branch.

This distribution branch and its source are retained for provenance, review,
and technical reference. No source code has been removed.

- Original downstream build: `e62332fd11c7b827497fd99b1be9769c9be520c1`
- Integrated upstream merge: `1c94ef4f94d30986d1f57a0aa1097ad42fa2964d`
- Licence: GPL-3.0-or-later
