#ifndef TOKEN_H
# define TOKEN_H

# include "../../../lib/libft/libft.h"
# include "../../garbage_collector/inc/garbage_collector.h"
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

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

t_token_type		get_token_type(char c, char next_c);
int					is_quote_special_char(char c, int in_double_quote);
int					is_heredoc_start(char *input, int position);
int					find_matching_quote(char *input, int start, char quote_char);
int					is_valid_token_syntax(char *token_value, t_token_type type);

t_token				*token_create(t_token_type type, char *value, int positon);
void				token_add_back(t_token **tokens, t_token *new_token);
void				token_free_list(t_token *head);
char				*token_type_to_str(t_token_type type);
void				token_list_print(t_token *head);

#endif