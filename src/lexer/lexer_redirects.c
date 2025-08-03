/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:29:56 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/03 13:19:14 by gbodur           ###   ########.fr       */
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
		// delimiter will be cleaned up automatically if allocated
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
	// delimiter will be cleaned up automatically
	return (token);
}

t_token	*handle_redirect_tokens(t_lexer *lexer, int start_pos)
{
	char	current;
	char	next;
	char	*redirect_str;

	current = lexer->current_char;
	next = lexer_peek_char(lexer);
	if (ft_isdigit(current) && (next == '<' || next == '>'))
	{
		char fd_num = current;
		char redir_char = next;
		char third_char = '\0';
		
		if (lexer->read_position + 1 < (int)ft_strlen(lexer->input))
			third_char = lexer->input[lexer->read_position + 1];
		if (ft_isdigit(third_char))
		{
			if (current == '<')
				return (handle_input_redirect(lexer, start_pos, next));
			else
				return (handle_output_redirect(lexer, start_pos, next));
		}
		if (redir_char == '>' && third_char == '>')
		{
			redirect_str = gc_malloc(lexer->gc, 4);
			redirect_str[0] = fd_num;
			redirect_str[1] = '>';
			redirect_str[2] = '>';
			redirect_str[3] = '\0';
			lexer_read_char(lexer);
			lexer_read_char(lexer);
			lexer_read_char(lexer);
			return (token_create(lexer->gc, TOKEN_APPEND, redirect_str, start_pos));
		}
		else if (redir_char == '<' && third_char == '<')
		{
			redirect_str = gc_malloc(lexer->gc, 4);
			redirect_str[0] = fd_num;
			redirect_str[1] = '<';
			redirect_str[2] = '<';
			redirect_str[3] = '\0';
			lexer_read_char(lexer);
			lexer_read_char(lexer);
			lexer_read_char(lexer);
			return (token_create(lexer->gc, TOKEN_HEREDOC, redirect_str, start_pos));
		}
		else if (redir_char == '>')
		{
			redirect_str = gc_malloc(lexer->gc, 3);
			redirect_str[0] = fd_num;
			redirect_str[1] = '>';
			redirect_str[2] = '\0';
			lexer_read_char(lexer);
			lexer_read_char(lexer);
			return (token_create(lexer->gc, TOKEN_REDIR_OUT, redirect_str, start_pos));
		}
		else if (redir_char == '<')
		{
			redirect_str = gc_malloc(lexer->gc, 3);
			redirect_str[0] = fd_num;
			redirect_str[1] = '<';
			redirect_str[2] = '\0';
			lexer_read_char(lexer);
			lexer_read_char(lexer);
			return (token_create(lexer->gc, TOKEN_REDIR_IN, redirect_str, start_pos));
		}
	}
	if (current == '<')
		return (handle_input_redirect(lexer, start_pos, next));
	else
		return (handle_output_redirect(lexer, start_pos, next));
}
