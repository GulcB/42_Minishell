NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./lib
LIBFT_DIR = ./lib/libft 
LIBFT = $(LIBFT_DIR)/libft.a

SRC_MAIN = ./src/minishell.c

SRC_BUILTIN = ./src/builtin/cd.c \
			./src/builtin/echo.c \
			./src/builtin/env.c \
			./src/builtin/exit.c \
			./src/builtin/export.c \
			./src/builtin/pwd.c \
			./src/builtin/unset.c 

SRC_EXECUTOR = ./src/executor/environment.c \
			./src/executor/executor.c \
			./src/executor/pipe.c \
			./src/executor/redirections.c

SRC_GC = ./src/garbage_collector/garbage_collector.c \
		./src/garbage_collector/gc_clean.c

SRC_LEXER = ./src/lexer/lexer.c \
			./src/lexer/token.c \
			./src/lexer/helper/lexer_string_op.c \
			./src/lexer/helper/token_type_op.c

SRC_PARSER = ./src/parser/parser.c \
			./src/parser/ast_helper.c \
			./src/parser/ast_nodes.c \
			./src/parser/parse_command.c \
			./src/parser/parse_redirect.c \
			./src/parser/print_ast_args.c \
			./src/parser/test_parser.c

SRCS = $(SRC_MAIN) $(SRC_BUILTIN) $(SRC_EXECUTOR) $(SRC_GC) $(SRC_LEXER) $(SRC_PARSER)


OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

%.o: %.c
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

.PHONY: all clean fclean re