/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 00:35:12 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/15 01:07:50 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/executor.h"

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

static int	wait_single_child(pid_t pid, t_exec_context *ctx)
{
	int	status;
	int	wait_result;

	wait_result = waitpid(pid, &status, 0);
	if (wait_result == -1)
	{
		perror("minishell: waitpid");
		return (-1);
	}
	update_exit_status(ctx, status);
	return (ctx->exit_status);
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
		last_exit_status = wait_single_child(ctx->child_pids[i], ctx);
		i++;
	}
	cleanup_children(ctx);
	return (last_exit_status);
}

void	update_exit_status(t_exec_context *ctx, int status)
{
	if (is_normal_exit(status))
		ctx->exit_status = extract_exit_code(status);
	else if (is_signaled_exit(status))
		ctx->exit_status = 128 + extract_signal_number(status);
	else
		ctx->exit_status = 1;
}

void	cleanup_children(t_exec_context *ctx)
{
	if (!ctx)
		return ;
	ctx->child_count = 0;
}
