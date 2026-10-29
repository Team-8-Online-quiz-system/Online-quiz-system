# Scoring and Results — Test Results (Sprint 2)

| Item | Value |
|---|---|
| Tester | Thanvitha Ippagunta (`thanvitha-ippagunta`) |
| Build verified by CI | `cbc2cf2` |
| Date | 2026-10-28 |
| Local test execution | 2026-10-28 |
| CI verification | 2026-10-28 |
| Environment | Windows 11; MSYS2 UCRT64 g++ 16.2.0; MSYS2 CLANG64 clang++ 23.1.3 |

| Test case | Requirement | Steps | Expected result | Actual result | Status | Defect |
|---|---|---|---|---|---|---|
| TC-SCORE-01 | OQS-F-011, OQS-F-012 | Score a known set through `evaluateAnswer` and `updateScore`; check after each answer. | Correct answers add one; incorrect answers add zero; the score stays in range. | `test_scoring` passed all 15 local checks under g++ and clang++. | Pass | — |
| TC-RES-01 | OQS-F-017 | Calculate and format 4/5, 5/5, 0/5, 1/3, and 2/3. | `80.0%`, `100.0%`, `0.0%`, `33.3%`, and `66.7%`. | All expected percentage values matched locally under both compilers. | Pass | — |
| TC-RES-02 | OQS-F-017 | Calculate and format 0/0. | `0.0%`; no division by zero. | Returned `0.0%` locally under both compilers. | Pass | — |
| TC-RES-03 | OQS-F-018 | Print Sarah's 4/5 summary; try scores -1 and 6/5. | Exact three-line summary; invalid scores throw before printing. | Exact summary matched; both invalid scores threw before writing output under both compilers. | Pass | — |
| TC-SEC-05 | OQS-SR-005 | Check scores 0, total, -1, and total+1; inspect score after each answer in a known sequence. | Only scores from zero through total are accepted; each valid update remains in range. | Boundary and per-answer score checks passed locally under both compilers. | Pass | — |
| Replay reset | OQS-F-019, OQS-SR-005 | Complete an attempt, call `resetSession`, then score a second attempt. | Score and question index reset; all responses become -1; second attempt gets its own percentage. | Reset state and second-attempt score/percentage checks passed locally under both compilers. | Pass | — |
| TC-RES-E2E | OQS-F-016, OQS-F-017 | Score B,A,B,C,B and A,A,A,A,A against `answerKey(loadQuestions())`. | Scores and percentages are 5/5 = 100.0% and 1/5 = 20.0%. | Both answer sets produced the expected scores and percentages locally under both compilers. | Pass | — |
| TC-NF-PORT-01 | OQS-NF-005 | Build the application and test targets with g++ and clang++; execute the test suite in CI. | Both compiler builds and test suites pass. | GitHub Actions [C++ Portability run #1](https://github.com/Team-8-Online-quiz-system/Online-quiz-system/actions/runs/42391660155) passed both `g++` and `clang++` build-and-test jobs, including all test targets. The [Build and Test run #18](https://github.com/Team-8-Online-quiz-system/Online-quiz-system/actions/runs/42391660127) also passed. | Pass | — |

## Summary

Local Windows execution passed 119 checks under g++ and 119 under clang++ (238 passing executions; zero assertion failures). Windows Application Control blocked the `test_question_bank` executable locally. GitHub Actions subsequently passed the complete test suite under both `g++` and `clang++`; the separate Build and Test workflow also passed. See the linked run records in TC-NF-PORT-01.

## Defects

No code defects observed. D-01 — Windows Application Control blocked `test_question_bank.exe` locally; portability verification passed in the Linux CI runs under both compilers.
