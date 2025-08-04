/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 22:30:00 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 22:30:18 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	setup_child_redirection(int input_fd, int output_fd)
{
	if (input_fd != STDIN_FILENO)
		dup2(input_fd, STDIN_FILENO);
	if (output_fd != STDOUT_FILENO)
		dup2(output_fd, STDOUT_FILENO);
}

void	close_fds_in_child(void)
{
	int	fd;

	fd = 3;
	while (fd < 256)
	{
		close(fd);
		fd++;
	}
}

void	execute_child_process_with_redirect(t_ast_node *node,
		t_exec_context *ctx, int input_fd, int output_fd)
{
	int	exit_status;

	setup_exec_signals();
	setup_child_redirection(input_fd, output_fd);
	close_fds_in_child();
	exit_status = execute_command(node, ctx);
	gc_destroy(ctx->gc);
	exit(exit_status);
}

int	process_command_heredocs(t_ast_node *node, t_exec_context *ctx)
{
	t_ast_node	*redirect_node;
	int			result;

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

int	process_pipe_heredocs(t_ast_node *node, t_exec_context *ctx)
{
	int	result;

	result = preprocess_heredocs_in_pipe_chain(node->left, ctx);
	if (result != 0)
		return (result);
	result = preprocess_heredocs_in_pipe_chain(node->right, ctx);
	if (result != 0)
		return (result);
	return (0);
}
