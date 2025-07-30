/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:00:43 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/30 14:16:57 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h" 
#include "executor.h"

static int	is_numeric(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	if (s[0] == '-' || s[0] == '+')
		s++;
	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	to_int_and_check_overflow(const char *s, int *out)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	if (*s == '-')
	{
		sign = -1;
		s++;
	}
	else if (*s == '+')
		s++;
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (1);
		result = result * 10 + (*s - '0');
		if (sign == 1 && result > INT_MAX)
			return (1);
		if (sign == -1 && -result < INT_MIN)
			return (1);
		s++;
	}
	*out = (int)(sign * result);
	return (0);
}

int	builtin_exit(t_builtin_cmd *cmd)
{
	int	code;

	write(STDERR_FILENO, "exit\n", 5);
	if (cmd->args[1])
	{
		if (!is_numeric(cmd->args[1])
			|| to_int_and_check_overflow(cmd->args[1], &code))
		{
			write(STDERR_FILENO, "minishell: exit: ", 17);
			write(STDERR_FILENO, cmd->args[1], ft_strlen(cmd->args[1]));
			write(STDERR_FILENO, ": numeric argument required\n", 28);
			*cmd->exit_status = 255;
			return (SHELL_EXIT);
		}
		if (cmd->args[2])
		{
			write(STDERR_FILENO, "minishell: exit: too many arguments\n", 37);
			*cmd->exit_status = 1;
			return (1);
		}
		*cmd->exit_status = code % 256;
	}
	return (SHELL_EXIT);
}
