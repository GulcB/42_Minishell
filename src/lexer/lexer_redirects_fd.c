/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redirects_fd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:35:00 by mdivan            #+#    #+#             */
/*   Updated: 2025/08/04 19:40:52 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static char	*create_fd_redirect_string(t_lexer *lexer, char fd_num,
		char redir_char, int append)
{
	char	*redirect_str;
	int		len;

	if (append)
		len = 4;
	else
		len = 3;
	redirect_str = gc_malloc(lexer->gc, len);
	redirect_str[0] = fd_num;
	redirect_str[1] = redir_char;
	if (append)
	{
		redirect_str[2] = redir_char;
		redirect_str[3] = '\0';
	}
	else
		redirect_str[2] = '\0';
	return (redirect_str);
}

static t_token	*handle_fd_append_redirect(t_lexer *lexer, int start_pos,
		char fd_num)
{
	char	*redirect_str;
	char	fourth_char;

	fourth_char = '\0';
	if (lexer->read_position + 2 < (int)ft_strlen(lexer->input))
		fourth_char = lexer->input[lexer->read_position + 2];
	if (fourth_char == '>')
		return (token_create(lexer->gc, TOKEN_ERROR,
				"syntax error near unexpected token `>'", start_pos));
	redirect_str = create_fd_redirect_string(lexer, fd_num, '>', 1);
	lexer_read_char(lexer);
	lexer_read_char(lexer);
	lexer_read_char(lexer);
	return (token_create(lexer->gc, TOKEN_APPEND, redirect_str, start_pos));
}

static t_token	*handle_fd_heredoc_redirect(t_lexer *lexer, int start_pos,
		char fd_num)
{
	char	*redirect_str;
	char	fourth_char;

	fourth_char = '\0';
	if (lexer->read_position + 2 < (int)ft_strlen(lexer->input))
		fourth_char = lexer->input[lexer->read_position + 2];
	if (fourth_char == '<')
		return (token_create(lexer->gc, TOKEN_ERROR,
				"syntax error near unexpected token `<'", start_pos));
	redirect_str = create_fd_redirect_string(lexer, fd_num, '<', 1);
	lexer_read_char(lexer);
	lexer_read_char(lexer);
	lexer_read_char(lexer);
	return (token_create(lexer->gc, TOKEN_HEREDOC, redirect_str, start_pos));
}

static t_token	*handle_fd_single_redirect(t_lexer *lexer, int start_pos,
		char fd_num, char redir_char)
{
	char			*redirect_str;
	t_token_type	token_type;

	redirect_str = create_fd_redirect_string(lexer, fd_num, redir_char, 0);
	lexer_read_char(lexer);
	lexer_read_char(lexer);
	if (redir_char == '>')
		token_type = TOKEN_REDIR_OUT;
	else
		token_type = TOKEN_REDIR_IN;
	return (token_create(lexer->gc, token_type, redirect_str, start_pos));
}

t_token	*handle_fd_redirect(t_lexer *lexer, int start_pos)
{
	char	fd_num;
	char	redir_char;
	char	third_char;

	fd_num = lexer->current_char;
	redir_char = lexer_peek_char(lexer);
	third_char = '\0';
	if (lexer->read_position + 1 < (int)ft_strlen(lexer->input))
		third_char = lexer->input[lexer->read_position + 1];
	if (ft_isdigit(third_char))
	{
		if (fd_num == '<')
			return (handle_input_redirect(lexer, start_pos, redir_char));
		else
			return (handle_output_redirect(lexer, start_pos, redir_char));
	}
	if (redir_char == '>' && third_char == '>')
		return (handle_fd_append_redirect(lexer, start_pos, fd_num));
	if (redir_char == '<' && third_char == '<')
		return (handle_fd_heredoc_redirect(lexer, start_pos, fd_num));
	return (handle_fd_single_redirect(lexer, start_pos, fd_num, redir_char));
}
