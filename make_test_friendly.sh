#!/bin/bash

# Script to make minishell test-friendly by adding non-interactive mode support

echo "This script will help make your minishell more test-friendly."
echo "The main issue is that your minishell always runs in interactive mode."
echo ""
echo "Here are the recommended changes to your minishell code:"
echo ""

cat << 'EOF'
# SOLUTION 1: Modify main() in src/minishell.c

Replace this section in main():
```c
print_banner();
configure_readline();
shell_loop(environment, main_gc);
```

With:
```c
// Check if we're running in interactive mode (connected to a terminal)
if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO)) {
    print_banner();
    configure_readline();
    shell_loop(environment, main_gc);
} else {
    // Non-interactive mode for testing
    shell_loop_non_interactive(environment, main_gc);
}
```

# SOLUTION 2: Add non-interactive shell loop function

Add this function to src/minishell.c:
```c
static void shell_loop_non_interactive(t_env *env, t_gc *main_gc)
{
    char *input;
    t_token *tokens;
    t_exec_context *ctx;
    int input_status;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    ctx = init_exec_context(env, main_gc);
    if (!ctx)
        return;
    
    // Read from stdin line by line
    while ((read = getline(&line, &len, stdin)) != -1) {
        // Remove newline if present
        if (line[read - 1] == '\n')
            line[read - 1] = '\0';
            
        input = ft_strdup(line);
        if (!input)
            continue;
            
        input_status = handle_input_validation(input);
        if (input_status == -1) {
            free(input);
            continue;
        }
        if (input_status == 0) {
            free(input);
            break;
        }
        
        if (!process_input_tokens(input, &tokens, main_gc)) {
            free(input);
            continue;
        }
        
        if (execute_and_cleanup(tokens, input, ctx) == -42) {
            free(input);
            break;
        }
        free(input);
    }
    
    if (line)
        free(line);
    restore_std_fds(ctx);
}
```

Don't forget to add #include <unistd.h> for isatty() function.

# SOLUTION 3: Alternative - Environment Variable Approach

You can also check for an environment variable:
```c
if (getenv("MINISHELL_TEST_MODE") != NULL) {
    // Test mode - no banner, no readline
    shell_loop_non_interactive(environment, main_gc);
} else {
    // Normal interactive mode
    print_banner();
    configure_readline();  
    shell_loop(environment, main_gc);
}
```

Then run tests with: MINISHELL_TEST_MODE=1 ./quick_test.sh 1
EOF

echo ""
echo "Which solution would you like to implement?"
echo "1) isatty() detection (recommended)"
echo "2) Environment variable approach"
echo "3) Show me the exact code changes needed"
echo "4) Create a test-friendly wrapper script instead"

read -p "Enter your choice (1-4): " choice

case $choice in
    1|3)
        echo ""
        echo "Creating patch file with isatty() solution..."
        cat > minishell_test_patch.txt << 'PATCH_EOF'
// Add this include at the top of src/minishell.c
#include <unistd.h>

// Add this function before main()
static void shell_loop_non_interactive(t_env *env, t_gc *main_gc)
{
    char *input;
    t_token *tokens;
    t_exec_context *ctx;
    int input_status;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    ctx = init_exec_context(env, main_gc);
    if (!ctx)
        return;
    
    while ((read = getline(&line, &len, stdin)) != -1) {
        if (line[read - 1] == '\n')
            line[read - 1] = '\0';
            
        input = ft_strdup(line);
        if (!input)
            continue;
            
        input_status = handle_input_validation(input);
        if (input_status == -1) {
            free(input);
            continue;
        }
        if (input_status == 0) {
            free(input);
            break;
        }
        
        if (!process_input_tokens(input, &tokens, main_gc)) {
            free(input);
            continue;
        }
        
        if (execute_and_cleanup(tokens, input, ctx) == -42) {
            free(input);
            break;
        }
        free(input);
    }
    
    if (line)
        free(line);
    restore_std_fds(ctx);
}

// Replace the main() function's last part:
// From:
    print_banner();
    configure_readline();
    shell_loop(environment, main_gc);

// To:
    if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO)) {
        print_banner();
        configure_readline();
        shell_loop(environment, main_gc);
    } else {
        shell_loop_non_interactive(environment, main_gc);
    }
PATCH_EOF
        echo "Patch saved to minishell_test_patch.txt"
        ;;
    2)
        echo "Use environment variable MINISHELL_TEST_MODE=1 approach"
        echo "Modify your main() function to check getenv(\"MINISHELL_TEST_MODE\")"
        ;;
    4)
        echo "Creating wrapper script..."
        cat > minishell_test_wrapper.sh << 'WRAPPER_EOF'
#!/bin/bash

# Wrapper script for testing minishell
# This script pipes commands to minishell and cleans the output

if [[ $# -eq 0 ]]; then
    echo "Usage: $0 <command>"
    echo "       $0 -f <file>"
    exit 1
fi

MINISHELL="./minishell"

if [[ "$1" == "-f" && -n "$2" ]]; then
    # Read commands from file
    while IFS= read -r cmd; do
        [[ -z "$cmd" || "$cmd" =~ ^[[:space:]]*# ]] && continue
        printf "%s\nexit\n" "$cmd" | "$MINISHELL" 2>&1 | sed -n '5,$p' | sed 's/^minishell> //g' | grep -v '^exit$' | head -n -1
    done < "$2"
else
    # Single command
    printf "%s\nexit\n" "$*" | "$MINISHELL" 2>&1 | sed -n '5,$p' | sed 's/^minishell> //g' | grep -v '^exit$' | head -n -1
fi
WRAPPER_EOF
        chmod +x minishell_test_wrapper.sh
        echo "Created minishell_test_wrapper.sh"
        echo "Usage: ./minishell_test_wrapper.sh 'echo hello'"
        ;;
esac
