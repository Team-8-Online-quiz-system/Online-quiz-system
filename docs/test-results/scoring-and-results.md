# Scoring and Results — Test Results (Sprint 2)

| Item | Value |
|---|---|
| Tester | Thanvitha Ippagunta (`thanvitha-ippagunta`) |
| Build | `c7c952f` |
| Date | 2026-11-07 |
| Environment | Windows 11; MSYS2 UCRT64 g++ 16.2.0; MSYS2 CLANG64 clang++ 23.1.3 |

| Test case | Requirement | Steps | Expected result | Actual result | Status | Defect |
|---|---|---|---|---|---|---|
| TC-SCORE-01 | OQS-F-011, OQS-F-012 | Score a known set through `evaluateAnswer` and `updateScore`; check after each answer. | Correct answers add one; incorrect answers add zero; the score stays in range. | `test_scoring` passed all 15 checks under g++ and clang++. | Pass | — |
| TC-RES-01 | OQS-F-017 | Calculate and format 4/5, 5/5, 0/5, 1/3, and 2/3. | `80.0%`, `100.0%`, `0.0%`, `33.3%`, and `66.7%`. | All expected percentage values matched under both compilers. | Pass | — |
| TC-RES-02 | OQS-F-017 | Calculate and format 0/0. | `0.0%`; no division by zero. | Returned `0.0%` under both compilers. | Pass | — |
| TC-RES-03 | OQS-F-018 | Print Sarah's 4/5 summary; try scores -1 and 6/5. | Exact three-line summary; invalid scores throw before printing. | Exact summary matched; both invalid scores threw before writing output under both compilers. | Pass | — |
| TC-SEC-05 | OQS-SR-005 | Check scores 0, total, -1, and total+1; inspect score after each answer in a known sequence. | Only scores from zero through total are accepted; each valid update remains in range. | Boundary and per-answer score checks passed under both compilers. | Pass | — |
| Replay reset | OQS-F-019, OQS-SR-005 | Complete an attempt, call `resetSession`, then score a second attempt. | Score and question index reset; all responses become -1; second attempt gets its own percentage. | Reset state and second-attempt score/percentage checks passed under both compilers. | Pass | — |
| TC-RES-E2E | OQS-F-016, OQS-F-017 | Score B,A,B,C,B and A,A,A,A,A against `answerKey(loadQuestions())`. | Scores and percentages are 5/5 = 100.0% and 1/5 = 20.0%. | Both answer sets produced the expected scores and percentages under both compilers. | Pass | — |
| TC-NF-PORT-01 | OQS-NF-005 | Build the application and test targets with g++ and clang++; execute the test suite. | Both compiler builds and test suites pass. | Both compilers built the application and all six test targets with `-Werror`. Five test programs ran successfully under each compiler; the `test_question_bank` executable was blocked by Windows Application Control. | Partial | Environment blocked one test executable before launch. |

## Summary

Across the five test programs that Windows allowed to launch, 119 checks passed under g++ and 119 passed under clang++ (238 passing executions; zero assertion failures). The `test_question_bank` target compiled with both compilers but could not launch because Windows Application Control blocked that executable. GitHub Actions has not yet run for this branch.

## Defects

No code defects observed in the tests that ran. D-01 — Windows Application Control blocked `test_question_bank.exe` before execution; the target compiled under both compilers. GitHub Actions is still needed to verify that test in the supported Linux environment.
