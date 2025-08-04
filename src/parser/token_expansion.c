/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:01 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 20:54:03 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"

static int	get_default_length(char *value)
{
	if (value)
		return (ft_strlen(value));
	return (1);
}

static int	calculate_quote_end(t_token *current)
{
	return (current->position + 2 + get_value_length(current->value));
}

static int	calculate_dollar_end(t_token *current)
{
	int	content_len;

	if (current->value && ft_strncmp(current->value, "LITERAL:", 8) == 0)
	{
		content_len = ft_strlen(current->value + 8);
		return (current->position + 3 + content_len);
	}
	else if (current->value && ft_strlen(current->value) == 1
		&& (current->value[0] == '?' || ft_isdigit(current->value[0])))
	{
		return (current->position + 2);
	}
	else
	{
		return (current->position + 1 + get_value_length(current->value));
	}
}

int	parse_tokens_are_adjacent(t_token *current, t_token *next)
{
	int	current_end;

	if (!current || !next)
		return (0);
	if (current->type == TOKEN_DQUOTE)
		current_end = calculate_quote_end(current);
	else if (current->type == TOKEN_SQUOTE)
		current_end = calculate_quote_end(current);
	else if (current->type == TOKEN_DOLLAR)
		current_end = calculate_dollar_end(current);
	else
		current_end = current->position + get_default_length(current->value);
	return (current_end == next->position);
}

char	*expand_token_value(t_token *token, struct s_exec_context *ctx)
{
	char	*expanded_value;

	if (!token || !token->value)
		return (NULL);
	if (token->type == TOKEN_DOLLAR)
	{
		expanded_value = get_env_value(token->value, ctx);
		if (!expanded_value)
			return (gc_strdup(ctx->gc, ""));
		return (expanded_value);
	}
	else if (token->type == TOKEN_DQUOTE)
		return (expand_variables(token->value, ctx));
	else if (token->type == TOKEN_SQUOTE)
		return (gc_strdup(ctx->gc, token->value));
	else
		return (expand_variables(token->value, ctx));
}
