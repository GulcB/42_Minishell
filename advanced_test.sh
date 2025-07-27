#!/bin/bash

# Advanced test script for minishell that handles interactive shell output
# This script works with your current minishell without requiring code changes

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

MINISHELL="./minishell"
TESTER_DIR="./tester"
TEMP_DIR="/tmp/minishell_advanced_test_$$"
mkdir -p "$TEMP_DIR"

# Counters
TOTAL=0
PASSED=0
FAILED=0

# Options
VERBOSE=0
SHOW_EXPECTED=1

print_status() {
    case $1 in
        "PASS") echo -e "${GREEN}✅ PASS${NC}: $2" ;;
        "FAIL") echo -e "${RED}❌ FAIL${NC}: $2" ;;
        "INFO") echo -e "${BLUE}ℹ️  INFO${NC}: $2" ;;
        "WARN") echo -e "${YELLOW}⚠️  WARN${NC}: $2" ;;
    esac
}

# Function to clean minishell output
clean_minishell_output() {
    local raw_output="$1"
    
    # Remove banner (first 4 lines), prompts, and exit
    echo "$raw_output" | \
    tail -n +5 | \
    sed 's/^minishell> //g' | \
    sed '/^exit$/d' | \
    sed '$d' 2>/dev/null || echo ""
}

# Function to test a single command
test_command() {
    local cmd="$1"
    local test_name="${2:-test}"
    
    [[ -z "$cmd" || "$cmd" =~ ^[[:space:]]*# ]] && return
    
    TOTAL=$((TOTAL + 1))
    
    # Get bash output
    local bash_output
    bash_output=$(timeout 3s bash -c "$cmd" 2>&1)
    local bash_exit=$?
    
    # Get minishell output
    local minishell_raw
    minishell_raw=$(timeout 5s printf "%s\nexit\n" "$cmd" | "$MINISHELL" 2>&1)
    local minishell_exit=$?
    
    # Clean minishell output
    local minishell_output
    minishell_output=$(clean_minishell_output "$minishell_raw")
    
    # Compare outputs
    if [[ "$bash_output" == "$minishell_output" ]]; then
        print_status "PASS" "$test_name: $cmd"
        PASSED=$((PASSED + 1))
    else
        print_status "FAIL" "$test_name: $cmd"
        FAILED=$((FAILED + 1))
        
        if [[ $VERBOSE -eq 1 ]] || [[ $SHOW_EXPECTED -eq 1 ]]; then
            echo "    Expected: '$bash_output'"
            echo "    Got:      '$minishell_output'"
            
            if [[ $VERBOSE -eq 1 ]]; then
                echo "    Raw minishell output:"
                echo "$minishell_raw" | sed 's/^/      /' | head -8
            fi
            echo
        fi
    fi
}

# Function to run tests from file
run_test_file() {
    local file="$1"
    local name=$(basename "$file" .sh)
    
    if [[ ! -f "$file" ]]; then
        print_status "WARN" "Test file not found: $file"
        return
    fi
    
    echo -e "${BLUE}━━━ Running $name tests ━━━${NC}"
    
    local count=0
    while IFS= read -r line; do
        if [[ -n "$line" && ! "$line" =~ ^[[:space:]]*# ]]; then
            test_command "$line" "$name"
            count=$((count + 1))
            
            # Progress indicator for large files
            if [[ $count -gt 0 && $((count % 10)) -eq 0 ]]; then
                echo "    ... processed $count commands"
            fi
        fi
    done < "$file"
    
    echo
}

# Check prerequisites
check_setup() {
    if [[ ! -f "$MINISHELL" ]]; then
        print_status "INFO" "Compiling minishell..."
        if ! make >/dev/null 2>&1; then
            print_status "FAIL" "Failed to compile minishell"
            exit 1
        fi
    fi
    
    if [[ ! -d "$TESTER_DIR" ]]; then
        print_status "FAIL" "Tester directory not found"
        exit 1
    fi
}

# Print usage
usage() {
    cat << EOF
Usage: $0 [OPTIONS] [TEST_NUMBERS...]

OPTIONS:
    -v, --verbose       Show detailed output including raw minishell output
    -q, --quiet         Show only summary (minimal output)
    -h, --help          Show this help
    --no-expected       Don't show expected vs got output for failures

TEST_NUMBERS:
    0   - Compare parsing (0_compare_parsing.sh)
    1   - Simple commands (1_scmds.sh)  
    2   - Builtins (1_builtins.sh)
    3   - Variables (1_variables.sh)
    4   - Redirections (1_redirs.sh)
    5   - Pipelines (1_pipelines.sh)
    6   - Correction (2_correction.sh)
    7   - Path check (2_path_check.sh)
    8   - Syntax errors (8_syntax_errors.sh)
    9   - Go wild (9_go_wild.sh)
    10  - Parsing hell (10_parsing_hell.sh)

Examples:
    $0                      # Run all tests
    $0 1 2                  # Run only simple commands and builtins
    $0 -v 0                 # Run parsing tests with verbose output
    $0 --quiet              # Run all tests quietly

EOF
}

# Parse arguments
TESTS_TO_RUN=()
while [[ $# -gt 0 ]]; do
    case $1 in
        -v|--verbose)
            VERBOSE=1
            shift
            ;;
        -q|--quiet)
            SHOW_EXPECTED=0
            shift
            ;;
        --no-expected)
            SHOW_EXPECTED=0
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        [0-9]|[0-9][0-9])
            TESTS_TO_RUN+=("$1")
            shift
            ;;
        *)
            echo "Unknown option: $1"
            usage
            exit 1
            ;;
    esac
done

# Test file mapping
declare -A TEST_FILES=(
    [0]="0_compare_parsing.sh"
    [1]="1_scmds.sh"
    [2]="1_builtins.sh"
    [3]="1_variables.sh"
    [4]="1_redirs.sh"
    [5]="1_pipelines.sh"
    [6]="2_correction.sh"
    [7]="2_path_check.sh"
    [8]="8_syntax_errors.sh"
    [9]="9_go_wild.sh"
    [10]="10_parsing_hell.sh"
)

# Main execution
main() {
    check_setup
    
    echo -e "${BLUE}🧪 Advanced Minishell Tester${NC}"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    
    # Determine which tests to run
    if [[ ${#TESTS_TO_RUN[@]} -eq 0 ]]; then
        # Run all tests
        for i in {0..10}; do
            if [[ -f "$TESTER_DIR/${TEST_FILES[$i]}" ]]; then
                run_test_file "$TESTER_DIR/${TEST_FILES[$i]}"
            fi
        done
    else
        # Run specified tests
        for test_num in "${TESTS_TO_RUN[@]}"; do
            local test_file="${TEST_FILES[$test_num]}"
            if [[ -n "$test_file" && -f "$TESTER_DIR/$test_file" ]]; then
                run_test_file "$TESTER_DIR/$test_file"
            else
                print_status "WARN" "Test $test_num not found or invalid"
            fi
        done
    fi
    
    # Summary
    echo -e "${BLUE}━━━ Summary ━━━${NC}"
    echo "Total tests: $TOTAL"
    echo -e "Passed: ${GREEN}$PASSED${NC}"
    echo -e "Failed: ${RED}$FAILED${NC}"
    
    if [[ $FAILED -eq 0 ]]; then
        echo -e "${GREEN}🎉 All tests passed!${NC}"
        exit 0
    else
        local success_rate=$((PASSED * 100 / TOTAL))
        echo -e "Success rate: ${YELLOW}${success_rate}%${NC}"
        
        if [[ $success_rate -gt 80 ]]; then
            echo -e "${GREEN}👍 Great job! Most tests are passing.${NC}"
        elif [[ $success_rate -gt 60 ]]; then
            echo -e "${YELLOW}👌 Good progress! Keep working on the failing tests.${NC}"
        else
            echo -e "${RED}💪 Keep working! Focus on basic functionality first.${NC}"
        fi
        exit 1
    fi
}

# Cleanup on exit
trap 'rm -rf "$TEMP_DIR"' EXIT

# Run main function
main "$@"
