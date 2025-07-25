/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:00:54 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/23 20:06:55 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

static int	is_valid_key(const char *key)
{
	int	i;

	if (!key || !(ft_isalpha(key[0]) || key[0] == '_'))
		return (0);
	i = 1;
	while (key[i] && key[i] != '=')
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	export_variable(t_builtin_cmd *cmd, const char *arg)
{
	char	*eq;
	char	*key;
	char	*value;
	t_env	*node;

	eq = ft_strchr(arg, '=');
	if (!eq)
	{
		if (!find_env_node(cmd->env, arg))
			env_add(cmd->gc, &cmd->env, arg, NULL);
		return ;
	}
	key = gc_substr(cmd->gc, arg, 0, eq - arg);
	value = gc_strdup(cmd->gc, eq + 1);
	node = find_env_node(cmd->env, key);
	if (node)
	{
		gc_free(cmd->gc, node->value);
		node->value = value;
	}
	else
		env_add(cmd->gc, &cmd->env, key, value);
}

static void	print_env_line(const char *entry)
{
	int	i;

	i = 0;
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	while (entry[i] && entry[i] != '=')
	{
		ft_putchar_fd(entry[i], STDOUT_FILENO);
		i++;
	}
	if (entry[i] == '=')
	{
		ft_putchar_fd('=', STDOUT_FILENO);
		ft_putchar_fd('\"', STDOUT_FILENO);
		ft_putstr_fd((char *)(&entry[i + 1]), STDOUT_FILENO);
		ft_putchar_fd('\"', STDOUT_FILENO);
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
}

int	print_sorted_env(t_env *env)
{
	int		count;
	char	**env_arr;
	int		i;

	count = env_count(env);
	env_arr = env_to_array(env, count);
	if (!env_arr)
		return (1);
	sort_env_array(env_arr, count);
	i = 0;
	while (env_arr[i])
	{
		print_env_line(env_arr[i]);
		free(env_arr[i]);
		i++;
	}
	free(env_arr);
	return (0);
}

int	builtin_export(t_builtin_cmd *cmd)
{
	int	i;

	if (!cmd->args[1])
		return (print_sorted_env(cmd->env));
	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_key(cmd->args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			*cmd->exit_status = 1;
		}
		else
			export_variable(cmd, cmd->args[i]);
		i++;
	}
	return (0);
}
