/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 00:35:12 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/24 16:00:53 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	add_child_pid(t_exec_context *ctx, pid_t pid)
{
	if (!ctx || !ctx->child_pids)
		return (-1);
	if (ctx->child_count >= ctx->max_children)
		return (-1);
	ctx->child_pids[ctx->child_count] = pid;
	ctx->child_count++;
	return (0);
}

static int	wait_single_child(t_exec_context *ctx, int index)
{
	int		status;
	pid_t	result;

	if (index >= ctx->child_count)
		return (1);
	result = waitpid(ctx->child_pids[index], &status, 0);
	if (result == -1)
	{
		if (errno == ECHILD)
			return (0);
		return (1);
	}
	if (result == ctx->child_pids[index])
	{
		update_exit_status(ctx, status);
		return (ctx->exit_status);
	}
	return (1);
}

int	wait_for_children(t_exec_context *ctx)
{
	int	i;
	int	last_exit_status;

	if (!ctx || ctx->child_count == 0)
		return (0);
	last_exit_status = 0;
	i = 0;
	while (i < ctx->child_count)
	{
		last_exit_status = wait_single_child(ctx, i);
		i++;
	}
	cleanup_children(ctx);
	return (last_exit_status);
}

void	cleanup_children(t_exec_context *ctx)
{
	if (!ctx)
		return ;
	ctx->child_count = 0;
}
