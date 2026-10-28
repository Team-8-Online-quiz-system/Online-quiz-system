# Scoring and Results — Test Results (Sprint 2)

| Item | Value |
|---|---|
| Tester | Thanvitha Ippagunta (`thanvitha-ippagunta`) |
| Build | `4c64ca9` |
| Date | 2026-10-28 |
| Environment | Windows 11; MSYS2 UCRT64 g++ 16.2.0; MSYS2 CLANG64 clang++ 23.1.3 |

| Test case | Requirement | Steps | Expected result | Actual result | Status | Defect |
|---|---|---|---|---|---|---|
| TC-SCORE-01 | OQS-F-011, OQS-F-012 | Score a known set through `evaluateAnswer` and `updateScore`; check after each answer. | Correct answers add one; incorrect answers add zero; the score stays in range. | Both compiler builds succeeded. Local execution is blocked by Windows Application Control; GitHub Actions result pending. | Pending CI | — |
| TC-RES-01 | OQS-F-017 | Calculate and format 4/5, 5/5, 0/5, 1/3, and 2/3. | `80.0%`, `100.0%`, `0.0%`, `33.3%`, and `66.7%`. | Result test target compiled with g++ and clang++; execution pending GitHub Actions. | Pending CI | — |
| TC-RES-02 | OQS-F-017 | Calculate and format 0/0. | `0.0%`; no division by zero. | Result test target compiled with g++ and clang++; execution pending GitHub Actions. | Pending CI | — |
| TC-RES-03 | OQS-F-018 | Print Sarah's 4/5 summary; try scores -1 and 6/5. | Exact three-line summary; invalid scores throw before printing. | Result test target compiled with g++ and clang++; execution pending GitHub Actions. | Pending CI | — |
| TC-SEC-05 | OQS-SR-005 | Check scores 0, total, -1, and total+1; inspect score after each answer in a known sequence. | Only scores from zero through total are accepted; each valid update remains in range. | Result test target compiled with g++ and clang++; execution pending GitHub Actions. | Pending CI | — |
| Replay reset | OQS-F-019, OQS-SR-005 | Complete an attempt, call `resetSession`, then score a second attempt. | Score and question index reset; all responses become -1; second attempt gets its own percentage. | Result test target compiled with g++ and clang++; execution pending GitHub Actions. | Pending CI | — |
| TC-RES-E2E | OQS-F-016, OQS-F-017 | Score B,A,B,C,B and A,A,A,A,A against `answerKey(loadQuestions())`. | Scores and percentages are 5/5 = 100.0% and 1/5 = 20.0%. | Result test target compiled with g++ and clang++; execution pending GitHub Actions. | Pending CI | — |
| TC-NF-PORT-01 | OQS-NF-005 | Build the application and test targets with g++ and clang++. | Both compiler builds and test suites pass. | g++ 16.2.0 and clang++ 23.1.3 built the application and test targets with `-Werror`. Windows Application Control blocked local test execution; GitHub Actions matrix pending. | Pending CI | — |

## Summary

0 test cases executed locally; 0 passed; 0 failed. The application and test targets compiled under both local C++17 compilers. Test execution is pending the GitHub Actions runs because Windows Application Control blocks the generated executables in this environment.

## Defects

No code defects observed during compilation. Local test execution is blocked by the environment policy; GitHub Actions will provide the executable test results.
