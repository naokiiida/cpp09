#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Test counter
PASS=0
FAIL=0

# Test function
test_rpn() {
    local input="$1"
    local expected="$2"
    local output=$(./RPN "$input" 2>&1)

    if [ "$output" = "$expected" ]; then
        echo -e "${GREEN}✓${NC} PASS: RPN \"$input\""
        ((PASS++))
    else
        echo -e "${RED}✗${NC} FAIL: RPN \"$input\""
        echo "  Expected: $expected"
        echo "  Got:      $output"
        ((FAIL++))
    fi
}

# Build the program
make re > /dev/null 2>&1

echo "Running RPN tests..."
echo

# Valid calculations
test_rpn "8 9 * 9 - 9 - 9 - 4 - 1 +" "42"
test_rpn "7 7 * 7 -" "42"
test_rpn "1 2 * 2 / 2 * 2 4 - +" "0"
test_rpn "1" "1"

# Error cases
test_rpn "(1 + 1)" "Error: Invalid expression"
test_rpn "1 2 + +" "Error: Invalid expression"
test_rpn "" "Error: Invalid expression"
test_rpn "1 2" "Error: Invalid expression"
test_rpn "10 0 /" "Error: Division by zero"
test_rpn "9 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 *" "Error: Integer overflow"

echo
echo -e "Results: ${GREEN}$PASS passed${NC}, ${RED}$FAIL failed${NC}"
