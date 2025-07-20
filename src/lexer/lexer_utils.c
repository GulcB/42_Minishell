/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:24:07 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/20 20:27:19 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	lexer_free(t_lexer *lexer)
{
	if (!lexer)
		return ;
}

t_token	*handle_pipe_tokens(t_lexer *lexer, int start_pos)
{
	t_token	*token;

	if (lexer_peek_char(lexer) == '|')
	{
		token = token_create(lexer->gc, TOKEN_OR, "||", start_pos);
		lexer_read_char(lexer);
		lexer_read_char(lexer);
	}
	else
	{
		token = token_create(lexer->gc, TOKEN_PIPE, "|", start_pos);
		lexer_read_char(lexer);
	}
	return (token);
}

t_token	*handle_quote_tokens(t_lexer *lexer, int start_pos)
{
	t_token	*token;
	char	*value;

	if (lexer->current_char == '"')
	{
		value = read_double_quoted_string(lexer);
		if (!value)
			return (token_create(lexer->gc, TOKEN_ERROR,
					"Unclosed double quote", start_pos));
		token = token_create(lexer->gc, TOKEN_DQUOTE, value, start_pos);
		gc_free(lexer->gc, value);
	}
	else
	{
		value = read_single_quoted_string(lexer);
		if (!value)
			return (token_create(lexer->gc, TOKEN_ERROR,
					"Unclosed single quote", start_pos));
		token = token_create(lexer->gc, TOKEN_SQUOTE, value, start_pos);
		gc_free(lexer->gc, value);
	}
	return (token);
}

t_token	*handle_special_chars(t_lexer *lexer, int start_pos)
{
	t_token	*token;

	if (lexer->current_char == '&' && lexer_peek_char(lexer) == '&')
	{
		token = token_create(lexer->gc, TOKEN_AND, "&&", start_pos);
		lexer_read_char(lexer);
		lexer_read_char(lexer);
	}
	else if (lexer->current_char == ';')
	{
		token = token_create(lexer->gc, TOKEN_SEMICOLON, ";", start_pos);
		lexer_read_char(lexer);
	}
	else if (lexer->current_char == '$')
	{
		return (handle_variable_token(lexer, start_pos));
	}
	else
	{
		token = token_create(lexer->gc, TOKEN_DOLLAR, "$", start_pos);
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
		return (token_create(lexer->gc, TOKEN_EOF, "", lexer->position));
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

t_token	*handle_variable_token(t_lexer *lexer, int start_pos)
{
	char	*var_name;
	int		consumed;
	t_token	*token;

	var_name = extract_var_name(&lexer->input[lexer->position], &consumed);
	if (!var_name)
	{
		lexer_read_char(lexer);
		return (token_create(lexer->gc, TOKEN_WORD, "$", start_pos));
	}
	token = token_create(lexer->gc, TOKEN_DOLLAR, var_name, start_pos);
	lexer->position += consumed;
	if (lexer->position < (int)ft_strlen(lexer->input))
		lexer->current_char = lexer->input[lexer->position];
	else
		lexer->current_char = '\0';
	lexer->read_position = lexer->position + 1;
	free(var_name);
	return (token);
}
