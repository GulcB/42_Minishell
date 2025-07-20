/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:23:43 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/20 15:55:09 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*handle_word_token(t_lexer *lexer, int start_pos)
{
	t_token	*token;
	char	*value;

	value = read_word(lexer);
	if (!value)
		return (token_create(lexer->gc, TOKEN_ERROR, "Invalid token", start_pos));
	token = token_create(lexer->gc, TOKEN_WORD, value, start_pos);
	gc_free(lexer->gc, value);
	return (token);
}

int	validate_and_add_token(t_gc *gc, t_token **token_list,
		t_token *current_token, t_token **prev_token)
{
	if (!current_token)
		return (0);
	if (!is_valid_token_sequence(*prev_token, current_token))
	{
		if (current_token->type == TOKEN_ERROR)
		{
			token_free_list(gc, current_token);
			return (1);
		}
	}
	token_add_back(gc, token_list, current_token);
	*prev_token = current_token;
	return (1);
}

int	validate_and_add_token_gc(t_gc *gc, t_token **token_list,
		t_token *current_token, t_token **prev_token)
{
	if (!current_token)
		return (0);
	if (!is_valid_token_sequence(*prev_token, current_token))
	{
		if (current_token->type == TOKEN_ERROR)
		{
			token_free_list(gc, current_token);
			return (1);
		}
	}
	token_add_back(gc, token_list, current_token);
	*prev_token = current_token;
	return (1);
}

int	should_stop_tokenizing(t_token *current_token)
{
	if (!current_token)
		return (1);
	if (current_token->type == TOKEN_EOF || current_token->type == TOKEN_ERROR)
		return (1);
	return (0);
}
