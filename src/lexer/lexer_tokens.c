/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:23:43 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/19 16:28:56 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static t_token	*handle_pipe_tokens(t_lexer *lexer, int start_pos)
{
	t_token	*token;

	if (lexer_peek_char(lexer) == '|')
	{
		token = token_create(TOKEN_OR, "||", start_pos);
		lexer_read_char(lexer);
		lexer_read_char(lexer);
	}
	else
	{
		token = token_create(TOKEN_PIPE, "|", start_pos);
		lexer_read_char(lexer);
	}
	return (token);
}

static t_token	*handle_quote_tokens(t_lexer *lexer, int start_pos)
{
	t_token	*token;
	char	*value;

	if (lexer->current_char == '"')
	{
		value = read_double_quoted_string(lexer);
		if (!value)
			return (token_create(TOKEN_ERROR,
					"Unclosed double quote", start_pos));
		token = token_create(TOKEN_DQUOTE, value, start_pos);
		gc_free(value);
	}
	else
	{
		value = read_single_quoted_string(lexer);
		if (!value)
			return (token_create(TOKEN_ERROR,
					"Unclosed single quote", start_pos));
		token = token_create(TOKEN_SQUOTE, value, start_pos);
		gc_free(value);
	}
	return (token);
}

static t_token	*handle_special_chars(t_lexer *lexer, int start_pos)
{
	t_token	*token;

	if (lexer->current_char == '&' && lexer_peek_char(lexer) == '&')
	{
		token = token_create(TOKEN_AND, "&&", start_pos);
		lexer_read_char(lexer);
		lexer_read_char(lexer);
	}
	else if (lexer->current_char == ';')
	{
		token = token_create(TOKEN_SEMICOLON, ";", start_pos);
		lexer_read_char(lexer);
	}
	else
	{
		token = token_create(TOKEN_DOLLAR, "$", start_pos);
		lexer_read_char(lexer);
	}
	return (token);
}

t_token	*lexer_next_token(t_lexer *lexer)
{
	t_token_type	type;
	int				start_pos;

	if (!lexer)
		return (NULL);
	skip_whitespace(lexer);
	if (lexer->current_char == '\0')
		return (token_create(TOKEN_EOF, "", lexer->position));
	if (lexer->current_char == '\\')
		lexer_read_char(lexer);
	start_pos = lexer->position;
	type = get_token_type(lexer->current_char, lexer_peek_char(lexer));
	if (type == TOKEN_PIPE || type == TOKEN_OR)
		return (handle_pipe_tokens(lexer, start_pos));
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_HEREDOC || type == TOKEN_APPEND)
		return (handle_redirect_tokens(lexer, start_pos));
	if (type == TOKEN_DQUOTE || type == TOKEN_SQUOTE)
		return (handle_quote_tokens(lexer, start_pos));
	if (type == TOKEN_AND || type == TOKEN_SEMICOLON || type == TOKEN_DOLLAR)
		return (handle_special_chars(lexer, start_pos));
	return (handle_word_token(lexer, start_pos));
}
