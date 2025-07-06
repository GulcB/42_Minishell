/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 19:46:29 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/06 20:03:05 by gbodur           ###   ########.fr       */
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

int	execute_command(t_ast_node *node, t_exec_context *ctx)
{
	char	*cmd_path;
	pid_t 	pid;
	int		status;
	char	**env_array;

	if (!node || !node->args || !node->args[0])
		return (1);
	if (is_builtin(node->args[0]))
		return (execute_builtin(node->args, ctx));
	cmd_path = find_command_path(node->args[0], ctx->env);
	if (!cmd_path)
	{
		printf("minishell: %s: command not found\n", node->args[0]);
		return (127);
	}
	pid = fork ();
	if (pid == 0)
	{
		env_array = env_to_array(ctx->env);
		execve = (cmd_path, node->args, env_array);
		perror("execve failed");
		exit (127);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		free(cmd_path);
		return (WEXITSTATUS(status));
	}
	free(cmd_path);
	return (1);
}

