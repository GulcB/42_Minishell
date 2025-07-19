/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 09:27:54 by mdivan            #+#    #+#             */
/*   Updated: 2025/07/19 16:28:14 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/lexer.h"

t_lexer	*lexer_create(char *input)
{
	t_lexer	*lexer;

	if (!input)
		return (NULL);
	lexer = (t_lexer *)gc_malloc(sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->input = ft_strdup(input);
	lexer->position = 0;
	lexer->read_position = 0;
	lexer->current_char = '\0';
	lexer_read_char(lexer);
	return (lexer);
}

void	lexer_free(t_lexer *lexer)
{
	if (!lexer)
		return ;
	if (lexer->input)
		gc_free(lexer->input);
	gc_free(lexer);
}

void	lexer_read_char(t_lexer *lexer)
{
	if (lexer->read_position >= (int)ft_strlen(lexer->input))
		lexer->current_char = '\0';
	else
		lexer->current_char = lexer->input[lexer->read_position];
	lexer->position = lexer->read_position;
	lexer->read_position++;
}

char	lexer_peek_char(t_lexer *lexer)
{
	if (lexer->read_position >= (int)ft_strlen(lexer->input))
		return ('\0');
	return (lexer->input[lexer->read_position]);
}

t_token	*lexer_tokenize(char *input)
{
	t_lexer	*lexer;
	t_token	*token_list;
	t_token	*current_token;
	t_token	*prev_token;

	prev_token = NULL;
	lexer = lexer_create(input);
	if (!lexer)
		return (NULL);
	token_list = NULL;
	while (1)
	{
		current_token = lexer_next_token(lexer);
		if (!validate_and_add_token(&token_list, current_token, &prev_token))
			break ;
		if (should_stop_tokenizing(current_token))
			break ;
	}
	lexer_free(lexer);
	return (token_list);
}
