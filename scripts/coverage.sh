#!/usr/bin/env bash
# Builds the Online Quiz System with coverage instrumentation, runs every test
# program plus a full quiz, and prints line and branch coverage per file.
# Run from the project folder:   bash scripts/coverage.sh
# Builds in a temporary folder, so the repository is left untouched.
set -euo pipefail

if [[ ! -d src || ! -d tests ]]; then
    echo "Run this from the project folder (the one containing src/ and tests/)." >&2
    exit 1
fi

CXX=${CXX:-g++}
GCOV=${GCOV:-gcov}
WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT
mkdir -p "$WORK/obj" "$WORK/bin"
FLAGS="-std=c++17 -O0 --coverage -Isrc"

echo "== Building with coverage instrumentation"
for src in src/*.cpp; do
    "$CXX" $FLAGS -c "$src" -o "$WORK/obj/$(basename "$src" .cpp).o"
done
LIB=$(ls "$WORK"/obj/*.o | grep -v '/main\.o$')

echo "== Running tests"
failed=0
for t in tests/test_*.cpp; do
    name=$(basename "$t" .cpp)
    "$CXX" $FLAGS "$t" $LIB -o "$WORK/bin/$name"
    if output=$("$WORK/bin/$name" 2>&1); then
        printf '  %-32s %3d checks passed\n' "$name" "$(grep -c '^\[PASS\]' <<<"$output")"
    else
        printf '  %-32s FAILED\n' "$name"
        failed=1
    fi
done
"$CXX" --coverage "$WORK"/obj/*.o -o "$WORK/bin/quiz"
printf '1\nTeam8\nB\nA\nB\nC\nB\n1\nA\nA\nA\nA\nA\n2\n' | "$WORK/bin/quiz" --plain >/dev/null
echo "  full quiz run (play, replay, exit)  ok"

echo
echo "== Coverage"
printf '%-24s %14s %22s\n' "File" "Lines" "Branches"
total_l=0; hit_l=0; total_b=0; hit_b=0
for src in src/*.cpp; do
    report=$("$GCOV" -t -b -o "$WORK/obj" "$src" 2>/dev/null | awk -v f="$src" '
        /^ *-: *0:Source:/ { mine = index($0, f) > 0; next }
        !mine { next }
        /^branch/ { if ($0 ~ /\(throw\)/) next; bt++; if ($0 ~ /taken [1-9]/) bh++; next }
        /^ *#####:/ { lt++; next }
        /^ *[0-9]+\*?:/ { lt++; lh++ }
        END { printf "%d %d %d %d", lh, lt, bh, bt }')
    read -r lh lt bh bt <<<"$report"
    total_l=$((total_l + lt)); hit_l=$((hit_l + lh)); total_b=$((total_b + bt)); hit_b=$((hit_b + bh))
    printf '%-24s %5.1f%% %3d/%-3d %9.1f%% %4d/%-4d\n' "$(basename "$src")" \
        "$(awk -v h="$lh" -v t="$lt" 'BEGIN{print t ? 100*h/t : 0}')" "$lh" "$lt" \
        "$(awk -v h="$bh" -v t="$bt" 'BEGIN{print t ? 100*h/t : 0}')" "$bh" "$bt"
done
printf '%-24s %5.1f%% %3d/%-3d %9.1f%% %4d/%-4d\n' "TOTAL" \
    "$(awk -v h="$hit_l" -v t="$total_l" 'BEGIN{print 100*h/t}')" "$hit_l" "$total_l" \
    "$(awk -v h="$hit_b" -v t="$total_b" 'BEGIN{print 100*h/t}')" "$hit_b" "$total_b"
echo
echo "Branches count every if/else, loop and &&/|| outcome; compiler-generated"
echo "exception branches are excluded. terminal.cpp only runs in a real terminal."
exit $failed
