/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 20:38:18 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 22:19:58 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	clean_empty_args(t_ast_node *cmd_node)
{
	int	i;
	int	j;

	i = 0;
	while (cmd_node->args[i] && ft_strlen(cmd_node->args[i]) == 0)
		i++;
	if (i > 0)
	{
		j = 0;
		while (cmd_node->args[i + j])
		{
			cmd_node->args[j] = cmd_node->args[i + j];
			j++;
		}
		cmd_node->args[j] = NULL;
	}
}

int	execute_redirections(t_ast_node *redirect_node,
		t_ast_node *last_heredoc, t_exec_context *ctx)
{
	int	redirect_error;

	redirect_error = 0;
	while (redirect_node)
	{
		if (redirect_node->redirect_type != REDIRECT_HEREDOC)
		{
			redirect_error = execute_redirection(redirect_node, ctx);
			if (redirect_error != 0)
				break ;
		}
		else if (redirect_node == last_heredoc)
		{
			redirect_error = execute_redirection(redirect_node, ctx);
			if (redirect_error != 0)
				break ;
		}
		redirect_node = redirect_node->right;
	}
	return (redirect_error);
}

int	execute_final_command(t_ast_node *cmd_node, t_exec_context *ctx)
{
	int	result;

	if (is_builtin_command(cmd_node->args[0]))
		result = execute_builtin_dispatcher(cmd_node->args, ctx);
	else
		result = execute_external_command(cmd_node->args, ctx);
	restore_std_fds(ctx);
	return (result);
}

int	handle_redirections_and_execute(t_ast_node *cmd_node,
		t_exec_context *ctx)
{
	t_ast_node	*redirect_node;
	t_ast_node	*last_heredoc;
	int			redirect_error;

	redirect_node = cmd_node->right;
	last_heredoc = find_last_heredoc(redirect_node);
	process_non_last_heredocs(cmd_node->right, last_heredoc, ctx);
	redirect_error = execute_redirections(cmd_node->right, last_heredoc, ctx);
	if (redirect_error)
	{
		restore_std_fds(ctx);
		if (redirect_error == 130)
			return (130);
		redirect_error = 1;
		return (redirect_error);
	}
	return (execute_final_command(cmd_node, ctx));
}

int	execute_command(t_ast_node *cmd_node, t_exec_context *ctx)
{
	if (!cmd_node || !ctx)
		return (1);
	if (cmd_node->type == NODE_REDIRECT)
		return (execute_redirection(cmd_node, ctx));
	if (!cmd_node->args || !cmd_node->args[0])
		return (1);
	clean_empty_args(cmd_node);
	if (!cmd_node->args[0])
		return (0);
	if (cmd_node->right && cmd_node->right->type == NODE_REDIRECT)
		return (handle_redirections_and_execute(cmd_node, ctx));
	if (is_builtin_command(cmd_node->args[0]))
		return (execute_builtin_dispatcher(cmd_node->args, ctx));
	else
		return (execute_external_command(cmd_node->args, ctx));
}
