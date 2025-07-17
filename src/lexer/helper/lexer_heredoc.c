/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:16:07 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/17 23:22:52 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/lexer.h"

static int	is_heredoc_delimiter_char(char c)
{
	return (!ft_isspace(c) && c != '|' && c != '<' && c != '>' && c != ';'
		&& c != '&' && c != '(' && c != ')');
}

char	*read_heredoc_delimiter(t_lexer *lexer)
{
	char	*delimiter;
	int		start_pos;
	int		len;

	skip_whitespace(lexer);
	start_pos = lexer->position;
	while (lexer->current_char && is_heredoc_delimiter_char(lexer->current_char))
		lexer_read_char(lexer);
	len = lexer->position - start_pos;
	if (len == 0)
		return (NULL);
	delimiter = ft_substr(lexer->input, start_pos, len);
	return (delimiter);
}
