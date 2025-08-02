NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror \
         -I./lib \
         -I./lib/libft \
         -Isrc \
         -Isrc/lexer/incs \
         -Isrc/parser/inc \
         -Isrc/builtin/inc \
         -Isrc/executor/inc \
         -Isrc/garbage_collector/inc

LIBFT_DIR = ./lib/libft 
LIBFT = $(LIBFT_DIR)/libft.a

SRC_MAIN = ./src/minishell.c \
			./src/shell_helpers.c

SRC_PARSER = ./src/parser/parser.c \
			./src/parser/ast_helper.c \
			./src/parser/ast_nodes.c \
			./src/parser/parse_command.c \
			./src/parser/parse_heredoc.c \
			./src/parser/parse_redirect.c \
			./src/parser/print_ast_args.c

SRC_LEXER = ./src/lexer/lexer.c \
			./src/lexer/token.c \
			./src/lexer/lexer_redirects.c \
			./src/lexer/lexer_tokens.c \
			./src/lexer/lexer_utils.c \
			./src/lexer/token_sequence.c \
			./src/lexer/helper/lexer_heredoc.c \
			./src/lexer/helper/lexer_quotes.c \
			./src/lexer/helper/lexer_string_op.c \
			./src/lexer/helper/lexer_word_parsing.c \
			./src/lexer/helper/token_type_checks.c \
			./src/lexer/helper/token_validation.c \
			./src/lexer/helper/variable_expansion_env.c \
			./src/lexer/helper/variable_expansion_process.c \
			./src/lexer/helper/variable_expansion.c \
			./src/lexer/helper/tilde_expansion.c

SRC_GC = 	./src/garbage_collector/garbage_collector.c \
			./src/garbage_collector/gc_clean.c \
			./src/garbage_collector/gc_helpers.c

SRC_BUILTIN = ./src/builtin/builtin_dispatcher.c \
			./src/builtin/cd.c \
			./src/builtin/echo.c \
			./src/builtin/env.c \
			./src/builtin/exit.c \
			./src/builtin/export.c \
			./src/builtin/pwd.c \
			./src/builtin/unset.c 

SRC_EXECUTOR = ./src/executor/ast_execution.c \
			./src/executor/command_execution.c \
			./src/executor/context_cleanup.c \
			./src/executor/context_management.c \
			./src/executor/env_ops.c \
			./src/executor/environment_management.c \
			./src/executor/exit_signal_check.c \
			./src/executor/heredoc_execution.c \
			./src/executor/path_resolution.c \
			./src/executor/pipe_chain.c \
			./src/executor/pipe_execution.c \
			./src/executor/process_management.c \
			./src/executor/recursive_pipe_execution.c \
			./src/executor/redirections.c \
			./src/executor/signal_handler.c

HEADERS = lib/minishell.h \
          src/lexer/incs/lexer.h \
          src/lexer/incs/token.h \
          src/parser/inc/parser.h \
          src/builtin/inc/builtin.h \
          src/executor/inc/executor.h \
          src/garbage_collector/inc/garbage_collector.h

SRCS = $(SRC_MAIN) $(SRC_BUILTIN) $(SRC_EXECUTOR) $(SRC_LEXER) $(SRC_GC) $(SRC_PARSER)

OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

clean:
	@make -C $(LIBFT_DIR) clean
	$(RM) $(OBJS)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	$(RM) $(NAME)

re: fclean all

run: re 
	$(RM) $(OBJS)
	@clear
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./$(NAME)

.PHONY: all clean fclean re run 