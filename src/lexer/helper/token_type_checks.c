/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_checks.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:37:33 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/19 11:41:07 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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
