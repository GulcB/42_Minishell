/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_extraction.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 16:00:00 by mdivan            #+#    #+#             */
/*   Updated: 2025/08/04 20:09:57 by mdivan           ###   ########.fr       */
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

static char	*handle_quoted_variable(t_gc *gc, const char *str, int *consumed)
{
	int		i;
	char	*content;
	char	*literal_marker;

	i = 2;
	while (str[i] && str[i] != '"')
		i++;
	if (str[i] == '"')
	{
		*consumed = i + 1;
		content = gc_substr(gc, str, 2, i - 2);
		literal_marker = gc_strjoin(gc, "LITERAL:", content);
		return (literal_marker);
	}
	return (NULL);
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
		return (handle_quoted_variable(gc, str, consumed));
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
