/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 21:57:09 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/22 19:16:29 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h" 
#include "executor.h"

static char	*get_target_directory(t_builtin_cmd *cmd)
{
	if (!cmd->args[1])
		return (env_get(cmd->env, "HOME"));
	if (ft_strncmp(cmd->args[1], "-", 2) == 0
		&& ft_strlen(cmd->args[1]) == 1)
		return (env_get(cmd->env, "OLDPWD"));
	return (cmd->args[1]);
}

static void	update_pwd_variables(t_builtin_cmd *cmd, char *old_pwd)
{
	char	*new_pwd;
	t_env	*oldpwd_node;
	t_env	*pwd_node;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return ;
	oldpwd_node = find_env_node(cmd->env, "OLDPWD");
	if (oldpwd_node)
	{
		gc_free(cmd->gc, oldpwd_node->value);
		oldpwd_node->value = gc_strdup(cmd->gc, old_pwd);
	}
	pwd_node = find_env_node(cmd->env, "PWD");
	if (pwd_node)
	{
		gc_free(cmd->gc, pwd_node->value);
		pwd_node->value = gc_strdup(cmd->gc, new_pwd);
	}
	free(new_pwd);
}

static int	handle_cd_minus(t_builtin_cmd *cmd)
{
	char	*oldpwd;

	oldpwd = env_get(cmd->env, "OLDPWD");
	if (!oldpwd)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		return (1);
	}
	ft_putstr_fd(oldpwd, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

static int	handle_too_many_args(char **args)
{
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	builtin_cd(t_builtin_cmd *cmd)
{
	char	*target_dir;
	char	*old_pwd;

	if (!cmd->args || !cmd->env || !cmd->exit_status || !cmd->gc)
		return (1);
	if (handle_too_many_args(cmd->args))
		return (1);
	target_dir = get_target_directory(cmd);
	if (!target_dir)
		return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	old_pwd = getcwd(NULL, 0);
	if (chdir(target_dir) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(target_dir, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		free(old_pwd);
		return (1);
	}
	if (cmd->args[1] && ft_strncmp(cmd->args[1], "-", 2) == 0)
		handle_cd_minus(cmd);
	update_pwd_variables(cmd, old_pwd);
	free(old_pwd);
	return (0);
}
