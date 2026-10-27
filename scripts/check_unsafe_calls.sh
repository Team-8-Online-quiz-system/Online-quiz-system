#!/usr/bin/env bash
# OQS-SR-004: basic source scan for unsafe C/C++ calls.

set -u

if [[ ! -d "src" ]]; then
    echo "ERROR: run this script from the repository root (src/ not found)." >&2
    exit 2
fi

unsafe_pattern='(^|[^[:alnum:]_])(system|popen|execl|execv|execvp|gets|strcpy|sprintf)[[:space:]]*\('
matches="$(grep -RInE "$unsafe_pattern" src/ || true)"

if [[ -n "$matches" ]]; then
    echo "Potentially unsafe calls found:"
    printf '%s\n' "$matches"
    exit 1
fi

echo "PASS: no listed unsafe calls found in src/."
exit 0
