/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recursive_pipe_execution.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:53:34 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/02 20:16:25 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	execute_with_redirection(t_ast_node *node, t_exec_context *ctx,
		int input_fd, int output_fd)
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
		if (input_fd != STDIN_FILENO)
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		if (output_fd != STDOUT_FILENO)
		{
			dup2(output_fd, STDOUT_FILENO);
			close(output_fd);
		}

		int exit_status = execute_command(node, ctx);
		gc_destroy(ctx->gc);
		exit(exit_status);
		
	}
	add_child_pid(ctx, pid);
	return (0);
}

static int	execute_pipe_recursive(t_ast_node *node, t_exec_context *ctx,
		int input_fd, int output_fd)
{
	int	pipe_fds[2];

	if (node->type == NODE_COMMAND)
		return (execute_with_redirection(node, ctx, input_fd, output_fd));
	if (node->type != NODE_PIPE)
		return (1);
	if (pipe(pipe_fds) == -1)
	{
		write(STDERR_FILENO, "minishell: pipe", 16);
		return (-1);
	}
	execute_pipe_recursive(node->left, ctx, input_fd, pipe_fds[1]);
	close(pipe_fds[1]);
	execute_pipe_recursive(node->right, ctx, pipe_fds[0], output_fd);
	close(pipe_fds[0]);
	return (0);
}

int	execute_pipe_chain(t_ast_node *pipe_node, t_exec_context *ctx)
{
	int	result;
	int	cmd_count;

	if (!pipe_node || !ctx)
		return (1);
	if (!validate_pipe_chain(pipe_node))
	{
		write(STDERR_FILENO, "minishell: invalid pipe chain\n", 31);
		return (1);
	}
	cmd_count = count_pipe_commands(pipe_node);
	if (cmd_count > ctx->max_children)
	{
		write(STDERR_FILENO, "minishell: pipe chain too long\n", 32);
		return (1);
	}
	result = execute_pipe_recursive(pipe_node, ctx, STDIN_FILENO,
			STDOUT_FILENO);
	if (result == -1)
		return (1);
	return (wait_for_pipe_children(ctx));
}
