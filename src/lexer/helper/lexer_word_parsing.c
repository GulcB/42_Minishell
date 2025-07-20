/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_parsing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:20:52 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/20 16:02:19 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/lexer.h"

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
	int	escape_result;

	if (!lexer->current_char)
		return (0);
	escape_result = process_escape_sequence(lexer, in_escape);
	if (escape_result == 0)
		return (0);
	if (escape_result == 1)
		return (1);
	if (is_word_delimiter(lexer->current_char))
		return (0);
	lexer_read_char(lexer);
	return (1);
}

char	*create_word_from_position(t_lexer *lexer, int start_pos)
{
	int		len;
	char	*word;

	len = lexer->position - start_pos;
	if (len == 0)
		return (NULL);
	word = ft_substr(lexer->input, start_pos, len);
	return (word);
}

char	*read_word(t_lexer *lexer)
{
	int	start_pos;
	int	in_escape;

	start_pos = lexer->position;
	in_escape = 0;
	while (should_continue_word(lexer, &in_escape))
		continue ;
	return (create_word_from_position(lexer, start_pos));
}
