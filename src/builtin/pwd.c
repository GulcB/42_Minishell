/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 21:57:29 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/31 15:18:02 by gbodur           ###   ########.fr       */
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
		write(STDERR_FILENO, "minishell: pwd: ", 16);
		write(STDERR_FILENO, "error retrieving current directory\n", 36);
		*cmd->exit_status = 1;
		return (1);
	}
	write(STDOUT_FILENO, current_dir, ft_strlen(current_dir));
	ft_putchar_fd('\n', STDOUT_FILENO);
	free(current_dir);
	*cmd->exit_status = 0;
	return (0);
}
