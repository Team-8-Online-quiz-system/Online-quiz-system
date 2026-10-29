# Question Bank — Test Results (Sprint 2)

| Item | Value |
|---|---|
| Tester | Jishnu |
| Build | 6e493c1 |
| Date | 2026-10-28 |
| Environment | Arch Linux, g++ (GCC) 16.2.1 |

| Test case | Requirement | Steps | Expected result | Actual result | Status | Defect |
|---|---|---|---|---|---|---|
| TC-QB-01 | OQS-F-004 | Execute `loadQuestions()` to load the built-in question bank | 5 valid questions returned; no errors logged to `std::cerr` | `[PASS] loadQuestions returns 5 built-in questions`<br>`[PASS] all questions returned by loadQuestions are valid` | Pass | — |
| TC-QB-02 | OQS-SR-003 | Execute `filterValidQuestions()` with a mixed list of valid and malformed questions | Valid records retained in original order; each rejected record logged to error stream | `[PASS] filterValidQuestions retains exactly 2 valid questions from mixed list`<br>`[PASS] filterValidQuestions outputs expected error lines` | Pass | — |
| TC-SEC-03 | OQS-SR-003 | Call `questionError()` with a 100 KB question text string | Safely rejected without crash or buffer overflow, returning `"question text is longer than 512 characters"` | `[PASS] 100 KB question text rejected safely without crash (TC-SEC-03)` | Pass | — |
| TC-NF-PERF-01 | OQS-NF-001 | Benchmark `filterValidQuestions()` on 1,000 items and `loadQuestions()` across 10 iterations | 1,000 item validation < 2,000 ms; each load execution < 2,000 ms | `[INFO] filterValidQuestions(1,000 items) elapsed: 2 ms`<br>`[PASS] 1,000 question validation startup time is under 2 seconds (TC-NF-PERF-01)`<br>`[INFO] loadQuestions() 10 runs max elapsed: 0 ms`<br>`[PASS] loadQuestions() 10 iterations each complete in under 2 seconds (OQS-NF-001)` | Pass | — |
| TC-SR-002-01 | OQS-SR-002 | Test `isValidQuestionIndex()` on 2-item bank with index 0, size-1, size, huge index, and empty bank | Indices 0 and 1 return `true`; indices >= size and empty bank return `false` | `[PASS] isValidQuestionIndex returns true for index 0`<br>`[PASS] isValidQuestionIndex returns true for index size-1`<br>`[PASS] isValidQuestionIndex returns false for index size`<br>`[PASS] isValidQuestionIndex returns false for huge index`<br>`[PASS] isValidQuestionIndex returns false for empty bank` | Pass | — |
| TC-SR-002-02 | OQS-SR-002 | Test `isValidOptionIndex()` on 4-option question with indices 0, 3, -1, 4 | Options 0 and 3 return `true`; options -1 and 4 return `false` | `[PASS] isValidOptionIndex returns true for option 0`<br>`[PASS] isValidOptionIndex returns true for option 3`<br>`[PASS] isValidOptionIndex returns false for negative option`<br>`[PASS] isValidOptionIndex returns false for option 4` | Pass | — |
| TC-SR-002-03 | OQS-SR-002 | Call `getQuestion()` with valid index 0 and out-of-range index 1 on 1-item bank | Index 0 returns reference to question; index 1 throws `std::out_of_range` | `[PASS] getQuestion returns expected question at index 0`<br>`[PASS] getQuestion throws std::out_of_range when index >= bank.size()` | Pass | — |
| TC-SR-003-01 | OQS-SR-003 | Call `questionError()` on malformed questions (blank text, 513-char text, 3 options, 5 options, empty option, 257-char option, duplicate option) | Returns exact error reason string for each condition in precedence order | Exact match for all reasons: `"question text is empty"`, `"question text is longer than 512 characters"`, `"expected 4 options, found <count>"`, `"option <N> is empty"`, `"option <N> is longer than 256 characters"`, `"option <N> duplicates option <M>"` | Pass | — |
| TC-SR-003-02 | OQS-SR-003 | Call `questionError()` with `correctIndex` set to -1, 4, and 100 | Returns `"correct answer is not one of the options"` | `[PASS] correctIndex -1 returns correct answer is not one of the options`<br>`[PASS] correctIndex 4 returns correct answer is not one of the options`<br>`[PASS] correctIndex 100 returns correct answer is not one of the options` | Pass | — |

## Summary
9 test cases executed, 9 passed, 0 failed.

## Defects
None.
