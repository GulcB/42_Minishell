/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 22:18:49 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/17 22:45:02 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/builtin.h"

static int	check_basic_builtins(const char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	return (0);
}

static int	check_advanced_builtins(const char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
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

static int	execute_basic_builtins(char **args, t_exec_context *ctx)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd(args));
	if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env(args, ctx->env));
	return (-1);
}

int	execute_builtin_dispatcher(char **args, t_exec_context *ctx)
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