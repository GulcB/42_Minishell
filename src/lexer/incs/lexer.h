/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 01:00:49 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/31 14:54:23 by gbodur           ###   ########.fr       */
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
void			lexer_read_char(t_lexer *lexer);
char			lexer_peek_char(t_lexer *lexer);
t_token			*lexer_tokenize_with_context(char *input, t_gc *gc);

void			lexer_free(t_lexer *lexer);
t_token			*handle_pipe_tokens(t_lexer *lexer, int start_pos);
t_token			*handle_quote_tokens(t_lexer *lexer, int start_pos);
int				has_matching_quote(t_lexer *lexer, char quote_char);
t_token			*handle_special_chars(t_gc *gc, t_lexer *lexer, int start_pos);
t_token			*lexer_next_token(t_gc *gc, t_lexer *lexer);
t_token			*handle_variable_token(t_gc *gc, t_lexer *lexer, int start_pos);

t_token			*handle_word_token(t_lexer *lexer, int start_pos);
int				validate_and_add_token(t_gc *gc, t_token **token_list,
					t_token *current_token, t_token **prev_token);
int				should_stop_tokenizing(t_token *current_token);

t_token			*handle_input_redirect(t_lexer *lexer, int start_pos,
					char next);
t_token			*handle_output_redirect(t_lexer *lexer, int start_pos,
					char next);
char			*process_heredoc_delimiter(t_lexer *lexer);
t_token			*create_heredoc_token(t_lexer *lexer, int start_pos);
t_token			*handle_redirect_tokens(t_lexer *lexer, int start_pos);

int				validate_word_sequence(t_token *prev, t_token *current);
int				validate_redirect_sequence(t_token *prev, t_token *current);
int				is_valid_token_sequence(t_token *prev, t_token *current);

int				is_heredoc_delimiter_char(char c);
char			*read_heredoc_delimiter(t_lexer *lexer);

int				handle_escape_in_double_quote(t_lexer *lexer, char *buffer,
					int *buf_index);
char			*process_double_quote_content(t_lexer *lexer, char *buffer);
char			*read_double_quoted_string(t_lexer *lexer);
char			*read_single_quoted_string(t_lexer *lexer);
char			*read_unclosed_quote_as_word(t_lexer *lexer);

int				ft_isspace(int c);
void			skip_whitespace(t_lexer *lexer);
int				is_word_delimiter(char c);

int				handle_escape_in_word(t_lexer *lexer);
int				process_escape_sequence(t_lexer *lexer, int *in_escape);
int				should_continue_word(t_lexer *lexer, int *in_escape);
char			*create_word_from_position(struct s_exec_context *ctx, t_lexer *lexer, int start_pos);
char			*read_word(t_lexer *lexer);

char			*search_env_var(const char *var_name,
					struct s_exec_context *ctx);
char			*get_env_value(const char *var_name,
					struct s_exec_context *ctx);
char			*add_literal_part(struct s_exec_context *ctx, char *result, const char *str, int start, int end);

char			*expand_variables(const char *str, struct s_exec_context *ctx);
char			*process_token_expansion(t_token *token,
					struct s_exec_context *ctx);

char			*extract_simple_var(t_gc *gc, const char *str, int *consumed);
char			*extract_braced_var(t_gc *gc, const char *str, int *consumed);
char			*extract_special_var(t_gc *gc, const char *str, int *consumed);
char			*extract_var_name(t_gc *gc, const char *str, int *consumed);
char			*get_special_var_value(const char *var_name,
					struct s_exec_context *ctx);

/* Tilde expansion */
char			*expand_tilde(const char *str, struct s_exec_context *ctx);

#endif
