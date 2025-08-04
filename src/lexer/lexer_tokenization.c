/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenization.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:30:00 by mdivan            #+#    #+#             */
/*   Updated: 2025/08/04 19:40:52 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static int	handle_error_token(t_token *token, t_lexer *lexer)
{
	if (token && token->type == TOKEN_ERROR)
	{
		write(2, token->value, ft_strlen(token->value));
		write(2, "\n", 1);
		lexer_free(lexer);
		return (0);
	}
	return (1);
}

static int	process_stop_token(t_token *token, t_token **token_list,
		t_lexer *lexer)
{
	if (!handle_error_token(token, lexer))
		return (0);
	if (token)
		token_add_back(token_list, token);
	return (1);
}

static int	tokenize_loop(t_lexer *lexer, t_token **token_list, t_gc *gc)
{
	t_token	*current_token;
	t_token	*prev_token;

	prev_token = NULL;
	while (1)
	{
		current_token = lexer_next_token(gc, lexer);
		if (should_stop_tokenizing(current_token))
		{
			if (!process_stop_token(current_token, token_list, lexer))
				return (0);
			break ;
		}
		if (!validate_and_add_token(gc, token_list, current_token, &prev_token))
		{
			lexer_free(lexer);
			return (0);
		}
	}
	return (1);
}

t_token	*lexer_tokenize_with_context(char *input, t_gc *gc)
{
	t_lexer	*lexer;
	t_token	*token_list;

	lexer = lexer_create(input, gc);
	if (!lexer)
		return (NULL);
	token_list = NULL;
	if (!tokenize_loop(lexer, &token_list, gc))
		return (NULL);
	lexer_free(lexer);
	return (token_list);
}
