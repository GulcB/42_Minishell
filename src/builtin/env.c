/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 21:57:12 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/23 19:37:47 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

static void	print_env_variable(t_env *node)
{
	if (!node->key)
		return ;
	ft_putstr_fd(node->key, STDOUT_FILENO);
	ft_putchar_fd('=', STDOUT_FILENO);
	if (node->value)
		ft_putstr_fd(node->value, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

int	builtin_env(t_builtin_cmd *cmd)
{
	t_env	*current;

	if (cmd->args[1])
	{
		ft_putstr_fd("minishell: env: too many arguments\n", STDERR_FILENO);
		*cmd->exit_status = 1;
		return (1);
	}
	current = cmd->env;
	while (current)
	{
		if (current->value)
			print_env_variable(current);
		current = current->next;
	}
	*cmd->exit_status = 0;
	return (0);
}
