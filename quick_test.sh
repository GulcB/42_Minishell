#!/bin/bash

# Simple test runner for minishell - runs a specific test file

if [[ $# -eq 0 ]]; then
    echo "Usage: $0 <test_file_number> [options]"
    echo "Available tests:"
    echo "  0 - Compare parsing (0_compare_parsing.sh)"
    echo "  1 - Simple commands (1_scmds.sh)"
    echo "  2 - Builtins (1_builtins.sh)"
    echo "  3 - Variables (1_variables.sh)"
    echo "  4 - Redirections (1_redirs.sh)"
    echo "  5 - Pipelines (1_pipelines.sh)"
    echo "  6 - Correction tests (2_correction.sh)"
    echo "  7 - Path check (2_path_check.sh)"
    echo "  8 - Syntax errors (8_syntax_errors.sh)"
    echo "  9 - Go wild (9_go_wild.sh)"
    echo "  10 - Parsing hell (10_parsing_hell.sh)"
    echo ""
    echo "Options:"
    echo "  -v    Verbose mode (show differences)"
    echo "  -c    Compare only (no execution)"
    exit 1
fi

MINISHELL="./minishell"
VERBOSE=0
COMPARE_ONLY=0

# Parse options
while [[ $# -gt 0 ]]; do
    case $1 in
        -v)
            VERBOSE=1
            shift
            ;;
        -c)
            COMPARE_ONLY=1
            shift
            ;;
        [0-9]*)
            TEST_NUM=$1
            shift
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Map test numbers to files
case $TEST_NUM in
    0) TEST_FILE="./tester/0_compare_parsing.sh" ;;
    1) TEST_FILE="./tester/1_scmds.sh" ;;
    2) TEST_FILE="./tester/1_builtins.sh" ;;
    3) TEST_FILE="./tester/1_variables.sh" ;;
    4) TEST_FILE="./tester/1_redirs.sh" ;;
    5) TEST_FILE="./tester/1_pipelines.sh" ;;
    6) TEST_FILE="./tester/2_correction.sh" ;;
    7) TEST_FILE="./tester/2_path_check.sh" ;;
    8) TEST_FILE="./tester/8_syntax_errors.sh" ;;
    9) TEST_FILE="./tester/9_go_wild.sh" ;;
    10) TEST_FILE="./tester/10_parsing_hell.sh" ;;
    *) 
        echo "Invalid test number: $TEST_NUM"
        exit 1
        ;;
esac

if [[ ! -f "$TEST_FILE" ]]; then
    echo "Test file not found: $TEST_FILE"
    exit 1
fi

if [[ ! -f "$MINISHELL" ]]; then
    echo "Minishell not found. Compiling..."
    make || exit 1
fi

echo "Running test: $(basename "$TEST_FILE")"
echo "============================================"

PASSED=0
FAILED=0
TOTAL=0

while IFS= read -r cmd; do
    # Skip empty lines and comments
    [[ -z "$cmd" || "$cmd" =~ ^[[:space:]]*# ]] && continue
    
    TOTAL=$((TOTAL + 1))
    
    if [[ $COMPARE_ONLY -eq 1 ]]; then
        echo "Command: $cmd"
        continue
    fi
    
    # Run in bash
    bash_output=$(timeout 3s bash -c "$cmd" 2>&1)
    bash_exit=$?
    
    # Run in minishell
    minishell_raw=$(timeout 5s printf "%s\nexit\n" "$cmd" | "$MINISHELL" 2>&1)
    minishell_exit=$?
    
    # Extract clean output from minishell (skip banner and prompts)
    # First, get everything after the prompt for the command
    minishell_output=$(echo "$minishell_raw" | sed -n '/^minishell> .*'"$(echo "$cmd" | sed 's/[[\.*^$()+?{|]/\\&/g')"'/,/^minishell> exit$/p' | sed '1d;$d' | sed 's/^minishell> //g')
    
    if [[ "$bash_output" == "$minishell_output" ]]; then
        echo "✅ PASS: $cmd"
        PASSED=$((PASSED + 1))
    else
        echo "❌ FAIL: $cmd"
        FAILED=$((FAILED + 1))
        
        if [[ $VERBOSE -eq 1 ]]; then
            echo "  Expected (bash):"
            echo "$bash_output" | sed 's/^/    /'
            echo "  Got (minishell):"
            echo "$minishell_output" | sed 's/^/    /'
            echo "  Raw minishell:"
            echo "$minishell_raw" | sed 's/^/    /' | head -8
            echo
        fi
    fi
    
done < "$TEST_FILE"

echo "============================================"
echo "Results: $PASSED passed, $FAILED failed, $TOTAL total"
echo "Success rate: $(( PASSED * 100 / TOTAL ))%"

[[ $FAILED -eq 0 ]] && exit 0 || exit 1
