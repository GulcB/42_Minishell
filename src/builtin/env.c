/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 21:57:12 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/17 22:18:18 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/builtin.h"

static void	print_env_variable(t_env *env_node)
{
	if (!env_node->key)
		return ;
	ft_putstr_fd(env_node->key, STDOUT_FILENO);
	if (env_node->value)
	{
		ft_putchar_fd('=', STDOUT_FILENO);
		ft_putstr_fd(env_node->value, STDOUT_FILENO);
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
}

static int	validate_env_args(char **args)
{
	if (!args)
		return (0);
	if (args[1])
		return (0);
	return (1);
}

int	builtin_env(char **args, t_env *env)
{
	t_env	*current;

	if (!validate_env_args(args))
	{
		ft_putstr_fd("minishell: env: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	current = env;
	while (current)
	{
		print_env_variable(current);
		current = current->next;
	}
	return (0);
}
