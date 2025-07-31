/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 22:18:49 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/31 15:32:44 by gbodur           ###   ########.fr       */
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

static int	execute_basic_builtins(t_builtin_cmd *cmd)
{
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (builtin_echo(cmd));
	if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (builtin_pwd(cmd));
	if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (builtin_env(cmd));
	if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (builtin_cd(cmd));
	if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (builtin_export(cmd));
	if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (builtin_unset(cmd));
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		return (builtin_exit(cmd));
	return (-1);
}

int	execute_builtin_dispatcher(char **args, struct s_exec_context *ctx)
{
	t_builtin_cmd	cmd;
	int				result;

	if (!args || !args[0] || !ctx)
		return (1);
	cmd.args = args;
	cmd.env = ctx->env;
	cmd.gc = ctx->gc;
	cmd.exit_status = &ctx->exit_status;
	result = execute_basic_builtins(&cmd);
	if (result != -1)
		return (result);
	if (ft_strncmp(args[0], "exit", 5) == 0 && ft_strlen(args[0]) == 4)
		return (builtin_exit(&cmd));
	write(STDERR_FILENO, "minishell: builtin not implemented: ", 37);
	write(STDERR_FILENO, args[0], ft_strlen(args[0]));
	ft_putchar_fd('\n', STDERR_FILENO);
	*cmd.exit_status = 1;
	return (1);
}
