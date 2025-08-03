/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:47:27 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/30 14:16:57 by mdivan           ###   ########.fr       */
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

int	validate_semicolon_sequence(t_token *prev, t_token *current)
{
	if (!current)
		return (1);
	// Check if semicolon appears at the beginning or after certain tokens
	if (current->type == TOKEN_SEMICOLON)
	{
		// Semicolon cannot be at the beginning of input
		if (!prev)
		{
			write(2, "bash: syntax error near unexpected token `;'\n", 46);
			return (0);
		}
		// Semicolon cannot follow another semicolon, pipe, or logical operators
		if (prev->type == TOKEN_SEMICOLON || prev->type == TOKEN_PIPE 
			|| prev->type == TOKEN_AND || prev->type == TOKEN_OR
			|| prev->type == TOKEN_REDIR_IN || prev->type == TOKEN_REDIR_OUT
			|| prev->type == TOKEN_APPEND || prev->type == TOKEN_HEREDOC)
		{
			write(2, "bash: syntax error near unexpected token `;'\n", 46);
			return (0);
		}
	}
	return (1);
}

int	validate_redirect_sequence(t_token *prev, t_token *current)
{
	if (!current)
		return (1);
	// Check for consecutive redirect tokens (like >> followed by >>)
	if (prev && is_redirect_token(prev) && is_redirect_token(current))
	{
		write(2, "syntax error near unexpected token `", 36);
		if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_APPEND)
			write(2, ">'", 2);
		else if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
			write(2, "<'", 2);
		else
			write(2, "newline'", 8);
		write(2, "\n", 1);
		return (0);
	}
	if (prev && (prev->type == TOKEN_REDIR_IN || prev->type == TOKEN_REDIR_OUT
			|| prev->type == TOKEN_APPEND || prev->type == TOKEN_HEREDOC))
	{
		if (current->type != TOKEN_WORD && current->type != TOKEN_DQUOTE
			&& current->type != TOKEN_SQUOTE)
		{
			write(2, "syntax error near unexpected token `", 36);
			if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_APPEND)
				write(2, ">'", 2);
			else if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
				write(2, "<'", 2);
			else if (current->type == TOKEN_EOF)
				write(2, "newline'", 8);
			else
				write(2, "unexpected token'", 17);
			write(2, "\n", 1);
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
