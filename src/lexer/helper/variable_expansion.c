/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 23:58:34 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/19 11:57:11 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/lexer.h"

char	*extract_simple_var(const char *str, int *consumed)
{
	int		i;
	int		start;

	i = 1;
	start = i;
	while (str[i] && (ft_isalpha(str[i]) || ft_isdigit(str[i])
			|| str[i] == '_'))
		i++;
	if (i == start)
		return (NULL);
	*consumed = i;
	return (ft_substr(str, start, i - start));
}

char	*extract_braced_var(const char *str, int *consumed)
{
	int		i;
	int		start;

	i = 2;
	start = i;
	while (str[i] && str[i] != '}')
		i++;
	if (str[i] != '}')
		return (NULL);
	*consumed = i + 1;
	return (ft_substr(str, start, i - start));
}

char	*extract_special_var(const char *str, int *consumed)
{
	if (str[1] == '?')
	{
		*consumed = 2;
		return (ft_strdup("?"));
	}
	return (NULL);
}

char	*extract_var_name(const char *str, int *consumed)
{
	char	*result;

	*consumed = 1;
	result = extract_special_var(str, consumed);
	if (result)
		return (result);
	if (str[1] == '{')
		return (extract_braced_var(str, consumed));
	return (extract_simple_var(str, consumed));
}

char	*get_special_var_value(const char *var_name, t_exec_context *ctx)
{
	if (ft_strncmp(var_name, "?", 2) == 0 && ft_strlen(var_name) == 1)
	{
		if (ctx)
			return (ft_itoa(ctx->exit_status));
		return (ft_itoa(0));
	}
	return (NULL);
}
