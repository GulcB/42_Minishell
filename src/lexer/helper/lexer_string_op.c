/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_string_op.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 09:56:34 by mdivan            #+#    #+#             */
/*   Updated: 2025/04/28 09:44:17 by mdivan           ###   ########.fr       */
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
	{
		lexer_read_char(lexer);
	}
}

char	*read_single_quoted_string(t_lexer *lexer)
{
	char	*buffer;
	int		buf_index;
	int		input_len;

	if (!lexer || !lexer->input)
		return (NULL);
	input_len = ft_strlen(lexer->input);
	buffer = malloc(input_len + 1);
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
		free(buffer);
		return (NULL);
	}
	lexer_read_char(lexer);
	buffer[buf_index] = '\0';
	return (buffer);
}

char	*read_double_quoted_string(t_lexer *lexer)
{
	char	*buffer;
	int		buf_index;
	int		input_len;

	if (!lexer || !lexer->input)
		return (NULL);
	input_len = ft_strlen(lexer->input);
	buffer = malloc(input_len + 1);
	if (!buffer)
		return (NULL);
	buf_index = 0;
	lexer_read_char(lexer);
	while (lexer->current_char != '\0' && lexer->current_char != '"')
	{
		if (lexer->current_char == '\\')
		{
			lexer_read_char(lexer);
			if (lexer->current_char == '\0')
				break ;
			if (lexer->current_char == '"' || lexer->current_char == '\\'
				|| lexer->current_char == '$')
			{
				buffer[buf_index++] = lexer->current_char;
			}
			else
			{
				buffer[buf_index++] = '\\';
				buffer[buf_index++] = lexer->current_char;
			}
		}
		else
		{
			buffer[buf_index++] = lexer->current_char;
		}
		lexer_read_char(lexer);
	}
	if (lexer->current_char == '\0')
	{
		free(buffer);
		return (NULL);
	}
	lexer_read_char(lexer);
	buffer[buf_index] = '\0';
	return (buffer);
}

// char	*read_word(t_lexer *lexer)
// {
// 	int		start_pos;
// 	int		len;
// 	char	*word;

// 	start_pos = lexer->position;
// 	while (lexer->current_char)
// 	{
// 		// if (in_escape)
// 		//{
// 		//	lexer_read_char(lexer);
// 		//	in_escape = 0;
// 		//	continue ;
// 		//}
// 		// if (lexer->current_char == '\\')
// 		//{
// 		//	in_escape = 1;
// 		//	lexer_read_char(lexer);
// 		//	continue ;
// 		//}
// 		if (ft_isspace(lexer->current_char) || lexer->current_char == '|'
// 			|| lexer->current_char == '<' || lexer->current_char == '>'
// 			|| lexer->current_char == ';' || lexer->current_char == '&'
// 			|| lexer->current_char == '\'' || lexer->current_char == '"'
// 			|| lexer->current_char == '$')
// 		{
// 			break ;
// 		}

// 		lexer_read_char(lexer);
// 	}

// 	len = lexer->position - start_pos;
// 	word = ft_substr(lexer->input, start_pos, len);
// 	return (word);
// }

char	*read_word(t_lexer *lexer)
{
	int		start_pos;
	int		len;
	char	*word;
	int		in_escape;

	start_pos = lexer->position;
	in_escape = 0;
	while (lexer->current_char)
	{
		if (in_escape)
		{
			lexer_read_char(lexer);
			in_escape = 0;
			continue ;
		}
		if (lexer->current_char == '\\')
		{
			in_escape = 1;
			lexer_read_char(lexer);
			if (lexer->current_char == '\0')
				break ;
			continue ;
		}
		if (ft_isspace(lexer->current_char) || lexer->current_char == '|'
			|| lexer->current_char == '<' || lexer->current_char == '>'
			|| lexer->current_char == ';' || lexer->current_char == '&'
			|| lexer->current_char == '\'' || lexer->current_char == '"'
			|| lexer->current_char == '$' || lexer->current_char == '('
			|| lexer->current_char == ')')
		{
			break ;
		}
		lexer_read_char(lexer);
	}
	len = lexer->position - start_pos;
	word = ft_substr(lexer->input, start_pos, len);
	return (word);
}
