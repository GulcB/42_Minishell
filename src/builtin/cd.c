/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 21:57:09 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/22 14:14:22 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h" 
#include "executor.h"

static char	*get_target_directory(char **args, struct s_exec_context *ctx)
{
	if (!args[1])
		return (env_get(ctx->env, "HOME"));
	if (ft_strncmp(args[1], "-", 2) == 0 && ft_strlen(args[1]) == 1)
		return (env_get(ctx->env, "OLDPWD"));
	return (args[1]);
}

static void	update_pwd_variables(struct s_exec_context *ctx, char *old_pwd)
{
	char	*new_pwd;
	t_env	*oldpwd_node;
	t_env	*pwd_node;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return ;
	oldpwd_node = find_env_node(ctx->env, "OLDPWD");
	if (oldpwd_node)
	{
		free(oldpwd_node->value);
		oldpwd_node->value = ft_strdup(old_pwd);
	}
	pwd_node = find_env_node(ctx->env, "PWD");
	if (pwd_node)
	{
		free(pwd_node->value);
		pwd_node->value = ft_strdup(new_pwd);
	}
	free(new_pwd);
}

static int	handle_cd_minus(struct s_exec_context *ctx)
{
	char	*oldpwd;

	oldpwd = env_get(ctx->env, "OLDPWD");
	if (!oldpwd)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		return (1);
	}
	ft_putstr_fd(oldpwd, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

int	builtin_cd(char **args, struct s_exec_context *ctx)
{
	char	*target_dir;
	char	*old_pwd;

	if (!args || !ctx)
		return (1);
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	target_dir = get_target_directory(args, ctx);
	if (!target_dir)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		return (1);
	}
	old_pwd = getcwd(NULL, 0);
	if (chdir(target_dir) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(target_dir, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		free(old_pwd);
		return (1);
	}
	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
		handle_cd_minus(ctx);
	update_pwd_variables(ctx, old_pwd);
	free(old_pwd);
	return (0);
}
