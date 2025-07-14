/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 19:46:29 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/15 01:43:31 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/executor.h"

void	setup_parent_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_parent_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

static int	prepare_execution(t_exec_context *ctx)
{
	if (backup_std_fds(ctx) != 0)
		return (-1);
	setup_parent_signals();
	return (0);
}

static void	cleanup_execution(t_exec_context *ctx)
{
	restore_std_fds(ctx);
	restore_parent_signals();
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
	else
	{
		ft_putstr_fd("minishell: unsupported node type\n", STDERR_FILENO);
		result = 1;
	}
	cleanup_execution(ctx);
	ctx->exit_status = result;
	return (result);
}
