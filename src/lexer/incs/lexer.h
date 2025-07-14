#ifndef LEXER_H
# define LEXER_H

# include "../../../lib/libft/libft.h"
# include "../../garbage_collector/inc/garbage_collector.h"
# include "../../executor/inc/executor.h"
# include "token.h"
# include <stddef.h>
# include <stdlib.h>

typedef struct s_lexer
{
	char		*input;
	int			position;
	int			read_position;
	char		current_char;
}				t_lexer;

int				ft_isspace(int c);
void			skip_whitespace(t_lexer *lexer);
char			*read_single_quoted_string(t_lexer *lexer);
char			*read_double_quoted_string(t_lexer *lexer);
char			*read_word(t_lexer *lexer);
char			*read_heredoc_delimiter(t_lexer *lexer);
int				is_valid_token_sequence(t_token *prev, t_token *current);

t_lexer			*lexer_create(char *input);
void			lexer_free(t_lexer *lexer);
void			lexer_read_char(t_lexer *lexer);
char			lexer_peek_char(t_lexer *lexer);
t_token			*create_heredoc_token(t_lexer *lexer, int start_pos);
t_token			*lexer_next_token(t_lexer *lexer);
t_token			*lexer_tokenize(char *input);

char			*expand_variables(const char *str, t_exec_context *ctx);
char			*process_token_expansion(t_token *token, t_exec_context *ctx);

#endif