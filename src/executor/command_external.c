/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 22:18:00 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 22:19:59 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	**convert_env_to_array(t_env *env, t_gc *gc)
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

int	handle_executable_not_found(char **args)
{
	if (!ft_strchr(args[0], '/'))
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, args[0], ft_strlen(args[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
	}
	return (127);
}

int	execute_child_process(char *executable_path, char **args,
		char **env_array)
{
	pid_t	pid;

	setup_exec_signals();
	pid = fork();
	if (pid == 0)
	{
		execve(executable_path, args, env_array);
		exit(127);
	}
	return (pid);
}

int	execute_external_command(char **args, t_exec_context *ctx)
{
	char	*executable_path;
	char	**env_array;
	pid_t	pid;

	if (!args || !args[0] || ft_strlen(args[0]) == 0)
		return (0);
	executable_path = resolve_executable(ctx, args[0], ctx->env);
	if (!executable_path)
		return (handle_executable_not_found(args));
	if (executable_path == (char *)-1)
		return (ctx->exit_status);
	env_array = convert_env_to_array(ctx->env, ctx->gc);
	pid = execute_child_process(executable_path, args, env_array);
	add_child_pid(ctx, pid);
	return (wait_for_children(ctx));
}
