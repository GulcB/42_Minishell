/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:00:43 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 21:04:22 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

static int	is_numeric(const char *s)
{
	int		i;
	char	*trimmed;

	if (!s || !*s)
		return (0);
	trimmed = trim_quotes(s);
	if (!trimmed || !*trimmed)
	{
		free(trimmed);
		return (0);
	}
	i = 0;
	if (trimmed[0] == '-' || trimmed[0] == '+')
		i++;
	if (!trimmed[i] || !check_digits_only(trimmed, i))
	{
		free(trimmed);
		return (0);
	}
	free(trimmed);
	return (1);
}

static int	to_int_and_check_overflow(const char *s, int *out)
{
	int		sign;
	char	*trimmed;

	trimmed = trim_quotes(s);
	if (!trimmed)
		return (1);
	sign = 1;
	s = trimmed;
	if (*s == '-')
	{
		sign = -1;
		s++;
	}
	else if (*s == '+')
		s++;
	if (parse_and_convert(s, sign, out))
	{
		free(trimmed);
		return (1);
	}
	free(trimmed);
	return (0);
}

static void	handle_invalid_argument(t_builtin_cmd *cmd)
{
	write(STDERR_FILENO, "minishell: exit: ", 17);
	write(STDERR_FILENO, cmd->args[1], ft_strlen(cmd->args[1]));
	write(STDERR_FILENO, ": numeric argument required\n", 28);
	*cmd->exit_status = 2;
}

static unsigned char	calculate_exit_code(int code)
{
	if (code < 0)
		return ((unsigned char)(((code % 256) + 256) % 256));
	else
		return ((unsigned char)(code % 256));
}

int	builtin_exit(t_builtin_cmd *cmd)
{
	int				code;
	unsigned char	final_code;

	write(STDERR_FILENO, "exit\n", 5);
	if (cmd->args[1])
	{
		if (!is_numeric(cmd->args[1]) || to_int_and_check_overflow(cmd->args[1],
				&code))
		{
			handle_invalid_argument(cmd);
			return (SHELL_EXIT);
		}
		if (cmd->args[2])
		{
			write(STDERR_FILENO, "minishell: exit: too many arguments\n", 37);
			*cmd->exit_status = 1;
			return (1);
		}
		final_code = calculate_exit_code(code);
		*cmd->exit_status = final_code;
	}
	return (SHELL_EXIT);
}
