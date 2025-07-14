/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 20:59:16 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/15 01:33:10 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/executor.h"

// void	init_exec_context(t_exec_context *ctx)
// {
// 	ctx->stdin_backup = -1;
// 	ctx->stdout_backup = -1;
// 	ctx->exit_status = 0;
// 	if (!ctx->env)
// 		ctx->env = init_env_from_system();
// }

// int	execute_command(t_ast_node *node, t_exec_context *ctx)
// {
// 	if (!node || !node->args || !node->args[0])
// 		return (1);
// 	if (is_builtin(node->args[0]))
// 		return (exec_builtin(node->args, ctx));
// 	return (exec_external(node->args, ctx));
// }

// int	execute_logical_and(t_ast_node *node, t_exec_context *ctx)
// {
// 	int	left_status;

// 	if (!node || !node->left)
// 		return (1);
// 	left_status = execute_ast(node->left, ctx);
// 	ctx->exit_status = left_status;
// 	if (left_status != 0)
// 		return (left_status);
// 	if (node->right)
// 		return (execute_ast(node->right, ctx));
// 	return (0);
// }

// int	execute_logical_or(t_ast_node *node, t_exec_context *ctx)
// {
// 	int	left_status;

// 	if (!node || !node->left)
// 		return (1);
// 	left_status = execute_ast(node->left, ctx);
// 	ctx->exit_status = left_status;
// 	if (left_status == 0)
// 		return (0);
// 	if (node->right)
// 		return (execute_ast(node->right, ctx));
// 	return (left_status);
// }

// int	handle_redirection_node(t_ast_node *ast, t_exec_context *ctx)
// {
// 	if (ast->type != NODE_REDIRECT)
// 		return (0);
// 	if (setup_redirection(ast, ctx) != 0)
// 		return (1);
// 	if (ast->left)
// 		return (execute_ast(ast->left, ctx));
// 	return (0);
// }
