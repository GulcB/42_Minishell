/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 19:46:29 by gbodur            #+#    #+#             */
/*   Updated: 2025/06/27 17:16:16 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/executor.h"

int execute_ast(t_ast_node *ast, t_exec_context *ctx)
{
	if (!ast || !ctx)
		return (1);
	if (ast->type == NODE_COMMAND)
		return (execute_command(ast, ctx));
	else if (ast->type == NODE_PIPE)
		return (execute_pipe(ast,ctx));
	else
	{
		printf("Error; Unsupported node type\n");
		return (1);
	}
}

int	execute_command(t_ast_node *ast, t_exec_context *ctx)
{
	char	*cmd_path;
	pid_t 	pid;
	int		
}

