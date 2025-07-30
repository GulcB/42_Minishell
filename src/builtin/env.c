/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 21:57:12 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/30 14:16:57 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

static void	print_env_variable(t_env *node)
{
	if (!node->key)
		return ;
	write(STDOUT_FILENO, node->key, ft_strlen(node->key));
	ft_putchar_fd('=', STDOUT_FILENO);
	if (node->value)
		write(STDOUT_FILENO, node->value, ft_strlen(node->value));
	ft_putchar_fd('\n', STDOUT_FILENO);
}

int	builtin_env(t_builtin_cmd *cmd)
{
	t_env	*current;

	if (cmd->args[1])
	{
		write(STDERR_FILENO, "minishell: env: too many arguments\n", 36);
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
