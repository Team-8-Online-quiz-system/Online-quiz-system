#!/usr/bin/env bash

# Security scan for unsafe C/C++ APIs and non-literal printf formats.
# Run from the project root.

set -u

if [[ ! -d src ]]; then
    echo "ERROR: Run this script from the project root."
    exit 2
fi

unsafe_pattern='(^|[^[:alnum:]_])(system|popen|execl|execv|execvp|ShellExecuteA|ShellExecuteW|ShellExecute|WinExec|gets|strcpy|sprintf)[[:space:]]*\('

printf_pattern='(^|[^[:alnum:]_])(printf|vprintf)[[:space:]]*\([[:space:]]*[^"[:space:]]|(^|[^[:alnum:]_])(fprintf|vfprintf|sprintf|vsprintf)[[:space:]]*\([^,]*,[[:space:]]*[^"[:space:]]|(^|[^[:alnum:]_])(snprintf|vsnprintf)[[:space:]]*\([^,]*,[^,]*,[[:space:]]*[^"[:space:]]'

files=(--include='*.c' --include='*.h' --include='*.cpp' --include='*.hpp' \
       --include='*.cc' --include='*.hh' --include='*.cxx' --include='*.hxx')

unsafe=$(grep -RInE "${files[@]}" "$unsafe_pattern" src/ 2>/dev/null || true)
printf_matches=$(grep -RInE "${files[@]}" "$printf_pattern" src/ 2>/dev/null || true)

failed=0

if [[ -n "$unsafe" ]]; then
    echo "FAIL: Potentially unsafe API calls found:"
    printf '%s\n' "$unsafe"
    failed=1
fi

if [[ -n "$printf_matches" ]]; then
    echo "FAIL: Non-literal printf format arguments found:"
    printf '%s\n' "$printf_matches"
    failed=1
fi

if [[ "$failed" -eq 1 ]]; then
    exit 1
fi

echo "PASS: No listed unsafe APIs or non-literal printf formats found in src/."
exit 0
