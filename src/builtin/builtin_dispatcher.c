/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 22:18:49 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/19 16:11:33 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h" 
#include "executor.h"

static int	check_basic_builtins(const char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0 && ft_strlen(cmd) == 4)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0 && ft_strlen(cmd) == 3)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0 && ft_strlen(cmd) == 3)
		return (1);
	return (0);
}

static int	check_advanced_builtins(const char *cmd)
{
	if (ft_strncmp(cmd, "cd", 3) == 0 && ft_strlen(cmd) == 2)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0 && ft_strlen(cmd) == 6)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0 && ft_strlen(cmd) == 5)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0 && ft_strlen(cmd) == 4)
		return (1);
	return (0);
}

int	is_builtin_command(const char *cmd)
{
	if (!cmd)
		return (0);
	if (check_basic_builtins(cmd))
		return (1);
	if (check_advanced_builtins(cmd))
		return (1);
	return (0);
}

static int	execute_basic_builtins(char **args, struct s_exec_context *ctx)
{
	if (ft_strncmp(args[0], "echo", 5) == 0 && ft_strlen(args[0]) == 4)
		return (builtin_echo(args));
	if (ft_strncmp(args[0], "pwd", 4) == 0 && ft_strlen(args[0]) == 3)
		return (builtin_pwd(args));
	if (ft_strncmp(args[0], "env", 4) == 0 && ft_strlen(args[0]) == 3)
		return (builtin_env(args, ctx->env));
	return (-1);
}

int	execute_builtin_dispatcher(char **args, struct s_exec_context *ctx)
{
	int	result;

	if (!args || !args[0] || !ctx)
		return (1);
	result = execute_basic_builtins(args, ctx);
	if (result != -1)
		return (result);
	ft_putstr_fd("minishell: builtin not implemented: ", STDERR_FILENO);
	ft_putstr_fd(args[0], STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	return (1);
}
