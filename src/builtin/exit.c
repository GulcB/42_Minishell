/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:00:43 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/31 18:57:04 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h" 
#include "executor.h"
#include <string.h>

static int	is_numeric(const char *s)
{
	int	i;
	char *trimmed;

	if (!s || !*s)
		return (0);
	
	// Remove quotes if present
	if ((s[0] == '"' && s[strlen(s) - 1] == '"') || 
		(s[0] == '\'' && s[strlen(s) - 1] == '\''))
	{
		trimmed = strndup(s + 1, strlen(s) - 2);
		if (!trimmed)
			return (0);
	}
	else
		trimmed = strdup(s);
	
	if (!trimmed || !*trimmed)
	{
		free(trimmed);
		return (0);
	}
	
	i = 0;
	if (trimmed[0] == '-' || trimmed[0] == '+')
		i++;
	
	if (!trimmed[i]) // Only sign character
	{
		free(trimmed);
		return (0);
	}
	
	while (trimmed[i])
	{
		if (!ft_isdigit(trimmed[i]))
		{
			free(trimmed);
			return (0);
		}
		i++;
	}
	free(trimmed);
	return (1);
}

static int	to_int_and_check_overflow(const char *s, int *out)
{
	long	result;
	int		sign;
	char	*trimmed;

	// Remove quotes if present
	if ((s[0] == '"' && s[strlen(s) - 1] == '"') || 
		(s[0] == '\'' && s[strlen(s) - 1] == '\''))
		trimmed = strndup(s + 1, strlen(s) - 2);
	else
		trimmed = strdup(s);
	
	if (!trimmed)
		return (1);
	
	result = 0;
	sign = 1;
	s = trimmed;
	
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
		{
			free(trimmed);
			return (1);
		}
		result = result * 10 + (*s - '0');
		if (sign == 1 && result > INT_MAX)
		{
			free(trimmed);
			return (1);
		}
		if (sign == -1 && -result < INT_MIN)
		{
			free(trimmed);
			return (1);
		}
		s++;
	}
	*out = (int)(sign * result);
	free(trimmed);
	return (0);
}

int	builtin_exit(t_builtin_cmd *cmd)
{
	int	code;
	unsigned char final_code;

	write(STDERR_FILENO, "exit\n", 5);
	if (cmd->args[1])
	{
		if (!is_numeric(cmd->args[1])
			|| to_int_and_check_overflow(cmd->args[1], &code))
		{
			write(STDERR_FILENO, "minishell: exit: ", 17);
			write(STDERR_FILENO, cmd->args[1], ft_strlen(cmd->args[1]));
			write(STDERR_FILENO, ": numeric argument required\n", 28);
			*cmd->exit_status = 2;
			return (SHELL_EXIT);
		}
		if (cmd->args[2])
		{
			write(STDERR_FILENO, "minishell: exit: too many arguments\n", 37);
			*cmd->exit_status = 1;
			return (1);
		}
		// Handle negative numbers properly for modulo
		if (code < 0)
			final_code = (unsigned char)(((code % 256) + 256) % 256);
		else
			final_code = (unsigned char)(code % 256);
		*cmd->exit_status = final_code;
	}
	return (SHELL_EXIT);
}
