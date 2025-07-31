/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_parsing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:20:52 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/31 17:48:39 by gbodur           ###   ########.fr       */
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

char	*create_word_from_position(struct s_exec_context *ctx, t_lexer *lexer, int start_pos)
{
	int		len;
	char	*word;

	len = lexer->position - start_pos;
	if (len == 0)
		return (NULL);
	word = gc_substr(ctx->gc, lexer->input, start_pos, len);
	return (word);
}

int	count_backslashes(t_lexer *lexer, int *consumed)
{
	int count = 0;
	int pos = lexer->position;
	int len = ft_strlen(lexer->input);
	
	*consumed = 0;
	while (pos < len && lexer->input[pos] == '\\')
	{
		count++;
		pos++;
		(*consumed)++;
	}
	return count;
}

char	*read_word(t_lexer *lexer)
{
	char	*buffer;
	int		buf_index;
	int		input_len;
	int		in_escape;
	int		backslash_consumed;

	if (!lexer || !lexer->input)
		return (NULL);
	input_len = ft_strlen(lexer->input);
	buffer = gc_malloc(lexer->gc, input_len * 2 + 1);
	if (!buffer)
		return (NULL);
	buf_index = 0;
	in_escape = 0;
	
	while (should_continue_word(lexer, &in_escape))
	{
		if (lexer->current_char == '\\')
		{
			int backslash_count = count_backslashes(lexer, &backslash_consumed);
			for (int i = 0; i < backslash_consumed; i++)
				lexer_read_char(lexer);
			if (backslash_count % 2 == 1 && lexer->current_char == '$')
			{
				for (int i = 0; i < backslash_count / 2; i++)
					buffer[buf_index++] = '\\';
				buffer[buf_index++] = '\x01';
				buffer[buf_index++] = '$';
				lexer_read_char(lexer);
			}
			else if (backslash_count % 2 == 1)
			{
				for (int i = 0; i < backslash_count / 2; i++)
					buffer[buf_index++] = '\\';
				buffer[buf_index++] = lexer->current_char;
				lexer_read_char(lexer);
			}
			else
			{
				for (int i = 0; i < backslash_count / 2; i++)
					buffer[buf_index++] = '\\';
			}
		}
		else
		{
			buffer[buf_index++] = lexer->current_char;
			lexer_read_char(lexer);
		}
	}
	
	buffer[buf_index] = '\0';
	if (buf_index == 0)
	{
		return (NULL);
	}
	return (buffer);
}
