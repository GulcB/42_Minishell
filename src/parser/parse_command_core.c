/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_core.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:01 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 20:54:08 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"

static t_ast_node	*create_command_from_args(struct s_exec_context *ctx,
		int arg_count, t_token **current, t_redirect_pair *redirects)
{
	t_ast_node	*cmd_node;
	t_args_info	args_info;
	char		**args;
	int			i;

	args = (char **)gc_malloc(ctx->gc, sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	i = 0;
	{
		args_info.args = args;
		args_info.i = &i;
		args_info.arg_count = arg_count;
		if (!parse_tokens_to_args(current, ctx, &args_info, redirects))
			return (*(redirects->first));
	}
	args[arg_count] = NULL;
	cmd_node = create_command_node(ctx->gc, args);
	if (!cmd_node)
		return (*(redirects->first));
	return (cmd_node);
}

static t_ast_node	*build_command_with_redirects(t_token **current,
		struct s_exec_context *ctx, t_ast_node **cmd_node,
		t_ast_node **first_redirect)
{
	t_ast_node		*last_redirect;
	t_token			*start_token;
	t_redirect_pair	redirects;
	int				arg_count;

	last_redirect = NULL;
	start_token = *current;
	arg_count = count_command_args(*current);
	if (arg_count > 0)
	{
		redirects.first = first_redirect;
		redirects.last = &last_redirect;
		*current = start_token;
		*cmd_node = create_command_from_args(ctx, arg_count, current,
				&redirects);
		if (!*cmd_node && *first_redirect)
			return (*first_redirect);
	}
	parse_remaining_redirects(current, ctx, first_redirect, &last_redirect);
	if (*cmd_node && *first_redirect)
		(*cmd_node)->right = *first_redirect;
	else if (!*cmd_node && *first_redirect)
		return (*first_redirect);
	return (*cmd_node);
}

t_ast_node	*parse_command(t_token **current, struct s_exec_context *ctx)
{
	t_ast_node	*cmd_node;
	t_ast_node	*first_redirect;

	cmd_node = NULL;
	first_redirect = NULL;
	if (!current || !*current || !ctx || !ctx->gc)
		return (NULL);
	return (build_command_with_redirects(current, ctx, &cmd_node,
			&first_redirect));
}
