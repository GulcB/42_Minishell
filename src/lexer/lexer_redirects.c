/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:29:56 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/20 16:27:11 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*handle_input_redirect(t_lexer *lexer, int start_pos, char next)
{
	t_token	*token;

	if (next == '<')
	{
		token = token_create(lexer->gc, TOKEN_HEREDOC, "<<", start_pos);
		lexer_read_char(lexer);
		lexer_read_char(lexer);
	}
	else
	{
		token = token_create(lexer->gc, TOKEN_REDIR_IN, "<", start_pos);
		lexer_read_char(lexer);
	}
	return (token);
}

t_token	*handle_output_redirect(t_lexer *lexer, int start_pos, char next)
{
	t_token	*token;

	if (next == '>')
	{
		token = token_create(lexer->gc, TOKEN_APPEND, ">>", start_pos);
		lexer_read_char(lexer);
		lexer_read_char(lexer);
	}
	else
	{
		token = token_create(lexer->gc, TOKEN_REDIR_OUT, ">", start_pos);
		lexer_read_char(lexer);
	}
	return (token);
}

char	*process_heredoc_delimiter(t_lexer *lexer)
{
	char	*delimiter;

	skip_whitespace(lexer);
	delimiter = read_heredoc_delimiter(lexer);
	if (!delimiter || ft_strlen(delimiter) == 0)
	{
		if (delimiter)
			gc_free(lexer->gc, delimiter);
		return (NULL);
	}
	return (delimiter);
}

t_token	*create_heredoc_token(t_lexer *lexer, int start_pos)
{
	char		*delimiter;
	t_token		*token;

	lexer_read_char(lexer);
	lexer_read_char(lexer);
	delimiter = process_heredoc_delimiter(lexer);
	if (!delimiter)
		return (token_create(lexer->gc, TOKEN_ERROR,
				"Invalid heredoc delimiter", start_pos));
	token = token_create(lexer->gc, TOKEN_HEREDOC, delimiter, start_pos);
	gc_free(lexer->gc, delimiter);
	return (token);
}

t_token	*handle_redirect_tokens(t_lexer *lexer, int start_pos)
{
	char	current;
	char	next;

	current = lexer->current_char;
	next = lexer_peek_char(lexer);
	if (current == '<')
		return (handle_input_redirect(lexer, start_pos, next));
	else
		return (handle_output_redirect(lexer, start_pos, next));
}
