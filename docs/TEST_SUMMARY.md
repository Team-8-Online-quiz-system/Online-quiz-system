# Test Summary Report — Online Quiz System v1.0

| Item | Value |
|---|---|
| Project | Online Quiz System (Team 8) |
| Version | 1.0 (final development freeze) |
| Test period | 26–30 October 2026 (Sprint 2) |
| Prepared by | Sarah (Person 1, integration) |
| Inputs | Software Test Plan v1.0, [RTM](RTM.md), module test records in [`docs/test-results/`](test-results/) |

## 1. Scope

All functional (OQS-F-001 to 020), non-functional (OQS-NF-001 to 005) and security (OQS-SR-001 to 005) requirements of the console application, tested at unit, integration, system and acceptance level as planned in the Software Test Plan. Out of scope, as planned: GUI/web, networking, databases and multi-user operation.

## 2. Environments

| Environment | Compiler | Result |
|---|---|---|
| GitHub Actions, Ubuntu (Build and Test) | g++ | Unsafe-call scans, `-Werror` build, all test programs, full-quiz smoke test with replay and exit: pass |
| GitHub Actions, Ubuntu (C++ Portability) | g++ and clang++ | Build and all test programs under both compilers: pass |
| Windows 11, MSYS2 UCRT64 | g++ 15.1 | Build and all test programs: pass |
| Arch Linux | g++ 16.2 | Question Bank tests: pass |

## 3. Results by test program

| Test program | Area | Checks | Passed | Failed |
|---|---|---|---|---|
| `test_session` | Session state, name input, menus, progress | 22 | 22 | 0 |
| `test_question_bank` | Question bank loading and validation | 17 | 17 | 0 |
| `test_question_bank_validation` | Bounds checking, malformed records, startup time | 38 | 38 | 0 |
| `test_quiz_engine` | Question display, answer parsing, re-prompting | 35 | 35 | 0 |
| `test_input_hardening` | Input length, control characters, command-like input | 23 | 23 | 0 |
| `test_robustness` | Mixed invalid input through the full quiz | 4 | 4 | 0 |
| `test_reliability_performance` | 100-input reliability, response time | 6 | 6 | 0 |
| `test_scoring` | Evaluation, score updates, feedback, final score | 15 | 15 | 0 |
| `test_results` | Percentage, summary, score bounds, replay reset | 34 | 34 | 0 |
| `test_integration` | Full quiz flow across all modules | 13 | 13 | 0 |
| `test_app` | Application loop: replay, exit, input ending, summaries | 15 | 15 | 0 |
| `test_tui` | Plain and styled terminal output | 20 | 20 | 0 |
| **Total** | | **242** | **242** | **0** |

Plus the CI smoke test (a full quiz scored 5/5 = 100.0%, a replay scored 1/5 = 20.0%, clean exit, no colour codes in piped output) and `scripts/check_unsafe_calls.sh`, which finds no command-execution calls or non-literal format strings.

## 4. Results by test case

| Test case | Requirement(s) | Status |
|---|---|---|
| TC-INIT-01, TC-INIT-02, TC-INIT-03 | OQS-F-001 to 003 | Pass |
| TC-QB-01, TC-QB-02 | OQS-F-004, 005 | Pass |
| TC-QUIZ-01 to TC-QUIZ-04 | OQS-F-006, 007, 014, 015 | Pass |
| TC-ANS-01 to TC-ANS-03 | OQS-F-008 to 010 | Pass |
| TC-SCORE-01 to TC-SCORE-03 | OQS-F-011 to 013 | Pass |
| TC-RES-01 to TC-RES-03 | OQS-F-016 to 018 | Pass |
| TC-CTRL-01, TC-CTRL-02 | OQS-F-019, 020 | Pass |
| TC-NF-PERF-01, TC-NF-PERF-02 | OQS-NF-001, 002 | Pass (1,000 records validated in ≤ 2 ms; ≈ 1 µs per answer) |
| TC-NF-REL-01 | OQS-NF-003 | Pass (100 mixed inputs, no abnormal termination) |
| TC-NF-UX-01 | OQS-NF-004 | Pass |
| TC-NF-PORT-01 | OQS-NF-005 | Pass (g++ and clang++) |
| TC-SEC-01 to TC-SEC-05 | OQS-SR-001 to 005 | Pass |

**Coverage:** 30 of 30 requirements executed and passed (see the [RTM](RTM.md)).

## 5. Defects

| ID | Description | Severity | Resolution | Retest |
|---|---|---|---|---|
| D-01 | A Windows Application Control policy blocked one locally built test executable on a tester's machine. Environment issue, not a code defect. | Low | Verified the same tests in CI under g++ and clang++. | Pass |
| D-02 | The MSYS2 MinGW64 g++ toolchain on one development machine exits without output when compiling. Environment issue. | Low | Build with the MSYS2 UCRT64 g++; documented in the README. | Pass |

No code defects were found in Sprint 2 integration. No open defects remain.

## 6. Exit criteria (Software Test Plan section 5)

| Criterion | Met |
|---|---|
| All planned test cases executed | Yes |
| All core / high-priority quiz functions pass | Yes |
| No critical defect open | Yes (0 open) |
| Failed cases retested after fixes | Yes (D-01, D-02 retested) |
| Final summary shows the system is ready for evaluation | Yes |

## 7. Conclusion

The integrated Online Quiz System v1.0 meets every requirement in the RTM, passes all 242 automated checks and the CI smoke test on Linux and Windows under two compilers, and has no open defects. It is ready for evaluation and the development version is frozen.
