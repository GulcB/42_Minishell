NAME = minishell
CC = cc
CGLAGS = -Wall -Wextra -Werror -I2
LIBFT_DIR = ./libft 
LIBFT = $(LIBFT_DIR)/libft.a
SRC = 
OBJS = $(SRC:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -o $(NAME)

clean:
	@make -C $(LIBFT_DIR) clean
	$(RM) $(OBJS)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	$(RM) $(NAME) $(OBJ)

re: fclean all

.PHONY: all clean fclean re