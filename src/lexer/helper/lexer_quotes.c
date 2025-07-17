/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:14:32 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/17 23:15:40 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/lexer.h"

static int	handle_escape_in_double_quote(t_lexer *lexer, char *buffer,
		int *buf_index)
{
	lexer_read_char(lexer);
	if (lexer->current_char == '\0')
		return (0);
	if (lexer->current_char == '"' || lexer->current_char == '\\'
		|| lexer->current_char == '$' || lexer->current_char == '`'
		|| lexer->current_char == '\n')
	{
		if (lexer->current_char == '\n')
			buffer[(*buf_index)++] = '\n';
		else
			buffer[(*buf_index)++] = lexer->current_char;
	}
	else
	{
		buffer[(*buf_index)++] = '\\';
		buffer[(*buf_index)++] = lexer->current_char;
	}
	return (1);
}

static char	*process_double_quote_content(t_lexer *lexer, char *buffer)
{
	int	buf_index;

	buf_index = 0;
	lexer_read_char(lexer);
	while (lexer->current_char != '\0' && lexer->current_char != '"')
	{
		if (lexer->current_char == '\\')
		{
			if (!handle_escape_in_double_quote(lexer, buffer, &buf_index))
				break ;
		}
		else if (lexer->current_char == '$')
			buffer[buf_index++] = lexer->current_char;
		else
			buffer[buf_index++] = lexer->current_char;
		lexer_read_char(lexer);
	}
	if (lexer->current_char == '\0')
		return (gc_free(buffer), NULL);
	lexer_read_char(lexer);
	buffer[buf_index] = '\0';
	return (buffer);
}

char	*read_double_quoted_string(t_lexer *lexer)
{
	char	*buffer;
	int		input_len;

	if (!lexer || !lexer->input)
		return (NULL);
	input_len = ft_strlen(lexer->input);
	buffer = gc_malloc(input_len + 1);
	if (!buffer)
		return (NULL);
	return (process_double_quote_content(lexer, buffer));
}

char	*read_single_quoted_string(t_lexer *lexer)
{
	char	*buffer;
	int		buf_index;
	int		input_len;

	if (!lexer || !lexer->input)
		return (NULL);
	input_len = ft_strlen(lexer->input);
	buffer = gc_malloc(input_len + 1);
	if (!buffer)
		return (NULL);
	buf_index = 0;
	lexer_read_char(lexer);
	while (lexer->current_char != '\0' && lexer->current_char != '\'')
	{
		buffer[buf_index++] = lexer->current_char;
		lexer_read_char(lexer);
	}
	if (lexer->current_char == '\0')
	{
		gc_free(buffer);
		return (NULL);
	}
	lexer_read_char(lexer);
	buffer[buf_index] = '\0';
	return (buffer);
}
