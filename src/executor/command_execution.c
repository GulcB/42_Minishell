/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 20:38:18 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/29 17:18:02 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	**convert_env_to_array(t_env *env, t_gc *gc)
{
	char	**env_array;
	t_env	*current;
	char	*env_string;
	int		count;
	int		i;

	count = 0;
	current = env;
	while (current && ++count)
		current = current->next;
	env_array = (char **)gc_malloc(gc, sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		env_string = ft_strjoin(current->key, "=");
		env_array[i] = ft_strjoin(env_string, current->value);
		free(env_string);
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

static int	execute_external_command(char **args, t_exec_context *ctx)
{
	char	*executable_path;
	char	**env_array;
	pid_t	pid;

	executable_path = resolve_executable(args[0], ctx->env);
	if (!executable_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		return (127);
	}
	env_array = convert_env_to_array(ctx->env, ctx->gc);
	setup_exec_signals();
	pid = fork();
	if (pid == 0)
	{

		execve(executable_path, args, env_array);
		exit(127);
	}
	add_child_pid(ctx, pid);
	free(executable_path);
	return (wait_for_children(ctx));
}

static int	execute_heredoc_consume_only(t_ast_node *heredoc_node, t_exec_context *ctx)
{
	char	*delimiter;
	char	*line;

	(void)ctx;
	if (!heredoc_node || heredoc_node->redirect_type != REDIRECT_HEREDOC)
		return (1);
	delimiter = heredoc_node->redirect_file;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putchar_fd('\n', STDOUT_FILENO);
			return (1);
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& ft_strlen(line) == ft_strlen(delimiter))
		{
			free(line);
			break ;
		}
		free(line);
	}
	return (0);
}

int	execute_command(t_ast_node *cmd_node, t_exec_context *ctx)
{
	int	result;

	if (!cmd_node || !ctx)
		return (1);
	if (cmd_node->type == NODE_REDIRECT)
		return (execute_redirect(cmd_node, ctx));
	if (!cmd_node->args || !cmd_node->args[0])
		return (1);
	if (cmd_node->right && cmd_node->right->type == NODE_REDIRECT)
	{
		t_ast_node *redirect_node = cmd_node->right;
		t_ast_node *last_heredoc = NULL;
		while (redirect_node)
		{
			if (redirect_node->redirect_type == REDIRECT_HEREDOC)
				last_heredoc = redirect_node;
			else
				execute_redirect(redirect_node, ctx);
			redirect_node = redirect_node->right;
		}
		redirect_node = cmd_node->right;
		while (redirect_node)
		{
			if (redirect_node->redirect_type == REDIRECT_HEREDOC)
			{
				if (redirect_node == last_heredoc)
					execute_redirect(redirect_node, ctx);
				else
					execute_heredoc_consume_only(redirect_node, ctx);
			}
			redirect_node = redirect_node->right;
		}
		
		if (is_builtin_command(cmd_node->args[0]))
			result = execute_builtin_dispatcher(cmd_node->args, ctx);
		else
			result = execute_external_command(cmd_node->args, ctx);
		restore_std_fds(ctx);
		return (result);
	}
	if (is_builtin_command(cmd_node->args[0]))
		return (execute_builtin_dispatcher(cmd_node->args, ctx));
	else
		return (execute_external_command(cmd_node->args, ctx));
}
