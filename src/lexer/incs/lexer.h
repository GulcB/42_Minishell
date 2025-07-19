/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 01:00:49 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/19 18:40:20 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "token.h"
# include "libft.h"
# include "garbage_collector.h"
# include <stddef.h>
# include <stdlib.h>

struct	s_exec_context;

typedef struct s_lexer
{
	char		*input;
	int			position;
	int			read_position;
	char		current_char;
	t_gc		*gc;
}				t_lexer;

t_lexer			*lexer_create(char *input, t_gc *gc);
void			lexer_free(t_lexer *lexer);
void			lexer_read_char(t_lexer *lexer);
char			lexer_peek_char(t_lexer *lexer);
t_token			*lexer_tokenize(char *input);
t_token			*lexer_tokenize_with_context(char *input,
					struct s_exec_context *ctx);

t_token			*lexer_next_token(t_lexer *lexer);
t_token			*handle_word_token(t_lexer *lexer, int start_pos);
int				validate_and_add_token(t_token **token_list,
					t_token *current_token, t_token **prev_token);
int				should_stop_tokenizing(t_token *current_token);

int				validate_word_sequence(t_token *prev, t_token *current);
int				validate_redirect_sequence(t_token *prev, t_token *current);
int				is_valid_token_sequence(t_token *prev, t_token *current);

t_token			*create_heredoc_token(t_lexer *lexer, int start_pos);
t_token			*handle_redirect_tokens(t_lexer *lexer, int start_pos);

t_token_type	get_token_type(char c, char next_c);
int				is_quote_special_char(char c, int in_double_quote);
int				is_heredoc_start(char *input, int position);
int				find_matching_quote(char *input, int start, char quote_char);
int				is_valid_token_syntax(char *token_value, t_token_type type);

char			*extract_var_name(const char *str, int *consumed);
char			*get_special_var_value(const char *var_name,
					struct s_exec_context *ctx);
char			*search_env_var(const char *var_name,
					struct s_exec_context *ctx);
char			*get_env_value(const char *var_name,
					struct s_exec_context *ctx);
char			*join_and_free(char *s1, char *s2);
char			*add_literal_part(char *result, const char *str, int start,
					int end);
char			*expand_variables(const char *str, struct s_exec_context *ctx);
char			*process_token_expansion(t_token *token,
					struct s_exec_context *ctx);

char			*read_heredoc_delimiter(t_lexer *lexer);
char			*read_double_quoted_string(t_lexer *lexer);
char			*read_single_quoted_string(t_lexer *lexer);
int				ft_isspace(int c);
void			skip_whitespace(t_lexer *lexer);
int				is_word_delimiter(char c);
char			*read_word(t_lexer *lexer);

char			*extract_special_var(const char *str, int *consumed);
char			*extract_braced_var(const char *str, int *consumed);
char			*extract_simple_var(const char *str, int *consumed);

#endif
