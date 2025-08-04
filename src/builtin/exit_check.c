/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:57:55 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 18:13:42 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h" 
#include "executor.h"

char	*trim_quotes(const char *s)
{
	size_t	len;

	if (!s || !*s)
		return (NULL);
	len = strlen(s);
	if ((s[0] == '"' && s[len - 1] == '"')
		|| (s[0] == '\'' && s[len - 1] == '\''))
		return (strndup(s + 1, len - 2));
	return (strdup(s));
}

int	check_digits_only(const char *trimmed, int start_idx)
{
	int	i;

	i = start_idx;
	while (trimmed[i])
	{
		if (!ft_isdigit(trimmed[i]))
			return (0);
		i++;
	}
	return (1);
}

int	check_overflow(long result, int sign)
{
	if (sign == 1 && result > INT_MAX)
		return (1);
	if (sign == -1 && (0 - result) < INT_MIN)
		return (1);
	return (0);
}

int	parse_and_convert(const char *str, int sign, int *out)
{
	long	result;

	result = 0;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (1);
		result = result * 10 + (*str - '0');
		if (check_overflow(result, sign))
			return (1);
		str++;
	}
	*out = (int)(sign * result);
	return (0);
}
