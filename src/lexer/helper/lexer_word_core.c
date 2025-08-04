/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_core.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 15:00:00 by mdivan            #+#    #+#             */
/*   Updated: 2025/08/04 20:05:13 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "executor.h"

int	should_continue_word(t_lexer *lexer, int *in_escape)
{
	if (!lexer->current_char)
		return (0);
	if (*in_escape)
		return (1);
	if (lexer->current_char == '\\')
		return (1);
	if (lexer->current_char == '"' || lexer->current_char == '\'')
	{
		if (!has_matching_quote(lexer, lexer->current_char))
			return (1);
	}
	if (is_word_delimiter(lexer->current_char))
		return (0);
	return (1);
}

char	*create_word_from_position(struct s_exec_context *ctx, t_lexer *lexer,
		int start_pos)
{
	int		len;
	char	*word;

	len = lexer->position - start_pos;
	if (len == 0)
		return (NULL);
	word = gc_substr(ctx->gc, lexer->input, start_pos, len);
	return (word);
}

static char	*initialize_word_buffer(t_lexer *lexer, int *buf_index,
	int *in_escape)
{
	char	*buffer;
	int		input_len;

	if (!lexer || !lexer->input)
		return (NULL);
	input_len = ft_strlen(lexer->input);
	buffer = gc_malloc(lexer->gc, input_len * 2 + 1);
	if (!buffer)
		return (NULL);
	*buf_index = 0;
	*in_escape = 0;
	return (buffer);
}

static void	process_word_character(t_lexer *lexer, char *buffer, int *buf_index)
{
	if (lexer->current_char == '\\')
		handle_backslash_sequence(lexer, buffer, buf_index);
	else
	{
		buffer[(*buf_index)++] = lexer->current_char;
		lexer_read_char(lexer);
	}
}

char	*read_word(t_lexer *lexer)
{
	char	*buffer;
	int		buf_index;
	int		in_escape;

	buffer = initialize_word_buffer(lexer, &buf_index, &in_escape);
	if (!buffer)
		return (NULL);
	while (should_continue_word(lexer, &in_escape))
		process_word_character(lexer, buffer, &buf_index);
	buffer[buf_index] = '\0';
	if (buf_index == 0)
		return (NULL);
	return (buffer);
}
