/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_env.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:40:39 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/27 18:43:56 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "executor.h"

char	*search_env_var(const char *var_name, struct s_exec_context *ctx)
{
	void	*current;
	size_t	var_len;

	if (!ctx || !ctx->env || !var_name)
		return (NULL);
	current = ctx->env;
	var_len = ft_strlen(var_name);
	while (current)
	{
		if (ft_strncmp(((struct s_env *)current)->key, var_name, var_len
				+ 1) == 0
			&& ft_strlen(((struct s_env *)current)->key) == var_len)
		{
			if (((struct s_env *)current)->value)
				return (ft_strdup(((struct s_env *)current)->value));
			return (ft_strdup(""));
		}
		current = ((struct s_env *)current)->next;
	}
	return (NULL);
}

char	*get_env_value(const char *var_name, t_exec_context *ctx)
{
	char	*result;
	char	*env_val;

	if (!var_name)
		return (ft_strdup(""));
		
	/* Check if this is marked as literal content from $"..." */
	if (ft_strncmp(var_name, "LITERAL:", 8) == 0)
		return (ft_strdup(var_name + 8)); /* Return content after "LITERAL:" */
		
	result = get_special_var_value(var_name, ctx);
	if (result)
		return (result);
	result = search_env_var(var_name, ctx);
	if (result)
		return (result);
	env_val = getenv(var_name);
	if (env_val)
		return (ft_strdup(env_val));
	return (ft_strdup(""));
}

char	*join_and_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	return (result);
}

char	*add_literal_part(char *result, const char *str, int start, int end)
{
	char	*temp;
	char	*new_result;

	if (end <= start)
		return (result);
	temp = ft_substr(str, start, end - start);
	new_result = join_and_free(result, temp);
	return (new_result);
}
