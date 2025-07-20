/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 00:35:08 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/20 22:32:29 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/executor.h"

static void	cleanup_pipes(t_exec_context *ctx)
{
	if (ctx->pipe_fd[0] != -1)
	{
		close(ctx->pipe_fd[0]);
		ctx->pipe_fd[0] = -1;
	}
	if (ctx->pipe_fd[1] != -1)
	{
		close(ctx->pipe_fd[1]);
		ctx->pipe_fd[1] = -1;
	}
	ctx->has_active_pipe = 0;
}

static void	cleanup_resources(t_exec_context *ctx)
{
	cleanup_pipes(ctx);
	restore_std_fds(ctx);
}

void	cleanup_exec_context(t_exec_context *ctx)
{
	if (!ctx)
		return ;
	cleanup_resources(ctx);
	ctx->gc = NULL;
	free(ctx);
}
