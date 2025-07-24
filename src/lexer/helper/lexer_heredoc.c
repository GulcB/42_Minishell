/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:16:07 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/24 10:39:42 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/lexer.h"

int	is_heredoc_delimiter_char(char c)
{
	return (!ft_isspace(c) && c != '|' && c != '<' && c != '>' && c != ';'
		&& c != '&' && c != '(' && c != ')');
}

static char	*read_quoted_delimiter(t_lexer *lexer, char quote_char)
{
	char	*delimiter;
	int		start_pos;
	int		len;

	start_pos = lexer->position;
	lexer_read_char(lexer);
	while (lexer->current_char && lexer->current_char != quote_char)
		lexer_read_char(lexer);
	if (lexer->current_char == quote_char)
		lexer_read_char(lexer);
	len = lexer->position - start_pos;
	if (len == 0)
		return (NULL);
	delimiter = ft_substr(lexer->input, start_pos, len);
	return (delimiter);
}

static char	*read_unquoted_delimiter(t_lexer *lexer)
{
	char	*delimiter;
	int		start_pos;
	int		len;

	start_pos = lexer->position;
	while (lexer->current_char
		&& is_heredoc_delimiter_char(lexer->current_char))
		lexer_read_char(lexer);
	len = lexer->position - start_pos;
	if (len == 0)
		return (NULL);
	delimiter = ft_substr(lexer->input, start_pos, len);
	return (delimiter);
}

char	*read_heredoc_delimiter(t_lexer *lexer)
{
	skip_whitespace(lexer);
	if (lexer->current_char == '\'' || lexer->current_char == '"')
		return (read_quoted_delimiter(lexer, lexer->current_char));
	else
		return (read_unquoted_delimiter(lexer));
}
