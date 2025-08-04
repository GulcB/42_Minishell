/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_main.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 15:30:00 by mdivan            #+#    #+#             */
/*   Updated: 2025/08/04 20:05:13 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "lexer.h"

char	*process_expansion_loop(char *result, const char *str,
		struct s_exec_context *ctx)
{
	int					i;
	int					start;
	t_expansion_params	params;
	t_position_params	pos_params;

	i = 0;
	start = 0;
	pos_params.result = result;
	pos_params.str = str;
	pos_params.start = &start;
	pos_params.i = &i;
	pos_params.ctx = ctx;
	while (str[i])
	{
		pos_params.result = result;
		result = process_char_at_position(pos_params);
	}
	init_expansion_params(&params, result, str, ctx);
	params.start = start;
	result = add_final_literal(params, i);
	return (result);
}

char	*expand_variables(const char *str, struct s_exec_context *ctx)
{
	char	*result;
	char	*tilde_expanded;

	if (!str)
		return (NULL);
	tilde_expanded = expand_tilde(str, ctx);
	if (!tilde_expanded)
		return (NULL);
	result = gc_strdup(ctx->gc, "");
	if (!result)
	{
		return (NULL);
	}
	result = process_expansion_loop(result, tilde_expanded, ctx);
	return (result);
}

char	*process_token_expansion(t_token *token, struct s_exec_context *ctx)
{
	if (!token || !token->value)
		return (NULL);
	if (token->type == TOKEN_DQUOTE)
		return (expand_variables(token->value, ctx));
	if (token->type == TOKEN_SQUOTE)
		return (gc_strdup(ctx->gc, token->value));
	if (token->type == TOKEN_WORD)
		return (expand_variables(token->value, ctx));
	return (gc_strdup(ctx->gc, token->value));
}
