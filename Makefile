NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./lib
LIBFT_DIR = ./lib/libft 
LIBFT = $(LIBFT_DIR)/libft.a

SRC_MAIN = ./src/minishell.c

SRC_BUILTIN = ./src/builtin/cd_pwd_cmd.c \
			./src/builtin/echo.c \
			./src/builtin/env.c

SRC_LEXER = ./src/lexer/lexer.c \
			./src/lexer/token.c \
			./src/lexer/helper/dolar_check.c \
			./src/lexer/helper/lexer_string_op.c \
			./src/lexer/helper/qm_check.c \
			./src/lexer/helper/token_type_op.c

SRC_PARSER = ./src/parser/parse_arg.c

SRCS = $(SRC_MAIN) $(SRC_BUILTIN) $(SRC_LEXER) $(SRC_PARSER)


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