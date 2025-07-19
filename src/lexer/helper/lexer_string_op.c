/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_string_op.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 09:56:34 by mdivan            #+#    #+#             */
/*   Updated: 2025/07/19 15:07:11 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/lexer.h"

int	ft_isspace(int c)
{
	c = (unsigned char)c;
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (1);
	return (0);
}

void	skip_whitespace(t_lexer *lexer)
{
	while (ft_isspace(lexer->current_char))
		lexer_read_char(lexer);
}

int	is_word_delimiter(char c)
{
	return (ft_isspace(c) || c == '|' || c == '<' || c == '>' || c == ';'
		|| c == '&' || c == '\'' || c == '"' || c == '$' || c == '('
		|| c == ')');
}
