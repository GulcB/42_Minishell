#!/bin/bash

# Improved test script that properly handles minishell output

MINISHELL="./minishell"
VERBOSE=0

if [[ $# -eq 0 ]]; then
    echo "Usage: $0 <test_number> [-v]"
    echo "Test numbers: 0-10"
    exit 1
fi

# Parse arguments
TEST_NUM=""
while [[ $# -gt 0 ]]; do
    case $1 in
        -v) VERBOSE=1; shift ;;
        [0-9]*) TEST_NUM=$1; shift ;;
        *) echo "Unknown option: $1"; exit 1 ;;
    esac
done

# Map test numbers
case $TEST_NUM in
    0) TEST_FILE="./tester/0_compare_parsing.sh" ;;
    1) TEST_FILE="./tester/1_scmds.sh" ;;
    2) TEST_FILE="./tester/1_builtins.sh" ;;
    *) echo "Invalid test: $TEST_NUM"; exit 1 ;;
esac

if [[ ! -f "$TEST_FILE" ]]; then
    echo "Test file not found: $TEST_FILE"
    exit 1
fi

if [[ ! -f "$MINISHELL" ]]; then
    echo "Compiling minishell..."
    make || exit 1
fi

echo "🧪 Testing $(basename "$TEST_FILE")"
echo "=" $(printf '=%.0s' {1..50})

PASSED=0
FAILED=0
TOTAL=0

# Function to extract clean output
extract_output() {
    local raw="$1"
    local cmd="$2"
    
    # Find the line with our command and get everything until next prompt or exit
    echo "$raw" | awk -v cmd="$cmd" '
    BEGIN { found=0; output="" }
    /^minishell> / { 
        if (found && $0 != "minishell> exit") {
            exit
        }
        if (index($0, cmd) > 0) {
            found=1
            next
        }
    }
    found && !/^minishell> / && !/^exit$/ {
        if (output != "") output = output "\n"
        output = output $0
    }
    END { print output }
    '
}

while IFS= read -r cmd; do
    [[ -z "$cmd" || "$cmd" =~ ^[[:space:]]*# ]] && continue
    
    TOTAL=$((TOTAL + 1))
    
    # Get bash output
    bash_out=$(timeout 3s bash -c "$cmd" 2>&1)
    bash_exit=$?
    
    # Get minishell raw output
    minishell_raw=$(timeout 5s printf "%s\nexit\n" "$cmd" | "$MINISHELL" 2>&1)
    minishell_exit=$?
    
    # Extract clean output
    minishell_out=$(extract_output "$minishell_raw" "$cmd")
    
    # Compare
    if [[ "$bash_out" == "$minishell_out" ]]; then
        echo "✅ PASS: $cmd"
        PASSED=$((PASSED + 1))
    else
        echo "❌ FAIL: $cmd"
        FAILED=$((FAILED + 1))
        echo "-------------------------------------------------"
        echo "   bash     : '$bash_out'"
        echo "   minishell: '$minishell_out'"
        echo "-------------------------------------------------"
        
        if [[ $VERBOSE -eq 1 ]]; then
            echo "   Raw minishell:"
            echo "$minishell_raw" | sed 's/^/     /'
        fi
        echo
    fi
    
done < "$TEST_FILE"

echo "=" $(printf '=%.0s' {1..50})
echo "📊 Sonuçlar: $PASSED başarılı, $FAILED başarısız, $TOTAL toplam"
echo "📈 Başarı oranı: $(( PASSED * 100 / TOTAL ))%"
