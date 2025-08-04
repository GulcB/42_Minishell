/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_backslash.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 15:00:00 by mdivan            #+#    #+#             */
/*   Updated: 2025/08/04 20:05:13 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "executor.h"

static void	handle_odd_backslash_dollar(t_lexer *lexer, char *buffer,
		int *buf_index, int count)
{
	add_backslashes_to_buffer(buffer, buf_index, count / 2);
	buffer[(*buf_index)++] = '\x01';
	buffer[(*buf_index)++] = '$';
	lexer_read_char(lexer);
}

static void	handle_odd_backslash_other(t_lexer *lexer, char *buffer,
		int *buf_index, int count)
{
	add_backslashes_to_buffer(buffer, buf_index, count / 2);
	buffer[(*buf_index)++] = lexer->current_char;
	lexer_read_char(lexer);
}

int	handle_backslash_sequence(t_lexer *lexer, char *buffer, int *buf_index)
{
	int	backslash_count;
	int	backslash_consumed;

	backslash_count = count_backslashes(lexer, &backslash_consumed);
	advance_lexer_by_count(lexer, backslash_consumed);
	if (backslash_count % 2 == 1 && lexer->current_char == '$')
		handle_odd_backslash_dollar(lexer, buffer, buf_index, backslash_count);
	else if (backslash_count % 2 == 1)
		handle_odd_backslash_other(lexer, buffer, buf_index, backslash_count);
	else
		add_backslashes_to_buffer(buffer, buf_index, backslash_count / 2);
	return (1);
}
