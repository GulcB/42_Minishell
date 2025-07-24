/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:00:43 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/24 21:35:45 by gbodur           ###   ########.fr       */
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

	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (cmd->args[1])
	{
		if (!is_numeric(cmd->args[1])
			|| to_int_and_check_overflow(cmd->args[1], &code))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(cmd->args[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			*cmd->exit_status = 255;
			return (SHELL_EXIT);
		}
		if (cmd->args[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n",
				STDERR_FILENO);
			*cmd->exit_status = 1;
			return (1);
		}
		*cmd->exit_status = code % 256;
	}
	return (SHELL_EXIT);
}
