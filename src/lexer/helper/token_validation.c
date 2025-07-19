/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:37:59 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/19 11:41:22 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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
