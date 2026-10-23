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

On Windows with MSYS2, use `mingw32-make` in place of `make`.

## Project layout

| Path | Module (SAD 3.4) | Owner |
|---|---|---|
| `src/main.cpp` | Application entry point and integration | Person 1 |
| `src/console_ui.*` | Console UI: welcome screen, name input, progress | Person 1 |
| `src/session.*` | Session Manager: attempt state, reset, replay | Person 1 |
| `src/question_bank.*` | Question Bank: loading and validation | Person 2 |
| `src/quiz_engine.*` | Quiz Controller and answer input | Person 3 |
| `src/scoring.*` | Scoring Engine and results | Person 4 |
| `tests/` | Unit tests, one `test_<module>.cpp` per module | All |

The Makefile compiles every `src/*.cpp` automatically, so new modules need no build changes. Each `tests/test_*.cpp` is built as its own test program with its own `main`.

## Contributing

1. Branch from `main` as `feature/<name>`.
2. Keep commits small and focused on your own tasks.
3. Open a pull request into `main`; GitHub Actions builds and tests it.
4. Merge after a teammate reviews and approves.

Code must compile cleanly with `-Wall -Wextra -Wpedantic -Werror`. Read input with `std::getline`; `gets`, `scanf("%s")` and `strcpy` are rejected by CI (OQS-SR-001).

## Sprint demos

- [Sprint 1 demo](demo/sprint1-demo.mp4): start the quiz, enter a name, answer every question with feedback and progress, and see the final score.
