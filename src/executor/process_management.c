/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 00:35:12 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/20 21:23:47 by gbodur           ###   ########.fr       */
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

int	wait_for_children(t_exec_context *ctx)
{
	int	i;
	int	last_exit_status;
	int	status;

	if (!ctx || ctx->child_count == 0)
		return (0);
	last_exit_status = 0;
	i = 0;
	while (i < ctx->child_count)
	{
		if (waitpid(ctx->child_pids[i], &status, 0) == -1)
		{
			perror("minishell:waitpid");
			last_exit_status = 1;
		}
		else
		{
			update_exit_status(ctx, status);
			last_exit_status = ctx->exit_status;
		}
		i++;
	}
	cleanup_children(ctx);
	reset_signal_flag();
	return (last_exit_status);
}

void	cleanup_children(t_exec_context *ctx)
{
	if (!ctx)
		return ;
	ctx->child_count = 0;
}
