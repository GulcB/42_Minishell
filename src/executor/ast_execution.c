/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 19:46:29 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/24 15:59:51 by gbodur           ###   ########.fr       */
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
	else
	{
		ft_putstr_fd("minishell: unsupported node type\n", STDERR_FILENO);
		result = 1;
	}
	if (result == SHELL_EXIT)
		return (SHELL_EXIT);
	cleanup_execution(ctx);
	ctx->exit_status = result;
	return (result);
}
