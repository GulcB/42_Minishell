/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recursive_pipe_execution.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:53:34 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 21:01:47 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	close_all_fds_except(int keep_fd1, int keep_fd2, int keep_fd3)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
	{
		if (fd != keep_fd1 && fd != keep_fd2 && fd != keep_fd3)
			close(fd);
		fd++;
	}
}

static int	execute_with_redirection(t_ast_node *node, t_exec_context *ctx,
		int input_fd, int output_fd)
{
	pid_t	pid;
	int		fd;
	int		exit_status;

	fd = 0;
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
		}
		if (output_fd != STDOUT_FILENO)
		{
			dup2(output_fd, STDOUT_FILENO);
		}
		fd = 3;
		while (fd < 256)
		{
			close(fd);
			fd++;
		}
		exit_status = execute_command(node, ctx);
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

static int	preprocess_heredocs_in_pipe_chain(t_ast_node *node,
		t_exec_context *ctx)
{
	t_ast_node	*redirect_node;
	int			result;

	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
	{
		redirect_node = node->right;
		while (redirect_node && redirect_node->type == NODE_REDIRECT)
		{
			if (redirect_node->redirect_type == REDIRECT_HEREDOC)
			{
				result = preprocess_heredoc(redirect_node, ctx);
				if (result != 0)
					return (result);
			}
			redirect_node = redirect_node->right;
		}
		return (0);
	}
	if (node->type == NODE_PIPE)
	{
		result = preprocess_heredocs_in_pipe_chain(node->left, ctx);
		if (result != 0)
			return (result);
		result = preprocess_heredocs_in_pipe_chain(node->right, ctx);
		if (result != 0)
			return (result);
	}
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
	result = preprocess_heredocs_in_pipe_chain(pipe_node, ctx);
	if (result != 0)
		return (result);
	result = execute_pipe_recursive(pipe_node, ctx, STDIN_FILENO,
			STDOUT_FILENO);
	if (result == -1)
		return (1);
	return (wait_for_pipe_children(ctx));
}
