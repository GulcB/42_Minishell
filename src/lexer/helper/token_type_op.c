/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_op.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 10:32:18 by mdivan            #+#    #+#             */
/*   Updated: 2025/07/14 23:55:05 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/lexer.h"

static t_token_type	check_pipe(char c, char next_c)
{
	if (c == '|')
	{
		if (next_c == '|')
			return (TOKEN_OR);
		return (TOKEN_PIPE);
	}
	return (TOKEN_WORD);
}

static t_token_type	check_redirection(char c, char next_c)
{
	if (c == '<')
	{
		if (next_c == '<')
			return (TOKEN_HEREDOC);
		return (TOKEN_REDIR_IN);
	}
	if (c == '>')
	{
		if (next_c == '>')
			return (TOKEN_APPEND);
		return (TOKEN_REDIR_OUT);
	}
	return (TOKEN_WORD);
}

t_token_type	get_token_type(char c, char next_c)
{
	t_token_type	type;

	type = check_pipe(c, next_c);
	if (type != TOKEN_WORD)
		return (type);
	type = check_redirection(c, next_c);
	if (type != TOKEN_WORD)
		return (type);
	if (c == '&' && next_c == '&')
		return (TOKEN_AND);
	if (c == ';')
		return (TOKEN_SEMICOLON);
	if (c == '"')
		return (TOKEN_DQUOTE);
	if (c == '\'')
		return (TOKEN_SQUOTE);
	if (c == '$')
		return (TOKEN_DOLLAR);
	return (TOKEN_WORD);
}

int	is_quote_special_char(char c, int in_double_quote)
{
	if (in_double_quote)
		return (c == '"' || c == '\\' || c == '$' || c == '`');
	else
		return (c == '\'');
}

int	is_heredoc_start(char *input, int position)
{
	if (position + 1 < (int)ft_strlen(input))
		return (input[position] == '<' && input[position + 1] == '<');
	return (0);
}

int	find_matching_quote(char *input, int start, char quote_char)
{
	int	i;
	int	in_escape;

	i = start + 1;
	in_escape = 0;
	
	while (input[i])
	{
		if (in_escape)
		{
			in_escape = 0;
			i++;
			continue ;
		}
		if (input[i] == '\\' && quote_char == '"')
		{
			in_escape = 1;
			i++;
			continue ;
		}
		if (input[i] == quote_char)
			return (i);
		i++;
	}
	return (-1);
}

static int	validate_word_token(char *token_value)
{
	if (!token_value)
		return (0);
	return (ft_strlen(token_value) > 0);
}

static int	validate_quote_tokens(t_token_type type)
{
	if (type == TOKEN_DQUOTE)
		return (1);
	if (type == TOKEN_SQUOTE)
		return (1);
	return (0);
}

static int	validate_heredoc_token(char *token_value)
{
	if (!token_value)
		return (0);
	return (ft_strlen(token_value) > 0);
}

int	is_valid_token_syntax(char *token_value, t_token_type type)
{
	if (!token_value)
		return (0);
	if (type == TOKEN_WORD)
		return (validate_word_token(token_value));
	if (validate_quote_tokens(type))
		return (1);
	if (type == TOKEN_HEREDOC)
		return (validate_heredoc_token(token_value));
	return (1);
}
