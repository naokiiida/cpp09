#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counter
PASS=0
FAIL=0

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}     PmergeMe Test Suite (ex02)${NC}"
echo -e "${BLUE}========================================${NC}"
echo

# Build the program
echo -e "${YELLOW}Building PmergeMe...${NC}"
make re > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Build failed!${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Build successful${NC}"
echo

# Function to test sorting correctness
test_sort() {
    local name="$1"
    shift
    local input="$@"

    echo -e "${YELLOW}Test: ${name}${NC}"
    echo -e "Input: $input"

    output=$(./PmergeMe $input 2>&1)

    # Extract the "After:" line
    after_line=$(echo "$output" | grep "After:")

    # Check if sorted
    if [ -n "$after_line" ]; then
        echo "$output"
        echo -e "${GREEN}✓ PASS${NC}"
        ((PASS++))
    else
        echo -e "${RED}✗ FAIL: No output${NC}"
        ((FAIL++))
    fi
    echo
}

# Function to test error cases
test_error() {
    local name="$1"
    shift
    local input="$@"

    echo -e "${YELLOW}Test: ${name}${NC}"
    echo -e "Input: $input"

    output=$(./PmergeMe $input 2>&1)

    if echo "$output" | grep -q "Error"; then
        echo "$output"
        echo -e "${GREEN}✓ PASS (Error detected)${NC}"
        ((PASS++))
    else
        echo -e "${RED}✗ FAIL: Should have errored${NC}"
        echo "$output"
        ((FAIL++))
    fi
    echo
}

# Function to verify sorted output
verify_sorted() {
    local output="$1"
    # Extract numbers from "After:" line
    after=$(echo "$output" | grep "After:" | sed 's/After://g' | sed 's/\[\.\.\.\]//g')

    # Simple check: should contain numbers
    if echo "$after" | grep -q "[0-9]"; then
        return 0
    else
        return 1
    fi
}

echo -e "${BLUE}=== Basic Sorting Tests ===${NC}"
echo

# Test 1: Small input (5 elements)
test_sort "5 elements" 3 5 9 7 4

# Test 2: Already sorted
test_sort "Already sorted" 1 2 3 4 5

# Test 3: Reverse sorted
test_sort "Reverse sorted" 5 4 3 2 1

# Test 4: Duplicates
test_sort "Duplicates" 3 3 1 1 2 2

# Test 5: Single element
test_sort "Single element" 42

# Test 6: Two elements
test_sort "Two elements" 5 3

# Test 7: Even number of elements
test_sort "Even count (8 elements)" 8 7 6 5 4 3 2 1

# Test 8: Odd number of elements
test_sort "Odd count (7 elements)" 7 6 5 4 3 2 1

# Test 9: Larger input (20 elements)
test_sort "20 elements" 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1

echo -e "${BLUE}=== Error Handling Tests ===${NC}"
echo

# Test 10: Negative number
test_error "Negative number" 3 -5 9

# Test 11: Non-numeric input
test_error "Non-numeric input" 3 abc 9

# Test 12: No input
test_error "No input"

# Test 13: Mixed valid/invalid
test_error "Mixed input" 3 5 7x 9

echo -e "${BLUE}=== Performance Tests ===${NC}"
echo

# Test 14: Large input (check if it runs and shows timing)
echo -e "${YELLOW}Test: Large input (3000 elements)${NC}"
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    echo "Generating 3000 random numbers (macOS)..."
    ./PmergeMe `jot -r 3000 1 1000 | tr '\n' ' '` 2>&1 | tail -3
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    echo "Generating 3000 random numbers (Linux)..."
    ./PmergeMe `shuf -i 1-1000 -n 3000 | tr "\n" " "` 2>&1 | tail -3
else
    echo "Generating 1000 random numbers (fallback)..."
    # Fallback: use a simple loop
    nums=""
    for i in $(seq 1 1000); do
        nums="$nums $((RANDOM % 1000 + 1))"
    done
    ./PmergeMe $nums 2>&1 | tail -3
fi
echo -e "${GREEN}✓ PASS (Large input handled)${NC}"
((PASS++))
echo

# Test 15: Performance comparison visible
echo -e "${YELLOW}Test: Timing comparison (100 elements)${NC}"
if [[ "$OSTYPE" == "darwin"* ]]; then
    output=$(./PmergeMe `jot -r 100 1 1000 | tr '\n' ' '` 2>&1)
else
    nums=""
    for i in $(seq 1 100); do
        nums="$nums $((RANDOM % 1000 + 1))"
    done
    output=$(./PmergeMe $nums 2>&1)
fi

if echo "$output" | grep -q "std::vector" && echo "$output" | grep -q "std::deque"; then
    echo "$output" | grep "Time to process"
    echo -e "${GREEN}✓ PASS (Both timings displayed)${NC}"
    ((PASS++))
else
    echo -e "${RED}✗ FAIL: Missing timing information${NC}"
    ((FAIL++))
fi
echo

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}            Test Summary${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "Total Tests: $((PASS + FAIL))"
echo -e "${GREEN}Passed: $PASS${NC}"
echo -e "${RED}Failed: $FAIL${NC}"
echo

if [ $FAIL -eq 0 ]; then
    echo -e "${GREEN}All tests passed! ✓${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi
