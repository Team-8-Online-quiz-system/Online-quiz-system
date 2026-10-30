# Online Quiz System

A console-based multiple-choice quiz game written in C++17 (Team 8, Software Engineering mini-project).

## Build and run

Requires `g++` (C++17) and `make`.

```sh
make          # builds ./quiz
./quiz
make test     # builds and runs every tests/test_*.cpp
make clean
```

On Windows with MSYS2, use `mingw32-make` in place of `make`, and build with the **UCRT64** toolchain (put `C:\msys64\ucrt64\bin` first on `PATH`); the MinGW64 g++ on some machines exits without output.

After the last question the result screen shows the final score and a summary (player, score, total, percentage). The player can then play again with a fresh attempt or exit.

In an interactive terminal the quiz runs as a styled TUI: a boxed title screen, a fresh screen per question with a progress bar and live score, coloured option labels, green/red feedback and a result panel. It uses only ANSI escape codes from the standard library, with no third-party packages. Output is plain text automatically when it is piped (tests, CI) or when `NO_COLOR` is set; `./quiz --plain` forces plain mode.

## Project layout

| Path | Module (SAD 3.4) | Owner |
|---|---|---|
| `src/main.cpp` | Entry point: picks plain or styled output | Person 1 |
| `src/app.*` | Application loop: welcome, quiz, result, replay and exit | Person 1 |
| `src/quiz_runner.*` | Quiz Controller: asks every question once and scores it | Person 1 |
| `src/console_ui.*` | Console UI and TUI: screens, menus, progress, result panel | Person 1 |
| `src/terminal.*` | Terminal detection and Windows ANSI/UTF-8 setup | Person 1 |
| `src/session.*` | Session Manager: attempt state, reset, replay | Person 1 |
| `src/question_bank.*` | Question Bank: loading, validation, bounds checking | Person 2 |
| `src/quiz_engine.*` | Question display, answer input and input hardening | Person 3 |
| `src/scoring.*` | Scoring Engine and Result Formatter: score, percentage, summary | Person 4 |
| `scripts/check_unsafe_calls.sh` | Source scan for command execution and unsafe formats (run in CI) | Person 3 |
| `tests/` | One test program per `tests/test_*.cpp` | All |
| `.github/workflows/` | CI: build and test; g++/clang++ portability | Persons 1, 4 |

The Makefile compiles every `src/*.cpp` automatically, so new modules need no build changes. Each `tests/test_*.cpp` is built as its own test program with its own `main`.

## Contributing

1. Branch from `main` as `feature/<name>`.
2. Keep commits small and focused on your own tasks.
3. Open a pull request into `main`; GitHub Actions builds and tests it.
4. Merge after a teammate reviews and approves.

Code must compile cleanly with `-Wall -Wextra -Wpedantic -Werror`. Read input with `std::getline`; `gets`, `scanf("%s")` and `strcpy` are rejected by CI (OQS-SR-001).

## Console conventions (OQS-NF-004)

All modules use the same wording so every screen reads the same way:

| Kind | Format | Example |
|---|---|---|
| Menu | Numbered options, then a prompt | `1. Start Quiz` / `2. Exit` / `Choose an option (1-2): ` |
| Prompt | `<Action> (<accepted input>): ` | `Your answer (A-D or 1-4): ` |
| Error | `Invalid <thing>. Please enter <accepted input>.` and the same prompt again | `Invalid answer. Please enter A-D or 1-4.` |
| Feedback | One short sentence | `Correct!` / `Incorrect. The correct answer was B.` |

Invalid input never advances the quiz or changes the score.

## Documentation

| Document | Location |
|---|---|
| Software Requirements Specification | [`8_SRS_.pdf`](8_SRS_.pdf) |
| Software Architecture and Design | [`8_SAD.pdf`](8_SAD.pdf) |
| Software Test Plan | [`8_TestPlan.pdf`](8_TestPlan.pdf) |
| Requirements Traceability Matrix | [`docs/RTM.md`](docs/RTM.md) |
| Test Summary Report | [`docs/TEST_SUMMARY.md`](docs/TEST_SUMMARY.md) |
| Test results per module | [`docs/test-results/`](docs/test-results/) |

## Sprint demos

- [Sprint 1 demo](demo/sprint1-demo.mp4): start the quiz, enter a name, answer every question with feedback and progress, and see the final score.
