/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 20:21:33 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/02 17:08:22 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	setup_pipe_fds(t_exec_context *ctx)
{
	if (pipe(ctx->pipe_fd) == -1)
	{
		perror("minishell:pipe");
		return (-1);
	}
	ctx->has_active_pipe = 1;
	return (0);
}

static void	cleanup_pipe_fds(t_exec_context *ctx)
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
}

static int	execute_left_command(t_ast_node *left_node, t_exec_context *ctx)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		write(STDERR_FILENO, "minishell: fork", 16);
		return (-1);
	}
	if (pid == 0)
	{
		setup_exec_signals();
		close(ctx->pipe_fd[0]);
		dup2(ctx->pipe_fd[1], STDOUT_FILENO);
		close(ctx->pipe_fd[1]);
		exit (execute_command(left_node, ctx));
	}
	add_child_pid(ctx, pid);
	return (0);
}

static int	execute_right_command(t_ast_node *right_node, t_exec_context *ctx)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		write(STDERR_FILENO, "minishell: fork", 16);
		return (-1);
	}
	if (pid == 0)
	{
		setup_exec_signals();
		close(ctx->pipe_fd[1]);
		dup2(ctx->pipe_fd[0], STDIN_FILENO);
		close(ctx->pipe_fd[0]);
		exit (execute_command(right_node, ctx));
	}
	add_child_pid(ctx, pid);
	return (0);
}

int	execute_pipe(t_ast_node *pipe_node, t_exec_context *ctx)
{
	int	result;

	if (!pipe_node || !ctx || !pipe_node->left || !pipe_node->right)
		return (1);
	if (setup_pipe_fds(ctx) == -1)
		return (1);
	if (execute_left_command(pipe_node->left, ctx) == -1)
	{
		cleanup_pipe_fds(ctx);
		return (1);
	}
	if (execute_right_command(pipe_node->right, ctx) == -1)
	{
		cleanup_pipe_fds(ctx);
		return (1);
	}
	cleanup_pipe_fds(ctx);
	result = wait_for_children(ctx);
	return (result);
}
