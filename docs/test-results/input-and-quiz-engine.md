# Input Hardening and Quiz Engine — Test Results

## Test execution environment

| Field | Recorded value |
|---|---|
| Operating system | Windows (version not recorded) |
| Shell | MSYS2 MinGW 64-bit |
| Compiler | GCC 12.1.0 (MSYS2 build) |
| Build tool | GNU Make 4.3 |
| Build command | `mingw32-make` |
| Test command | `mingw32-make test` |
| Branch | `feature/input-hardening` |
| Test execution date | 10 November 2026 |

## Test results

The identifiers below match the RTM identifiers specified for this work.

| RTM test ID | Test coverage and procedure | Expected result | Actual result | Status |
|---|---|---|---|---|
| TC-ANS-01 | Submit valid choices A–D and 1–4 to the answer parser. | Each valid choice maps to its option index. | Valid choices were accepted and mapped to their option indices. | Pass |
| TC-ANS-02 | Submit E, 5, 0, empty input, multiple characters, overlong input and disallowed control characters. | Invalid or unsafe answers are rejected and the quiz prompts again. | Invalid and unsafe answer cases were rejected; a subsequent valid answer was accepted. | Pass |
| TC-SEC-04 | Submit command-like strings, format-string-looking input, oversized input and control characters; scan `src/` for unsafe APIs. | Input is handled as data; unsafe APIs and non-literal printf formats are detected. | No listed unsafe APIs or non-literal printf format arguments found in `src/` (`bash scripts/check_unsafe_calls.sh` exited 0). Command-like and format-string inputs were rejected as invalid answers. | Pass |
| TC-NF-REL-01 | Run the 100-line reliability test (95 invalid and 5 valid lines) and mixed-invalid-input robustness test. | Quiz remains stable, records valid responses and completes normally. | 100-line test reported all checks passing; mixed-input robustness test passed in the test suite. | Pass |
| TC-NF-PERF-02 | Run the 20-question response-time test with 20 valid answers. | Quiz completes with average response time below one second per answer. | 20 answers completed in 2.46e-05 seconds total; average 1.23e-06 seconds per answer (approximately 1.23 microseconds). | Pass |

## Test execution summary

* RTM test IDs covered: 5
* Passed: 5
* Retest required: 0
* Failed: 0

## Notes

* The performance measurement is copied from the observed terminal output: `2.46e-05` seconds total and `1.23e-06` seconds average per answer.
* GCC 12.1.0 and GNU Make 4.3 are recorded from the terminal output. The exact Windows version was not captured.
* RTM identifiers used: `TC-ANS-01`, `TC-ANS-02`, `TC-SEC-04`, `TC-NF-REL-01`, and `TC-NF-PERF-02`.
