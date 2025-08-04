/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redirects_basic.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:35:00 by mdivan            #+#    #+#             */
/*   Updated: 2025/08/04 19:40:52 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*handle_input_redirect(t_lexer *lexer, int start_pos, char next)
{
	t_token	*token;
	char	third_char;

	if (next == '<')
	{
		third_char = '\0';
		if (lexer->read_position + 1 < (int)ft_strlen(lexer->input))
			third_char = lexer->input[lexer->read_position + 1];
		if (third_char == '<')
		{
			return (token_create(lexer->gc, TOKEN_ERROR,
					"syntax error near unexpected token `<'", start_pos));
		}
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
	char	third_char;

	if (next == '>')
	{
		third_char = '\0';
		if (lexer->read_position + 1 < (int)ft_strlen(lexer->input))
			third_char = lexer->input[lexer->read_position + 1];
		if (third_char == '>')
		{
			return (token_create(lexer->gc, TOKEN_ERROR,
					"minishell: syntax error near unexpected token `>'",
					start_pos));
		}
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
		return (NULL);
	}
	return (delimiter);
}

t_token	*create_heredoc_token(t_lexer *lexer, int start_pos)
{
	char	*delimiter;
	t_token	*token;

	lexer_read_char(lexer);
	lexer_read_char(lexer);
	delimiter = process_heredoc_delimiter(lexer);
	if (!delimiter)
		return (token_create(lexer->gc, TOKEN_ERROR,
				"Invalid heredoc delimiter", start_pos));
	token = token_create(lexer->gc, TOKEN_HEREDOC, delimiter, start_pos);
	return (token);
}

t_token	*handle_redirect_tokens(t_lexer *lexer, int start_pos)
{
	char	current;
	char	next;

	current = lexer->current_char;
	next = lexer_peek_char(lexer);
	if (ft_isdigit(current) && (next == '<' || next == '>'))
		return (handle_fd_redirect(lexer, start_pos));
	if (current == '<')
		return (handle_input_redirect(lexer, start_pos, next));
	else
		return (handle_output_redirect(lexer, start_pos, next));
}
