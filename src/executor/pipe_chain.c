/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_chain.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:47:47 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/24 14:33:40 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	wait_single_pipe_child(t_exec_context *ctx, int index)
{
	int	status;
	int	wait_result;

	wait_result = waitpid(ctx->child_pids[index], &status, 0);
	if (wait_result == -1)
	{
		perror("minishell: waitpid");
		return (1);
	}
	update_exit_status(ctx, status);
	return (ctx->exit_status);
}

static int	wait_for_pipe_chain(t_exec_context *ctx)
{
	int	i;
	int	last_exit_status;

	if (!ctx || ctx->child_count == 0)
		return (0);
	last_exit_status = 0;
	i = 0;
	while (i < ctx->child_count)
	{
		last_exit_status = wait_single_pipe_child(ctx, i);
		i++;
	}
	cleanup_children(ctx);
	return (last_exit_status);
}

int	count_pipe_commands(t_ast_node *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
		return (1);
	if (node->type == NODE_PIPE)
		return (count_pipe_commands(node->left)
			+ count_pipe_commands(node->right));
	return (0);
}

int	validate_pipe_chain(t_ast_node *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
		return (node->args && node->args[0]);
	if (node->type == NODE_PIPE)
		return (validate_pipe_chain(node->left)
			&& validate_pipe_chain(node->right));
	return (0);
}

int	wait_for_pipe_children(t_exec_context *ctx)
{
	return (wait_for_pipe_chain(ctx));
}
