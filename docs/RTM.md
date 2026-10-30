# Requirements Traceability Matrix — Online Quiz System v1.0

Every requirement in the SRS (functional OQS-F, non-functional OQS-NF) and the SAD's security requirements (OQS-SR) is mapped to the module that implements it, the pull request that delivered it, the test case(s) from the Software Test Plan, and the automated test program that executes it.

**Status legend:** N = Not Run, P = Pass, A = Accepted / Approved.
**Build verified:** final integrated `main`, CI "Build and Test" and "C++ Portability" (g++ and clang++) green.

## Functional requirements

| Req ID | Requirement | Module | Delivered in | Test case(s) | Automated test(s) | Status |
|---|---|---|---|---|---|---|
| OQS-F-001 | Welcome screen and Start Quiz option | Console UI | #1, #71 | TC-INIT-01 | `test_session`, `test_app`, CI smoke test | P |
| OQS-F-002 | Capture and validate player name | Console UI | #1 | TC-INIT-02 | `test_session` | P |
| OQS-F-003 | Initialize attempt (score 0, index 0) | Session Manager | #1 | TC-INIT-03 | `test_session` | P |
| OQS-F-004 | Load question bank | Question Bank | #30, #74 | TC-QB-01 | `test_question_bank`, `test_question_bank_validation` | P |
| OQS-F-005 | Validate question records | Question Bank | #30, #74 | TC-QB-02 | `test_question_bank`, `test_question_bank_validation` | P |
| OQS-F-006 | Present one question at a time | Quiz Engine, Quiz Runner | #31, #33 | TC-QUIZ-01 | `test_quiz_engine`, `test_integration` | P |
| OQS-F-007 | Display all options with consistent labels | Quiz Engine, Console UI | #31, #71 | TC-QUIZ-02 | `test_quiz_engine`, `test_tui` | P |
| OQS-F-008 | Accept a valid answer (A-D / 1-4) | Quiz Engine | #31, #72 | TC-ANS-01 | `test_quiz_engine`, `test_input_hardening` | P |
| OQS-F-009 | Reject invalid answer without advancing | Quiz Engine | #31, #72 | TC-ANS-02 | `test_quiz_engine`, `test_input_hardening`, `test_integration` | P |
| OQS-F-010 | Record exactly one answer per question | Quiz Engine, Session Manager | #31 | TC-ANS-03 | `test_quiz_engine`, `test_integration` | P |
| OQS-F-011 | Compare answer with the answer key | Scoring Engine | #2 | TC-SCORE-01 | `test_scoring` | P |
| OQS-F-012 | +1 for correct, +0 for incorrect | Scoring Engine | #2, #73 | TC-SCORE-02 | `test_scoring`, `test_results` | P |
| OQS-F-013 | Correct / Incorrect feedback | Scoring Engine, Console UI | #2, #71 | TC-SCORE-03 | `test_scoring`, `test_tui` | P |
| OQS-F-014 | Show progress (question N of M) | Console UI | #1, #71 | TC-QUIZ-03 | `test_session`, `test_integration`, `test_tui` | P |
| OQS-F-015 | Advance until every question is asked once | Quiz Runner, Scoring Engine | #2, #33 | TC-QUIZ-04 | `test_scoring`, `test_integration` | P |
| OQS-F-016 | Final score after the last question | Scoring Engine | #2, #33 | TC-RES-01 | `test_scoring`, `test_results`, `test_integration` | P |
| OQS-F-017 | Percentage, no division by zero | Scoring Engine | #73, final integration | TC-RES-02 | `test_results`, `test_app` | P |
| OQS-F-018 | Result summary: name, score, total, percentage | Scoring Engine, App | #73, final integration | TC-RES-03 | `test_results`, `test_app`, CI smoke test | P |
| OQS-F-019 | Replay with a fresh attempt | App, Session Manager | #71 | TC-CTRL-01 | `test_app`, `test_results`, CI smoke test | P |
| OQS-F-020 | Clean exit | App | #71 | TC-CTRL-02 | `test_app`, CI smoke test | P |

## Non-functional requirements

| Req ID | Requirement | Delivered in | Test case | Evidence | Status |
|---|---|---|---|---|---|
| OQS-NF-001 | Start and show the first question within 2 s (1,000 questions) | #74 | TC-NF-PERF-01 | `test_question_bank_validation`: 1,000 records validated in ≤ 2 ms; `loadQuestions` ×10, each < 1 ms | P |
| OQS-NF-002 | Feedback and next prompt within 1 s | #72 | TC-NF-PERF-02 | `test_reliability_performance`: 20 answers, average ≈ 1 µs per answer | P |
| OQS-NF-003 | No crash or score corruption over 100 mixed inputs | #72 | TC-NF-REL-01 | `test_reliability_performance` (100-line run), `test_robustness` | P |
| OQS-NF-004 | Consistent, unambiguous prompts | #71 | TC-NF-UX-01 | Prompt/error conventions in README; `test_app`, `test_tui`; manual review of every screen | P |
| OQS-NF-005 | Compiles with a standard C++17 compiler | #73 | TC-NF-PORT-01 | CI "C++ Portability": g++ and clang++ build and pass all tests with `-Werror`; Windows MSYS2 g++ | P |

## Security requirements

The SAD (section 3.9) defines OQS-SR-001 to OQS-SR-005 for the console version; those definitions are used here.

| Req ID | Requirement | Delivered in | Test case | Evidence | Status |
|---|---|---|---|---|---|
| OQS-SR-001 | Validate all input before use (bounded length, no control characters) | #72 | TC-SEC-01 | `test_input_hardening` (256/257 chars, 1 MB line, control characters); CI bans `gets`, `scanf("%s")`, `strcpy` | P |
| OQS-SR-002 | Bounds-check question and option indexes | #74 | TC-SEC-02 | `test_question_bank_validation` (`isValidQuestionIndex`, `isValidOptionIndex`, `getQuestion` throws) | P |
| OQS-SR-003 | Reject records whose answer is not a displayed option | #74 | TC-SEC-03 | `test_question_bank_validation` (every malformed case, 100 KB text) | P |
| OQS-SR-004 | Input is data only; no command execution or format strings | #72, final integration | TC-SEC-04 | `test_input_hardening` (shell-like and `%n%s` input); `scripts/check_unsafe_calls.sh` runs in CI | P |
| OQS-SR-005 | Score stays within 0..total and resets on replay | #73, final integration | TC-SEC-05 | `test_results` (bounds, per-answer checks, reset); app refuses an out-of-range score | P |

## Coverage

- 30 requirements, 30 mapped to at least one executed test case, 30 Pass.
- Detailed results per area: [question bank](test-results/question-bank.md), [input and quiz engine](test-results/input-and-quiz-engine.md), [scoring and results](test-results/scoring-and-results.md). The overall outcome is in the [Test Summary Report](TEST_SUMMARY.md).
