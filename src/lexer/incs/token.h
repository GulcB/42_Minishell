#ifndef TOKEN_H
# define TOKEN_H

# include "../../../lib/libft/libft.h"
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>

typedef enum 		e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_SEMICOLON,
	TOKEN_DQUOTE,
	TOKEN_SQUOTE,
	TOKEN_DOLLAR,
	TOKEN_ERROR,
	TOKEN_EOF
}					t_token_type;

typedef struct 		s_token
{
	t_token_type	type;
	char			*value;
	int				positon;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

t_token				*token_create(t_token_type type, char *value, int positon);
void				token_add_back(t_token **tokens, t_token *new_token);
char				*token_type_to_str(t_token_type type);
void				token_list_print(t_token *head);

#endif