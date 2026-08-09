#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")"
OUT=run_tests_results.txt
: > "$OUT"
echo "Running ex00 automated tests" >> "$OUT"

# Test 1: no args
echo "\n=== Test: no args ===" | tee -a "$OUT"
( ./btc ) 2>>"$OUT" || true

# Test 2: example input
echo "\n=== Test: input.txt ===" | tee -a "$OUT"
./btc input.txt 2>>"$OUT" | tee -a "$OUT" || true

# Test 3: only header
echo "\n=== Test: onlyheader.txt ===" | tee -a "$OUT"
./btc onlyheader.txt 2>>"$OUT" | tee -a "$OUT" || true

# Test 4: empty file
echo "\n=== Test: empty.txt ===" | tee -a "$OUT"
./btc empty.txt 2>>"$OUT" | tee -a "$OUT" || true

# Test 5: with blank lines
echo "\n=== Test: withblank.txt ===" | tee -a "$OUT"
./btc withblank.txt 2>>"$OUT" | tee -a "$OUT" || true

# Test 6: malformed custom cases
cat > malformed.txt <<'EOF'
date | value
badline
2011-01-03 | notanumber
2011-13-01 | 5
2011-02-30 | 5
EOF
echo "\n=== Test: malformed cases ===" | tee -a "$OUT"
./btc malformed.txt 2>>"$OUT" | tee -a "$OUT" || true
rm -f malformed.txt

# Test 7: large generated input (10k lines)
N=10000
echo "\n=== Test: large input N=$N ===" | tee -a "$OUT"
# Extract dates from data.csv (skip header) and sample N dates with replacement
tail -n +2 data.csv | awk -F',' '{print $1}' > dates_all.txt
# If not enough unique dates, allow repetition via shuf -n with replacement by looping
if command -v shuf >/dev/null 2>&1; then
  shuf -n $N dates_all.txt | awk 'BEGIN{srand()} {printf "%s | %d\n", $0, int(rand()*1000)}' > large_input.txt
else
  awk 'BEGIN{srand()} NR<=ENVIRON["N"]{print} END{while(NR<ENVIRON["N"]) {print "2011-01-01"; NR++}}' dates_all.txt > large_input.txt
fi
# prepend header
sed -i '1i date | value' large_input.txt
# Run and measure time (real time in seconds)
/usr/bin/time -f "TIME: %e sec" -o time_tmp.txt ./btc large_input.txt 2>>"$OUT" | sed -n '1,5p' >>"$OUT" || true
cat time_tmp.txt >>"$OUT" && rm -f time_tmp.txt large_input.txt dates_all.txt

# Test 8: valgrind memcheck on medium input (1k)
echo "\n=== Test: valgrind (1000 lines) ===" | tee -a "$OUT"
N=1000
tail -n +2 data.csv | awk -F',' '{print $1}' | shuf -n $N | awk 'BEGIN{srand()} {printf "%s | %d\n", $0, int(rand()*1000)}' > medium_input.txt
# prepend header
sed -i '1i date | value' medium_input.txt
valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=0 ./btc medium_input.txt >>"$OUT" 2>&1 || true
rm -f medium_input.txt

echo "\nAll tests finished. Results in $OUT"
exit 0
