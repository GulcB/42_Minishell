/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_escape.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 15:00:00 by mdivan            #+#    #+#             */
/*   Updated: 2025/08/04 20:05:13 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "executor.h"

int	handle_escape_in_word(t_lexer *lexer)
{
	lexer_read_char(lexer);
	if (lexer->current_char == '\0')
		return (0);
	lexer_read_char(lexer);
	return (1);
}

int	process_escape_sequence(t_lexer *lexer, int *in_escape)
{
	if (*in_escape)
	{
		if (!handle_escape_in_word(lexer))
			return (0);
		*in_escape = 0;
		return (1);
	}
	if (lexer->current_char == '\\')
	{
		*in_escape = 1;
		return (1);
	}
	return (-1);
}

int	count_backslashes(t_lexer *lexer, int *consumed)
{
	int	count;
	int	pos;
	int	len;

	count = 0;
	pos = lexer->position;
	len = ft_strlen(lexer->input);
	*consumed = 0;
	while (pos < len && lexer->input[pos] == '\\')
	{
		count++;
		pos++;
		(*consumed)++;
	}
	return (count);
}

void	add_backslashes_to_buffer(char *buffer, int *buf_index, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		buffer[(*buf_index)++] = '\\';
		i++;
	}
}

void	advance_lexer_by_count(t_lexer *lexer, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		lexer_read_char(lexer);
		i++;
	}
}
