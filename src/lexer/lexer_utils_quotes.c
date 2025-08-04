/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:50:00 by mdivan            #+#    #+#             */
/*   Updated: 2025/08/04 19:52:34 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static t_token	*create_double_quote_token(t_lexer *lexer, int start_pos)
{
	char	*value;
	t_token	*token;

	value = read_double_quoted_string(lexer);
	if (!value)
	{
		token = token_create(lexer->gc, TOKEN_ERROR,
				"minishell: syntax error: unexpected end of file", start_pos);
		return (token);
	}
	token = token_create(lexer->gc, TOKEN_DQUOTE, value, start_pos);
	return (token);
}

static t_token	*create_single_quote_token(t_lexer *lexer, int start_pos)
{
	char	*value;
	t_token	*token;

	value = read_single_quoted_string(lexer);
	if (!value)
	{
		token = token_create(lexer->gc, TOKEN_ERROR,
				"minishell: syntax error: unexpected end of file", start_pos);
		return (token);
	}
	token = token_create(lexer->gc, TOKEN_SQUOTE, value, start_pos);
	return (token);
}

t_token	*handle_quote_tokens(t_lexer *lexer, int start_pos)
{
	if (lexer->current_char == '"')
		return (create_double_quote_token(lexer, start_pos));
	else
		return (create_single_quote_token(lexer, start_pos));
}

char	*read_unclosed_quote_as_word(t_lexer *lexer)
{
	char	*buffer;
	int		buf_index;
	int		input_len;

	if (!lexer || !lexer->input)
		return (NULL);
	input_len = ft_strlen(lexer->input);
	buffer = gc_malloc(lexer->gc, input_len + 1);
	if (!buffer)
		return (NULL);
	buf_index = 0;
	lexer_read_char(lexer);
	while (lexer->current_char != '\0' && !ft_isspace(lexer->current_char)
		&& lexer->current_char != '|' && lexer->current_char != '<'
		&& lexer->current_char != '>' && lexer->current_char != ';'
		&& lexer->current_char != '&' && lexer->current_char != '$'
		&& lexer->current_char != '(' && lexer->current_char != ')')
	{
		buffer[buf_index++] = lexer->current_char;
		lexer_read_char(lexer);
	}
	buffer[buf_index] = '\0';
	return (buffer);
}

int	has_matching_quote(t_lexer *lexer, char quote_char)
{
	int	pos;
	int	input_len;

	if (!lexer || !lexer->input)
		return (0);
	input_len = ft_strlen(lexer->input);
	pos = lexer->position + 1;
	while (pos < input_len)
	{
		if (lexer->input[pos] == quote_char)
			return (1);
		pos++;
	}
	return (0);
}
