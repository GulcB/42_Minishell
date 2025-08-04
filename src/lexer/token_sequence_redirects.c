/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_sequence_redirects.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:40:00 by mdivan            #+#    #+#             */
/*   Updated: 2025/08/04 19:47:04 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"

static void	write_redirect_error_symbol(t_token_type type)
{
	if (type == TOKEN_REDIR_OUT || type == TOKEN_APPEND)
		write(2, ">'", 2);
	else if (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC)
		write(2, "<'", 2);
	else if (type == TOKEN_EOF)
		write(2, "newline'", 8);
	else
		write(2, "unexpected token'", 17);
}

static int	handle_consecutive_redirects(t_token *prev, t_token *current)
{
	if (prev && is_redirect_token(prev) && is_redirect_token(current))
	{
		write(2, "syntax error near unexpected token `", 36);
		if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_APPEND)
			write(2, ">'", 2);
		else if (current->type == TOKEN_REDIR_IN
			|| current->type == TOKEN_HEREDOC)
			write(2, "<'", 2);
		else
			write(2, "newline'", 8);
		write(2, "\n", 1);
		return (0);
	}
	return (1);
}

static int	is_valid_redirect_target(t_token_type type)
{
	if (type == TOKEN_WORD || type == TOKEN_DQUOTE || type == TOKEN_SQUOTE)
		return (1);
	return (0);
}

static int	handle_redirect_target(t_token *prev, t_token *current)
{
	if (prev && (prev->type == TOKEN_REDIR_IN || prev->type == TOKEN_REDIR_OUT
			|| prev->type == TOKEN_APPEND || prev->type == TOKEN_HEREDOC))
	{
		if (!is_valid_redirect_target(current->type))
		{
			write(2, "syntax error near unexpected token `", 36);
			write_redirect_error_symbol(current->type);
			write(2, "\n", 1);
			return (0);
		}
	}
	return (1);
}

int	validate_redirect_sequence(t_token *prev, t_token *current)
{
	if (!current)
		return (1);
	if (!handle_consecutive_redirects(prev, current))
		return (0);
	if (!handle_redirect_target(prev, current))
		return (0);
	return (1);
}
