/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 21:57:07 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/30 14:16:57 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

static int	is_valid_n_flag(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	if (arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (i >= 2);
}

static int	count_n_flags(char **args)
{
	int	i;
	int	count;

	i = 1;
	count = 0;
	while (args[i] && is_valid_n_flag(args[i]))
	{
		count++;
		i++;
	}
	return (count);
}

static void	print_args_with_spaces(char **args, int start_index)
{
	int	i;

	i = start_index;
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
}

int	builtin_echo(t_builtin_cmd *cmd)
{
	int	newline_flag;
	int	n_flag_count;
	int	start_index;

	if (!cmd->args || !cmd->args[0])
		return (1);
	newline_flag = 1;
	n_flag_count = count_n_flags(cmd->args);
	if (n_flag_count > 0)
		newline_flag = 0;
	start_index = 1 + n_flag_count;
	print_args_with_spaces(cmd->args, start_index);
	if (newline_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	*cmd->exit_status = 0;
	return (0);
}
