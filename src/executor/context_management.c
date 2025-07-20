/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 00:34:33 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/20 19:13:28 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	allocate_child_array(t_exec_context *ctx)
{
	ctx->child_pids = (pid_t *)gc_malloc(ctx->gc, sizeof(pid_t) * MAX_CHILDREN);
	if (!ctx->child_pids)
		return (-1);
	ctx->max_children = MAX_CHILDREN;
	ctx->child_count = 0;
	return (0);
}

static void	init_context_values(t_exec_context *ctx, t_env *env, t_gc *gc)
{
	ctx->env = env;
	ctx->gc = gc;
	ctx->exit_status = 0;
	ctx->stdin_backup = -1;
	ctx->stdout_backup = -1;
	ctx->pipe_fd[0] = -1;
	ctx->pipe_fd[1] = -1;
	ctx->has_active_pipe = 0;
}

t_exec_context	*init_exec_context(t_env *env, t_gc *gc)
{
	t_exec_context	*ctx;

	if (!gc)
		return (NULL);
	ctx = (t_exec_context *)malloc(sizeof(t_exec_context));
	if (!ctx)
		return (NULL);
	init_context_values(ctx, env, gc);
	if (allocate_child_array(ctx) == -1)
	{
		free(ctx);
		return (NULL);
	}
	return (ctx);
}

int	backup_std_fds(t_exec_context *ctx)
{
	ctx->stdin_backup = dup(STDIN_FILENO);
	ctx->stdout_backup = dup(STDOUT_FILENO);
	if (ctx->stdin_backup == -1 || ctx->stdout_backup == -1)
	{
		perror("minishell: dup");
		return (-1);
	}
	return (0);
}

void	restore_std_fds(t_exec_context *ctx)
{
	if (ctx->stdin_backup != -1)
	{
		dup2(ctx->stdin_backup, STDIN_FILENO);
		close(ctx->stdin_backup);
		ctx->stdin_backup = -1;
	}
	if (ctx->stdout_backup != -1)
	{
		dup2(ctx->stdout_backup, STDOUT_FILENO);
		close(ctx->stdout_backup);
		ctx->stdout_backup = -1;
	}
}
