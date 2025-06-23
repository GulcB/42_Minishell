#ifndef LEXER_H
# define LEXER_H

# include "../../../lib/libft/libft.h"
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

t_lexer			*lexer_create(char *input);

void			lexer_read_char(t_lexer *lexer);
char			lexer_peek_char(t_lexer *lexer);

t_token			*lexer_next_token(t_lexer *lexer);
t_token			*lexer_tokenize(char *input);

void			skip_whitespace(t_lexer *lexer);
char			*read_word(t_lexer *lexer);
char			*read_single_quoted_string(t_lexer *lexer);
char			*read_double_quoted_string(t_lexer *lexer);
char			*process_escapes(char *input, char quote);

t_token_type	get_token_type(char c, char next_c);

#endif