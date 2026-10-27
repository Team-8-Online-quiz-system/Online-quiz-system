# Input and Quiz Engine — Sprint 2 Test Results

> Complete the actual-result and status columns only after running the tests. Do not report a test as passed until its output has been checked.

|Test ID|Requirement / area|Test procedure|Expected result|Actual result|Status|
|-|-|-|-|-|-|
|TC-INPUT-01|Input length|Test 256-character and 257-character strings with `isSafeInput`.|256 accepted; 257 rejected.|256 characters accepted; 257 rejected|Pass|
|TC-INPUT-02|Control characters|Test ESC, NUL, BEL and DEL.|All disallowed control characters rejected.|ESC, NUL, BEL and DEL rejected|Pass|
|TC-INPUT-03|Answer parsing|Test valid A-D / 1-4 and invalid E, 5, 0, empty and multi-character inputs.|Valid options parsed; invalid values rejected.|Valid answers accepted; invalid answers rejected|Pass|
|TC-SEC-01|Malicious-looking input|Submit shell-like and format-string-looking text as answers.|Input is treated as data and rejected; no payload is executed or echoed.|Malicious-looking inputs rejected safely|Pass|
|TC-ROB-01|Mixed invalid input|Run `build/test\\\_robustness`.|Quiz remains stable and accepts a later valid answer for each question.|Quiz remained stable with mixed invalid inputs|Pass|
|TC-REL-01|100 input lines|Run `build/test\\\_reliability\\\_performance`.|Exactly 100 lines are consumed; quiz completes.|Quiz completed with 100 input lines|Pass|
|TC-PERF-01|Response time|Run the 20-question timing test.|Average is below one second per answer; record the printed measurement.|Average response time: 20-question test completed in 2.46e-05 seconds total, averaging 1.23e-06 seconds per answer|Pass|
|TC-SCRIPT-01|Unsafe-call scan|Run `bash scripts/check\\\_unsafe\\\_calls.sh` from the repository root.|Script exits successfully if no listed unsafe calls are found.|Security script completed successfully|Pass|

## Execution summary

## Tests executed: 8

## Passed: 8

## Failed: 0

## Not yet executed: 0

## Environment

* Operating system: TODO
* Compiler and version: TODO (`g++ --version`)
* Build command: `mingw32-make`
* Test command: `mingw32-make test`
* Branch: `feature/input-hardening`

## Defects and retests

Record any failures, the corrective commit, and the retest output here.

