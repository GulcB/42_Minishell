/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recursive_pipe_execution.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:53:34 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/17 16:53:57 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/executor.h"

static int	execute_with_redirection(t_ast_node *node, t_exec_context *ctx,
		int input_fd, int output_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (-1);
	}
	if (pid == 0)
	{
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
		exit(execute_command(node, ctx));
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
		perror("minishell: pipe");
		return (-1);
	}
	if (execute_pipe_recursive(node->left, ctx, input_fd, pipe_fds[1]) == -1)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return (-1);
	}
	close(pipe_fds[1]);
	if (execute_pipe_recursive(node->right, ctx, pipe_fds[0], output_fd) == -1)
	{
		close(pipe_fds[0]);
		return (-1);
	}
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
		ft_putstr_fd("minishell: invalid pipe chain\n", STDERR_FILENO);
		return (1);
	}
	cmd_count = count_pipe_commands(pipe_node);
	if (cmd_count > ctx->max_children)
	{
		ft_putstr_fd("minishell: pipe chain too long\n", STDERR_FILENO);
		return (1);
	}
	result = execute_pipe_recursive(pipe_node, ctx, STDIN_FILENO, STDOUT_FILENO);
	if (result == -1)
		return (1);
	return (wait_for_pipe_children(ctx));
}
