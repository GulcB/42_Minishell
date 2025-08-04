/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_sequence_validation.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:40:00 by mdivan            #+#    #+#             */
/*   Updated: 2025/08/04 19:47:04 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"

int	validate_word_sequence(t_token *prev, t_token *current)
{
	if (!current)
		return (1);
	if (prev && prev->type == TOKEN_PIPE)
	{
		if (current->type == TOKEN_PIPE || current->type == TOKEN_EOF)
			return (0);
	}
	return (1);
}

static int	is_invalid_before_semicolon(t_token_type type)
{
	if (type == TOKEN_SEMICOLON || type == TOKEN_PIPE)
		return (1);
	if (type == TOKEN_AND || type == TOKEN_OR)
		return (1);
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT)
		return (1);
	if (type == TOKEN_APPEND || type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

int	validate_semicolon_sequence(t_token *prev, t_token *current)
{
	if (!current)
		return (1);
	if (current->type == TOKEN_SEMICOLON)
	{
		if (!prev)
		{
			write(2, "bash: syntax error near unexpected token `;'\n", 46);
			return (0);
		}
		if (is_invalid_before_semicolon(prev->type))
		{
			write(2, "bash: syntax error near unexpected token `;'\n", 46);
			return (0);
		}
	}
	return (1);
}

int	is_valid_token_sequence(t_token *prev, t_token *current)
{
	if (!current)
		return (1);
	if (current->type == TOKEN_ERROR)
	{
		write(2, current->value, ft_strlen(current->value));
		return (0);
	}
	if (!validate_word_sequence(prev, current))
		return (0);
	if (!validate_semicolon_sequence(prev, current))
		return (0);
	if (!validate_redirect_sequence(prev, current))
		return (0);
	return (1);
}
