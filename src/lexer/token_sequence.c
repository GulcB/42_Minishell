/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:47:27 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/20 16:10:59 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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

int	validate_redirect_sequence(t_token *prev, t_token *current)
{
	if (!current)
		return (1);
	if (prev && (prev->type == TOKEN_REDIR_IN || prev->type == TOKEN_REDIR_OUT
			|| prev->type == TOKEN_APPEND || prev->type == TOKEN_HEREDOC))
	{
		if (current->type != TOKEN_WORD && current->type != TOKEN_DQUOTE
			&& current->type != TOKEN_SQUOTE)
			return (0);
	}
	return (1);
}

int	is_valid_token_sequence(t_token *prev, t_token *current)
{
	if (!current)
		return (1);
	if (!validate_word_sequence(prev, current))
		return (0);
	if (!validate_redirect_sequence(prev, current))
		return (0);
	return (1);
}

