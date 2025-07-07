/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 19:46:29 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/07 21:17:33 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/executor.h"

static int	backup_std_fds(t_exec_context *ctx)
{
	ctx->stdin_backup = dup(STDIN_FILENO);
	ctx->stdout_backup = dup(STDOUT_FILENO);
	if (ctx->stdin_backup == -1 || ctx->stdout_backup == -1)
	{
		perror("minishell: dup");
		return (1);
	}
	return (0);
}

static void	restore_std_fds(t_exec_context *ctx)
{
	if (ctx->stdin_backup != -1)
	{
		dup2(ctx->stdin_backup, STDIN_FILENO);
		close(ctx->stdin_backup);
		ctx->stdin_backup = -1;
	}
	if (ctx->stdout_backup != -1)
	{
		dup2(ctx->stdout_backup, STDOUT_FILENO);
		close(ctx->stdout_backup);
		ctx->stdout_backup = -1;
	}
}

void	setup_parent_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

static int	execute_by_type(t_ast_node *ast, t_exec_context *ctx)
{
	if (ast->type == NODE_COMMAND)
		return (execute_command(ast, ctx));
	else if (ast->type == NODE_PIPE)
		return (execute_pipe(ast, ctx));
	else if (ast->type == NODE_AND)
		return (execute_logical_and(ast, ctx));
	else if (ast->type == NODE_OR)
		return (execute_logical_or(ast, ctx));
	else if (ast->type == NODE_REDIRECT)
		return (handle_redirection_node(ast, ctx));
	ft_putstr_fd("minishell: unsupported AST node\n", STDERR_FILENO);
	return (1);
}

int	execute_ast(t_ast_node *ast, t_exec_context *ctx)
{
	int	result;

	if (!ast || !ctx)
		return (1);
	init_exec_context(ctx);
	if (backup_std_fds(ctx) != 0)
		return (1);
	setup_parent_signals();
	result = execute_by_type(ast, ctx);
	restore_std_fds(ctx);
	ctx->exit_status = result;
	return (result);
}
