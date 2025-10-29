#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Test counter
PASS=0
FAIL=0

# Test function
test_pmergeme() {
    local input="$1"
    local expected="$2"
    local output=$(./PmergeMe "$input" 2>&1)

    if [ "$output" = "$expected" ]; then
        echo -e "${GREEN}✓${NC} PASS: PmergeMe \"$input\""
        ((PASS++))
    else
        echo -e "${RED}✗${NC} FAIL: PmergeMe \"$input\""
        echo "  Expected: $expected"
        echo "  Got:      $output"
        ((FAIL++))
    fi
}

# Build the program
make re > /dev/null 2>&1

echo "Running PmergeMe tests..."
echo

# Valid calculations


# Error cases


echo
echo -e "Results: ${GREEN}$PASS passed${NC}, ${RED}$FAIL failed${NC}"
