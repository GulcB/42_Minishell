/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 21:57:29 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/17 22:18:04 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/builtin.h"

static void	print_error_and_exit(void)
{
	ft_putstr_fd("minishell: pwd: ", STDERR_FILENO);
	ft_putstr_fd("error retrieving current directory\n", STDERR_FILENO);
}

static char	*get_current_directory(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	return (cwd);
}

static void	print_directory(char *directory)
{
	ft_putstr_fd(directory, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

int	builtin_pwd(char **args)
{
	char	*current_dir;

	(void)args;
	current_dir = get_current_directory();
	if (!current_dir)
	{
		print_error_and_exit();
		return (1);
	}
	print_directory(current_dir);
	free(current_dir);
	return (0);
}
