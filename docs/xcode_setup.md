# Xcode Setup for CMake Projects

## Generating an Xcode Project from CMake

Instead of generating Makefiles, tell CMake to generate an Xcode project:

```bash
cmake -G Xcode -B build
```

This creates a `.xcodeproj` inside `build/`. Open it:

```bash
open build/realtime-detection-pipeline.xcodeproj
```

## Compiling

In Xcode, select your target from the **scheme dropdown** at the top (e.g., your app target, `tests`, or `ALL_BUILD`). Then:

- **Cmd+B** — Build
- The output appears in the bottom panel (Issue Navigator shows errors)

## Running

- **Cmd+R** — Build and Run
- Make sure the right target is selected in the scheme dropdown (e.g., your app, not a library target)
- Output appears in Xcode's console at the bottom

## Debugging

- **Click the line number gutter** to set a breakpoint (blue arrow appears)
- **Cmd+R** — runs and stops at breakpoints
- When stopped, you can:
  - **Hover over variables** to see their values
  - **Use the Variables View** (bottom left) to inspect all local variables
  - **Step Over** (F6) — execute current line, move to next
  - **Step Into** (F7) — enter a function call
  - **Step Out** (F8) — finish current function, return to caller
  - **Continue** (Cmd+Y) — resume until next breakpoint
- The **Debug Console** at the bottom lets you type LLDB commands for advanced inspection

## Running Tests

Select the `tests` target in the scheme dropdown, then **Cmd+U** runs all tests. Xcode integrates with gtest — test results appear in the Test Navigator (Cmd+6).

## Workflow

```bash
# One-time setup for each project:
cmake -G Xcode -B build
open build/realtime-detection-pipeline.xcodeproj

# After changing CMakeLists.txt, re-run:
cmake -G Xcode -B build
# Xcode picks up changes automatically
```

You can still use the command line (`cmake --build build`) if you prefer — both work on the same build directory.
