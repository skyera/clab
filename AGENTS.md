# Repository guidance

## Project

This repository contains a C99 contact-directory demo using uthash. Keep the
application and build workflow compatible with Windows (MSVC and MinGW GCC)
and Linux (GCC). Image rendering and TUI features have been discussed but are
not implemented.

## Responding to questions and requests

For each question, feature idea, or implementation request, assess the goal
and relevant project context before proposing a solution. Present the design
or plan before substantial implementation, with detail proportional to the task.

- State your understanding of the desired outcome and any material assumptions.
- Give a concrete recommendation and explain the key reasons and tradeoffs.
  Compare alternatives when they would affect the user's decision.
- Describe the proposed design and implementation steps, including affected
  components, dependencies, Windows/Linux compatibility, and validation.
- For UI or TUI work, show a mockup or wireframe when helpful. Include layout,
  keyboard interactions, resizing behavior, and relevant empty/error states.
  A text mockup is sufficient for terminal interfaces.
- Explain expected performance impact where relevant: CPU, memory, startup
  time, rendering latency, terminal output volume, or scaling with data size.
  Distinguish estimates from measurements and suggest benchmarks when needed.
- Identify material limitations and unresolved decisions. Ask focused questions
  only when the answers would materially change the solution.

Keep simple answers and routine edits brief; omit inapplicable items rather
than filling out a checklist. For questions, provide the analysis and
recommendation without making unrequested changes.

## Review, approval, and delivery

For feature and change requests, use this workflow:

1. Inspect the relevant code and prepare a concrete, reviewable proposal with
   the design, implementation plan, recommendation, applicable UI mockup,
   performance impact, and validation approach.
2. Present the proposal and wait for the user's approval before implementing.
   Read-only investigation may continue while awaiting approval. If the user
   revises the proposal, update it before proceeding.
3. Treat `doit`, `ship`, or an equivalent explicit approval of the proposal as
   authorization to implement the approved scope, verify it, commit it, and push
   it to the current branch's configured remote. No separate commit or push
   confirmation is needed unless the user limits that authorization.
4. Implement the approved changes, run relevant builds and tests, inspect the
   final diff, and fix failures caused by the changes. Report unavailable checks
   accurately; do not claim they passed.
5. Commit only changes within the approved scope, preserving unrelated staged
   and working-tree changes. Push normally without force, and report the commit,
   destination branch, validation results, and any remaining limitations.

Approval applies to the reviewed scope. If implementation reveals a material
change in behavior, dependencies, UI, or performance tradeoffs, present the
revised design and obtain approval for that change. Honor direct instructions
to implement, verify, commit, or push an already specified change without
restarting the review process. Never infer approval from silence or elapsed time.

## Layout

- `src/main.c`: contact hash-table operations and the demo entry point.
- `CMakeLists.txt`: executable target, compiler options, and CTest registration.
- `build.ps1`: Windows Release build and smoke test; defaults to Visual Studio
  2022 x64, with `-MinGW` selecting MinGW Makefiles.
- `build.cmd`: Command Prompt wrapper for `build.ps1`; forwards options and exit codes.
- `build.sh`: Linux Release build and smoke test using Unix Makefiles.
- `.github/workflows/build.yml`: Windows and Linux builds using these scripts.
- `third_party/uthash`: pinned upstream Git submodule for hash-table macros.
- `third_party/Unity`: pinned upstream Git submodule for C unit testing.

## Dependencies

Initialize dependencies with:

```sh
git submodule update --init --recursive
```

Preserve submodules as Git links. Do not copy their sources into application
directories or edit upstream code for ordinary application changes. Update a
submodule revision only when needed for the requested work, and explain the
reason. Unity is present but is not yet connected to CMake or application tests.

## Build and validation

Run the relevant command from the repository root:

| Environment | Build and smoke test | Also run the demo |
| --- | --- | --- |
| Windows CMD, MSVC | `build.cmd` | `build.cmd -Run` |
| Windows CMD, MinGW | `build.cmd -MinGW` | `build.cmd -MinGW -Run` |
| Windows PowerShell, MSVC | `.\build.ps1` | `.\build.ps1 -Run` |
| Windows PowerShell, MinGW | `.\build.ps1 -MinGW` | `.\build.ps1 -MinGW -Run` |
| Linux Bash | `bash build.sh` | `bash build.sh --run` |

The scripts require CMake, CTest, and the selected compiler/build tools. Keep
the separate `build-windows`, `build-mingw`, and `build-linux` directories;
never share a CMake cache across platforms or generators.

The current CTest smoke test checks successful exit and `After cleanup: 0` in
the output. For changes to contact behavior, add meaningful unit tests using
Unity and register them with CTest. Test through the existing build scripts.
Report which platforms were actually checked and any unavailable tooling.
Documentation-only changes do not require rebuilding.

## Implementation conventions

- Follow the existing four-space indentation and C99 style.
- Keep platform-specific code isolated and retain portable application logic.
- Check allocation failures, reject oversized input, and release owned memory.
- Keep uthash keys unchanged while entries are stored; remove entries before
  freeing them and use `HASH_ITER` when deleting during traversal.
- Preserve failure exit codes in scripts and quote paths so spaces work.
- Keep shell scripts in LF and CMD scripts in CRLF, as specified in `.gitattributes`.
- Keep generated binaries and build directories out of commits.
- Update `README.md` when commands, dependencies, or program behavior change.
- Preserve unrelated working-tree and staged changes. Run `git diff --check`
  before finishing code changes; report validation results concisely.
