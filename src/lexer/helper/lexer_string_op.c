/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_string_op.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 09:56:34 by mdivan            #+#    #+#             */
/*   Updated: 2025/04/27 14:21:56 by mdivan           ###   ########.fr       */
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

char *read_string(t_lexer *lexer, char quote)
{
    char    *buffer;
    int     buf_index;

    buffer = malloc(ft_strlen(lexer->input) + 1); // Yeterince büyük alan
    if (!buffer)
        return (NULL);
    buf_index = 0;
    while (lexer->current_char != '\0' && lexer->current_char != quote)
    {
        if (lexer->current_char == '\\')
        {
            lexer_read_char(lexer); // '\' karakterini atla
            if (lexer->current_char == '\0')
                break;
            if (lexer->current_char == '\'')
            {
                // Eğer \' varsa --> \ ve ' ekle
                buffer[buf_index++] = '\\';
                buffer[buf_index++] = '\'';
            }
            else
            {
                // Eğer \" varsa --> sadece " ekle
                buffer[buf_index++] = lexer->current_char;
            }
        }
        else
        {
            buffer[buf_index++] = lexer->current_char;
        }
        lexer_read_char(lexer);
    }
    buffer[buf_index] = '\0'; // null terminate
    if (lexer->current_char == quote)
        lexer_read_char(lexer);
    return (buffer);
}




// char	*read_string(t_lexer *lexer, char quote)
// {
// 	int		start_pos;
// 	int		len;
// 	char	*string;

// 	start_pos = lexer->position;
// 	while (lexer->current_char != '\0' && lexer->current_char != quote)
// 	{
// 		if (lexer->current_char == '\\')
// 		{
// 			printf("Kaçış karakteri bulundu: '%c'\n", lexer->current_char);
// 			lexer_read_char(lexer);
// 			if (lexer->current_char == '\0')
// 				break ;
// 		}
// 		lexer_read_char(lexer);
// 	}
// 	len = lexer->position - start_pos;
// 	string = ft_substr(lexer->input, start_pos, len);
// 	if (lexer->current_char == quote)
// 		lexer_read_char(lexer);
// 	return (string);
// }

// char	*read_string(t_lexer *lexer, char quote)
//{
//	int		start_pos;
//	int		len;
//	char	*string;
//	unsigned char quote_val = (unsigned char)quote;
//
//	start_pos = lexer->position;
//	while (lexer->current_char != '\0')
//	{
//		// Kaçış karakteri (\) kontrolü
//		if (lexer->current_char == '\\')
//		{
//			// Kaçış karakterini oku
//			lexer_read_char(lexer);
//
//			// Eğer bu kaçış dizisi tırnak içinse (\" veya \') ve tırnak tipimizle eşleşiyorsa
//			if (lexer->current_char == quote)
//			{
//				// Bu durumda sadece karakteri okuyoruz ama eşleşme olarak değerlendirmiyoruz
//				lexer_read_char(lexer);
//				continue ;
//			}
//
//			// Dosya sonu kontrolü
//			if (lexer->current_char == '\0')
//				break ;
//
//			// Diğer kaçış dizileri için karakteri oku ve devam et
//			lexer_read_char(lexer);
//			continue ;
//		}
//
//		// Eşleşen tırnak kontrolü (kaçış dizisi olmayan)
//		if ((unsigned char)lexer->current_char == quote_val)
//		{
//			printf("TIRMAK EŞLEŞMESI BULUNDU: '%c'\n", lexer->current_char);
//			break ;
//		}
//
//		// Debug bilgisi
//		printf("current_char: '%c' (%d | 0x%02X)\n",
//			lexer->current_char, (int)lexer->current_char,
//(unsigned char)lexer->current_char);
//		printf("Quote: '%c' (%d | 0x%02X)\n",
//			quote, (int)quote, (unsigned char)quote);
//
//		// Sonraki karaktere geç
//		lexer_read_char(lexer);
//	}
//
//	len = lexer->position - start_pos;
//	string = ft_substr(lexer->input, start_pos, len);
//
//	// Kapatan tırnak karakterini atla
//	if ((unsigned char)lexer->current_char == quote_val)
//	{
//		printf("Kapatan tırnak okunuyor: '%c'\n",
// lexer->current_char);
//		lexer_read_char(lexer);
//	}
//
//	return (string);
//}

char	*read_word(t_lexer *lexer)
{
	int		start_pos;
	int		len;
	char	*word;

	// int		in_escape;
	// in_escape = 0;
	start_pos = lexer->position;
	while (lexer->current_char)
	{
		// if (in_escape)
		//{
		//	lexer_read_char(lexer);
		//	in_escape = 0;
		//	continue ;
		//}
		// if (lexer->current_char == '\\')
		//{
		//	in_escape = 1;
		//	lexer_read_char(lexer);
		//	continue ;
		//}
		if (ft_isspace(lexer->current_char) || lexer->current_char == '|'
			|| lexer->current_char == '<' || lexer->current_char == '>'
			|| lexer->current_char == ';' || lexer->current_char == '&'
			|| lexer->current_char == '\'' || lexer->current_char == '"'
			|| lexer->current_char == '$')
		{
			break ;
		}
		lexer_read_char(lexer);
	}
	len = lexer->position - start_pos;
	word = ft_substr(lexer->input, start_pos, len);
	return (word);
}
