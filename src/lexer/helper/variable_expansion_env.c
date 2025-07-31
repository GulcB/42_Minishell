/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_env.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:40:39 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/31 14:33:36 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "executor.h"

char	*search_env_var(const char *var_name, t_exec_context *ctx)
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
				return (gc_strdup(ctx->gc, ((struct s_env *)current)->value));
			return (gc_strdup(ctx->gc, ""));
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
		return (gc_strdup(ctx->gc, ""));
	if (ft_strncmp(var_name, "LITERAL:", 8) == 0)
		return (gc_strdup(ctx->gc, var_name + 8));
	result = get_special_var_value(var_name, ctx);
	if (result)
		return (result);
	result = search_env_var(var_name, ctx);
	if (result)
		return (result);
	env_val = getenv(var_name);
	if (env_val)
		return (gc_strdup(ctx->gc, env_val));
	return (gc_strdup(ctx->gc, ""));
}

char	*add_literal_part(t_exec_context *ctx, char *result, const char *str, int start, int end)
{
	char	*temp;
	char	*new_result;

	if (end <= start)
		return (result);
	temp = gc_substr(ctx->gc, str, start, end - start);
	new_result = gc_strjoin(ctx->gc, result, temp);
	return (new_result);
}
