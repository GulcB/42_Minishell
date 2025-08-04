/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_processing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:01 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 20:54:07 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"

static t_ast_node	*process_word_token(t_token **current,
		struct s_exec_context *ctx, char ***args, int *i)
{
	char	*expanded_value;
	char	*concatenated_arg;

	expanded_value = expand_token_value(*current, ctx);
	if (!expanded_value)
		return (NULL);
	concatenated_arg = expanded_value;
	*current = (*current)->next;
	while (*current && is_word_token(*current) && !is_stop_token(*current)
		&& !is_redirect_token(*current) && (*current)->prev
		&& parse_tokens_are_adjacent((*current)->prev, *current))
	{
		expanded_value = expand_token_value(*current, ctx);
		if (!expanded_value)
			return (NULL);
		concatenated_arg = gc_strjoin(ctx->gc, concatenated_arg,
				expanded_value);
		if (!concatenated_arg)
			return (NULL);
		*current = (*current)->next;
	}
	(*args)[*i] = concatenated_arg;
	(*i)++;
	return ((t_ast_node *)1);
}

void	handle_redirects(t_token **current, struct s_exec_context *ctx,
		t_ast_node **first_redirect, t_ast_node **last_redirect)
{
	t_ast_node	*redirect_node;

	redirect_node = parse_redirect(current, ctx);
	if (redirect_node)
	{
		if (!*first_redirect)
		{
			*first_redirect = redirect_node;
			*last_redirect = redirect_node;
		}
		else
		{
			(*last_redirect)->right = redirect_node;
			*last_redirect = redirect_node;
		}
	}
}

int	parse_tokens_to_args(t_token **current, struct s_exec_context *ctx,
		t_args_info *args_info, t_redirect_pair *redirects)
{
	while (*current && !is_stop_token(*current)
		&& *(args_info->i) < args_info->arg_count)
	{
		if (is_redirect_token(*current))
			handle_redirects(current, ctx, redirects->first, redirects->last);
		else if (is_word_token(*current))
		{
			if (!process_word_token(current, ctx, &(args_info->args),
					args_info->i))
				return (0);
		}
		else
			*current = (*current)->next;
	}
	return (1);
}

void	parse_remaining_redirects(t_token **current,
		struct s_exec_context *ctx, t_ast_node **first_redirect,
		t_ast_node **last_redirect)
{
	while (*current && is_redirect_token(*current))
		handle_redirects(current, ctx, first_redirect, last_redirect);
}
