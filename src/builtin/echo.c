/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 21:57:07 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/17 22:17:52 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/builtin.h"

static int	is_valid_n_flag(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (i > 2);
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
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
}

int	builtin_echo(char **args)
{
	int	newline_flag;
	int	n_flag_count;
	int	start_index;

	if (!args || !args[0])
		return (1);
	newline_flag = 1;
	n_flag_count = count_n_flags(args);
	if (n_flag_count > 0)
		newline_flag = 0;
	start_index = 1 + n_flag_count;
	print_args_with_spaces(args, start_index);
	if (newline_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
