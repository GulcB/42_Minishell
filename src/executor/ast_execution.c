/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 19:46:29 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/30 14:16:57 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	prepare_execution(t_exec_context *ctx)
{
	if (backup_std_fds(ctx) != 0)
		return (-1);
	return (0);
}

static void	cleanup_execution(t_exec_context *ctx)
{
	restore_std_fds(ctx);
}

int	execute_ast(t_ast_node *ast, t_exec_context *ctx)
{
	int	result;

	if (!ast || !ctx)
		return (1);
	if (prepare_execution(ctx) != 0)
		return (1);
	if (ast->type == NODE_COMMAND)
		result = execute_command(ast, ctx);
	else if (ast->type == NODE_PIPE)
		result = execute_pipe_chain(ast, ctx);
	else if (ast->type == NODE_REDIRECT)
		result = execute_redirection(ast, ctx);
	else
	{
		write(STDERR_FILENO, "minishell: unsupported node type\n", 34);
		result = 1;
	}
	if (result == SHELL_EXIT)
		return (SHELL_EXIT);
	cleanup_execution(ctx);
	ctx->exit_status = result;
	return (result);
}
