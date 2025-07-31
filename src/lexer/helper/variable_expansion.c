/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 23:58:34 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/31 14:55:27 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "executor.h"

char	*extract_simple_var(t_gc *gc, const char *str, int *consumed)
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
	return (gc_substr(gc, str, start, i - start));
}

char	*extract_braced_var(t_gc *gc, const char *str, int *consumed)
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
	return (gc_substr(gc, str, start, i - start));
}

char	*extract_special_var(t_gc *gc, const char *str, int *consumed)
{
	if (str[1] == '?')
	{
		*consumed = 2;
		return (gc_strdup(gc, "?"));
	}
	if (str[1] == '$')
	{
		*consumed = 2;
		return (gc_strdup(gc, "$"));
	}
	if (ft_isdigit(str[1]))
	{
		*consumed = 2;
		return (gc_substr(gc, str, 1, 1));
	}
	if (str[1] == '"')
	{
		int i = 2;
		char *content;
		char *literal_marker;
		
		while (str[i] && str[i] != '"')
			i++;
		if (str[i] == '"')
		{
			*consumed = i + 1;
			content = gc_substr(gc, str, 2, i - 2);
			literal_marker = gc_strjoin(gc, "LITERAL:", content);
			return (literal_marker);
		}
	}
	return (NULL);
}

char	*extract_var_name(t_gc *gc, const char *str, int *consumed)
{
	char	*result;

	*consumed = 1;
	result = extract_special_var(gc, str, consumed);
	if (result)
		return (result);
	if (str[1] == '{')
		return (extract_braced_var(gc, str, consumed));
	return (extract_simple_var(gc, str, consumed));
}

char	*get_special_var_value(const char *var_name, struct s_exec_context *ctx)
{
	if (ft_strncmp(var_name, "?", 2) == 0 && ft_strlen(var_name) == 1)
	{
		if (ctx)
			return (ft_itoa(ctx->exit_status));
		return (ft_itoa(0));
	}
	if (ft_strncmp(var_name, "$", 2) == 0 && ft_strlen(var_name) == 1)
	{
		return (ft_itoa(getpid()));
	}
	if (ft_isdigit(var_name[0]) && ft_strlen(var_name) == 1)
	{
		return (gc_strdup(ctx->gc, ""));
	}
	return (NULL);
}
