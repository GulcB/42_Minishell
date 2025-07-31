/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:24:07 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/31 14:46:59 by gbodur           ###   ########.fr       */
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
		{
			token = token_create(lexer->gc, TOKEN_ERROR,
					"minishell: syntax error: unexpected end of file", start_pos);
			return (token);
		}
		token = token_create(lexer->gc, TOKEN_DQUOTE, value, start_pos);
		gc_free(lexer->gc, value);
	}
	else
	{
		value = read_single_quoted_string(lexer);
		if (!value)
		{
			token = token_create(lexer->gc, TOKEN_ERROR,
					"minishell: syntax error: unexpected end of file", start_pos);
			return (token);
		}
		token = token_create(lexer->gc, TOKEN_SQUOTE, value, start_pos);
		gc_free(lexer->gc, value);
	}
	return (token);
}

t_token	*handle_special_chars(t_gc *gc, t_lexer *lexer, int start_pos)
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
		return (handle_variable_token(gc, lexer, start_pos));
	}
	else
	{
		token = token_create(lexer->gc, TOKEN_WORD, "$", start_pos);
		lexer_read_char(lexer);
	}
	return (token);
}

t_token	*lexer_next_token(t_gc *gc, t_lexer *lexer)
{
	t_token_type	type;
	int				start_pos;

	if (!lexer)
		return (NULL);
	skip_whitespace(lexer);
	if (lexer->current_char == '\0')
		return (token_create(lexer->gc, TOKEN_EOF, "", lexer->position));
	start_pos = lexer->position;
	if (ft_isdigit(lexer->current_char))
	{
		char next_char = lexer_peek_char(lexer);
		if (next_char == '>' || next_char == '<')
		{
			char second_next = '\0';
			if (lexer->read_position + 1 < (int)ft_strlen(lexer->input))
				second_next = lexer->input[lexer->read_position + 1];
			if (next_char == '>' && second_next == '>')
				return (handle_redirect_tokens(lexer, start_pos));
			else if (next_char == '<' && second_next == '<')
				return (handle_redirect_tokens(lexer, start_pos));
			else if (!ft_isdigit(second_next))
				return (handle_redirect_tokens(lexer, start_pos));
		}
	}
	type = get_token_type(lexer->current_char, lexer_peek_char(lexer));
	if (type == TOKEN_PIPE || type == TOKEN_OR)
		return (handle_pipe_tokens(lexer, start_pos));
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_HEREDOC || type == TOKEN_APPEND)
		return (handle_redirect_tokens(lexer, start_pos));
	if (type == TOKEN_DQUOTE || type == TOKEN_SQUOTE)
		return (handle_quote_tokens(lexer, start_pos));
	if (type == TOKEN_AND || type == TOKEN_SEMICOLON || type == TOKEN_DOLLAR)
		return (handle_special_chars(gc, lexer, start_pos));
	return (handle_word_token(lexer, start_pos));
}

t_token	*handle_variable_token(t_gc *gc, t_lexer *lexer, int start_pos)
{
	char	*var_name;
	int		consumed;
	t_token	*token;

	var_name = extract_var_name(gc, &lexer->input[lexer->position], &consumed);
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
	int		pos;
	int		input_len;

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
