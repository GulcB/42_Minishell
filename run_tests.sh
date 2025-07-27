#!/bin/bash

# **************************************************************************** #
#                              MINISHELL TESTER                               #
# **************************************************************************** #

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Directories
MINISHELL_PATH="./minishell"
TESTER_DIR="./tester"
TEMP_DIR="/tmp/minishell_test_$$"

# Create temporary directory for test outputs
mkdir -p "$TEMP_DIR"

# Function to print colored output
print_status() {
    local status=$1
    local message=$2
    
    case $status in
        "PASS")
            echo -e "${GREEN}[PASS]${NC} $message"
            ;;
        "FAIL")
            echo -e "${RED}[FAIL]${NC} $message"
            ;;
        "INFO")
            echo -e "${BLUE}[INFO]${NC} $message"
            ;;
        "WARN")
            echo -e "${YELLOW}[WARN]${NC} $message"
            ;;
        "HEADER")
            echo -e "${PURPLE}[====]${NC} $message"
            ;;
    esac
}

# Function to extract minishell output (skip banner and prompt)
extract_minishell_output() {
    local input_file="$1"
    local output_file="$2"
    
    # Skip the banner (first 4 lines) and extract everything between prompts
    # Remove "minishell> " prompts and "exit" line
    sed -n '5,$p' "$input_file" | \
    sed 's/^minishell> //g' | \
    grep -v '^exit$' | \
    head -n -1 > "$output_file"
}

# Function to run a single command test
run_command_test() {
    local cmd="$1"
    local test_name="$2"
    
    # Skip empty lines and comments
    if [[ -z "$cmd" || "$cmd" =~ ^[[:space:]]*# ]]; then
        return
    fi
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Run command in bash
    timeout 5s bash -c "$cmd" > "$TEMP_DIR/bash_output" 2>&1
    local bash_exit=$?
    
    # Run command in minishell and extract clean output
    timeout 10s printf "%s\nexit\n" "$cmd" | "$MINISHELL_PATH" > "$TEMP_DIR/minishell_raw" 2>&1
    local minishell_exit=$?
    
    # Extract clean output from minishell (skip banner and prompts)
    extract_minishell_output "$TEMP_DIR/minishell_raw" "$TEMP_DIR/minishell_output"
    
    # Compare outputs
    if diff -q "$TEMP_DIR/bash_output" "$TEMP_DIR/minishell_output" >/dev/null 2>&1; then
        print_status "PASS" "$test_name: $cmd"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        print_status "FAIL" "$test_name: $cmd"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        
        if [[ ${SHOW_DIFFS:-1} -eq 1 ]]; then
            echo "  Expected (bash):"
            cat "$TEMP_DIR/bash_output" | sed 's/^/    /'
            echo "  Got (minishell):"
            cat "$TEMP_DIR/minishell_output" | sed 's/^/    /'
            echo "  Raw minishell output:"
            cat "$TEMP_DIR/minishell_raw" | sed 's/^/    /' | head -10
            echo
        fi
    fi
}

# Function to run tests from a file
run_test_file() {
    local test_file="$1"
    local test_name=$(basename "$test_file" .sh)
    
    print_status "HEADER" "Running $test_name tests"
    
    if [[ ! -f "$test_file" ]]; then
        print_status "WARN" "Test file not found: $test_file"
        return
    fi
    
    # Read each line from the test file
    while IFS= read -r line; do
        # Skip comments and empty lines
        if [[ -n "$line" && ! "$line" =~ ^[[:space:]]*# ]]; then
            run_command_test "$line" "$test_name"
        fi
    done < "$test_file"
    
    echo
}

# Function to run interactive tests
run_interactive_tests() {
    print_status "HEADER" "Running interactive tests"
    
    # Test some basic interactive functionality
    local interactive_tests=(
        "echo hello"
        "pwd"
        "env | head -3"
        "export TEST_VAR=42; echo \$TEST_VAR"
        "cd /tmp; pwd; cd -"
        "echo \$?"
    )
    
    for cmd in "${interactive_tests[@]}"; do
        run_command_test "$cmd" "interactive"
    done
    
    echo
}

# Function to check prerequisites
check_prerequisites() {
    print_status "INFO" "Checking prerequisites..."
    
    # Check if minishell exists
    if [[ ! -f "$MINISHELL_PATH" ]]; then
        print_status "FAIL" "Minishell executable not found at $MINISHELL_PATH"
        print_status "INFO" "Attempting to compile minishell..."
        
        if make > /dev/null 2>&1; then
            print_status "PASS" "Minishell compiled successfully"
        else
            print_status "FAIL" "Failed to compile minishell"
            exit 1
        fi
    fi
    
    # Check if tester directory exists
    if [[ ! -d "$TESTER_DIR" ]]; then
        print_status "FAIL" "Tester directory not found at $TESTER_DIR"
        exit 1
    fi
    
    print_status "PASS" "All prerequisites met"
    echo
}

# Function to run syntax error tests
run_syntax_tests() {
    local syntax_file="$TESTER_DIR/8_syntax_errors.sh"
    
    if [[ -f "$syntax_file" ]]; then
        print_status "HEADER" "Running syntax error tests"
        
        while IFS= read -r line; do
            if [[ -n "$line" && ! "$line" =~ ^[[:space:]]*# ]]; then
                TOTAL_TESTS=$((TOTAL_TESTS + 1))
                
                # For syntax errors, we expect both bash and minishell to fail
                timeout 5s bash -c "$line" >/dev/null 2>&1
                local bash_exit=$?
                
                timeout 10s printf "%s\nexit\n" "$line" | "$MINISHELL_PATH" >/dev/null 2>&1
                local minishell_exit=$?
                
                # Both should return non-zero exit codes for syntax errors
                if [[ $bash_exit -ne 0 && $minishell_exit -ne 0 ]]; then
                    print_status "PASS" "syntax: $line"
                    PASSED_TESTS=$((PASSED_TESTS + 1))
                else
                    print_status "FAIL" "syntax: $line (bash: $bash_exit, minishell: $minishell_exit)"
                    FAILED_TESTS=$((FAILED_TESTS + 1))
                fi
            fi
        done < "$syntax_file"
        echo
    fi
}

# Function to print summary
print_summary() {
    print_status "HEADER" "Test Summary"
    echo "Total tests: $TOTAL_TESTS"
    echo -e "Passed: ${GREEN}$PASSED_TESTS${NC}"
    echo -e "Failed: ${RED}$FAILED_TESTS${NC}"
    
    if [[ $FAILED_TESTS -eq 0 ]]; then
        echo -e "${GREEN}All tests passed! 🎉${NC}"
    else
        local success_rate=$((PASSED_TESTS * 100 / TOTAL_TESTS))
        echo -e "Success rate: ${YELLOW}${success_rate}%${NC}"
    fi
}

# Cleanup function
cleanup() {
    rm -rf "$TEMP_DIR"
}

# Set up cleanup on exit
trap cleanup EXIT

# Main execution
main() {
    echo -e "${CYAN}**************************************************************************${NC}"
    echo -e "${CYAN}                              MINISHELL TESTER                           ${NC}"
    echo -e "${CYAN}**************************************************************************${NC}"
    echo
    
    check_prerequisites
    
    # Test files to run (in order)
    local test_files=(
        "$TESTER_DIR/0_compare_parsing.sh"
        "$TESTER_DIR/1_scmds.sh"
        "$TESTER_DIR/1_builtins.sh"
        "$TESTER_DIR/1_variables.sh"
        "$TESTER_DIR/1_redirs.sh"
        "$TESTER_DIR/1_pipelines.sh"
        "$TESTER_DIR/2_correction.sh"
        "$TESTER_DIR/2_path_check.sh"
        "$TESTER_DIR/9_go_wild.sh"
        "$TESTER_DIR/10_parsing_hell.sh"
    )
    
    # Run all test files
    for test_file in "${test_files[@]}"; do
        if [[ -f "$test_file" ]]; then
            run_test_file "$test_file"
        else
            print_status "WARN" "Test file not found: $test_file"
        fi
    done
    
    # Run syntax error tests separately
    run_syntax_tests
    
    # Run some interactive tests
    run_interactive_tests
    
    # Print final summary
    print_summary
    
    # Exit with appropriate code
    if [[ $FAILED_TESTS -eq 0 ]]; then
        exit 0
    else
        exit 1
    fi
}

# Parse command line arguments
case "${1:-}" in
    "-h"|"--help")
        echo "Usage: $0 [OPTIONS]"
        echo "Options:"
        echo "  -h, --help     Show this help message"
        echo "  -v, --verbose  Enable verbose output"
        echo "  -q, --quiet    Run quietly (minimal output)"
        echo "  --no-diffs     Don't show diff output for failed tests"
        exit 0
        ;;
    "-v"|"--verbose")
        set -x
        SHOW_DIFFS=1
        ;;
    "-q"|"--quiet")
        SHOW_DIFFS=0
        ;;
    "--no-diffs")
        SHOW_DIFFS=0
        ;;
esac

# Run main function
main "$@"
