/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 21:57:29 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/23 19:36:56 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

int	builtin_pwd(t_builtin_cmd *cmd)
{
	char	*current_dir;

	(void)cmd->args;
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
	{
		ft_putstr_fd("minishell: pwd: ", STDERR_FILENO);
		ft_putstr_fd("error retrieving current directory\n", STDERR_FILENO);
		*cmd->exit_status = 1;
		return (1);
	}
	ft_putstr_fd(current_dir, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	free(current_dir); // veya gc_free(cmd->gc, current_dir); istersen
	*cmd->exit_status = 0;
	return (0);
}
