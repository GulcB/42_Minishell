/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 09:27:54 by mdivan            #+#    #+#             */
/*   Updated: 2025/08/02 12:45:05 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_lexer	*lexer_create(char *input, t_gc *gc)
{
	t_lexer	*lexer;

	if (!input || !gc)
		return (NULL);
	lexer = (t_lexer *)gc_malloc(gc, sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->input = gc_strdup(gc, input);
	lexer->position = 0;
	lexer->read_position = 0;
	lexer->current_char = '\0';
	lexer->gc = gc;
	lexer_read_char(lexer);
	return (lexer);
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

t_token	*lexer_tokenize_with_context(char *input, t_gc *gc)
{
	t_lexer	*lexer;
	t_token	*token_list;
	t_token	*current_token;
	t_token	*prev_token;

	prev_token = NULL;
	lexer = lexer_create(input, gc);
	if (!lexer)
		return (NULL);
	token_list = NULL;
	while (1)
	{
		current_token = lexer_next_token(gc,lexer);
		if (should_stop_tokenizing(current_token))
		{
			if (current_token && current_token->type == TOKEN_ERROR)
			{
				// Print error message and return NULL to stop execution
				write(2, current_token->value, ft_strlen(current_token->value));
				write(2, "\n", 1);
				lexer_free(lexer);
				return (NULL);
			}
			if (current_token)
				token_add_back(&token_list, current_token);
			break ;
		}
		if (!validate_and_add_token(gc, &token_list, current_token,
				&prev_token))
		{
			// Syntax error detected, return NULL to stop execution
			lexer_free(lexer);
			return (NULL);
		}
	}
	lexer_free(lexer);
	return (token_list);
}
