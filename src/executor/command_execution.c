/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 20:38:18 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/03 11:05:56 by gbodur           ###   ########.fr       */
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
		env_string = gc_strjoin(gc, current->key, "=");
		env_array[i] = gc_strjoin(gc, env_string, current->value);
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

	if (!args || !args[0] || ft_strlen(args[0]) == 0)
		return (0);
	executable_path = resolve_executable(ctx, args[0], ctx->env);
	if (!executable_path)
	{
		if (!ft_strchr(args[0], '/'))
		{
			write(STDERR_FILENO, "minishell: ", 11);
			write(STDERR_FILENO, args[0], ft_strlen(args[0]));
			write(STDERR_FILENO, ": command not found\n", 20);
		}
		return (127);
	}
	if (executable_path == (char *)-1)
		return (ctx->exit_status);
	env_array = convert_env_to_array(ctx->env, ctx->gc);
	setup_exec_signals();
	pid = fork();
	if (pid == 0)
	{

		execve(executable_path, args, env_array);
		exit(127);
	}
	add_child_pid(ctx, pid);
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
	int	i;

	if (!cmd_node || !ctx)
		return (1);
	if (cmd_node->type == NODE_REDIRECT)
		return (execute_redirection(cmd_node, ctx));
	if (!cmd_node->args || !cmd_node->args[0])
		return (1);
	i = 0;
	while (cmd_node->args[i] && ft_strlen(cmd_node->args[i]) == 0)
		i++;
	if (!cmd_node->args[i])
		return (0);
	if (i > 0)
	{
		int j = 0;
		while (cmd_node->args[i + j])
		{
			cmd_node->args[j] = cmd_node->args[i + j];
			j++;
		}
		cmd_node->args[j] = NULL;
	}
	if (cmd_node->right && cmd_node->right->type == NODE_REDIRECT)
	{
		t_ast_node *redirect_node = cmd_node->right;
		t_ast_node *last_heredoc = NULL;
		while (redirect_node)
		{
			if (redirect_node->redirect_type == REDIRECT_HEREDOC)
				last_heredoc = redirect_node;
			redirect_node = redirect_node->right;
		}
		redirect_node = cmd_node->right;
		while (redirect_node)
		{
			if (redirect_node->redirect_type == REDIRECT_HEREDOC && redirect_node != last_heredoc)
				execute_heredoc_consume_only(redirect_node, ctx);
			redirect_node = redirect_node->right;
		}
		redirect_node = cmd_node->right;
		int redirect_error = 0;
		while (redirect_node)
		{
			if (redirect_node->redirect_type != REDIRECT_HEREDOC)
			{
				if (execute_redirection(redirect_node, ctx) != 0)
				{
					redirect_error = 1;
					break ;
				}
			}
			else if (redirect_node == last_heredoc)
			{
				if (execute_redirection(redirect_node, ctx) != 0)
				{
					redirect_error = 1;
					break;
				}
			}
			redirect_node = redirect_node->right;
		}
		if (redirect_error)
		{
			restore_std_fds(ctx);
			return (1);
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
