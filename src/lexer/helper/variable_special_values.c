/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_special_values.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 16:00:00 by mdivan            #+#    #+#             */
/*   Updated: 2025/08/04 20:09:57 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "executor.h"

static char	*handle_exit_status_var(struct s_exec_context *ctx, t_gc *gc)
{
	char	*temp_str;
	char	*result;

	if (ctx)
	{
		temp_str = ft_itoa(ctx->exit_status);
		result = gc_strdup(gc, temp_str);
		free(temp_str);
		return (result);
	}
	temp_str = ft_itoa(0);
	result = gc_strdup(gc, temp_str);
	free(temp_str);
	return (result);
}

static char	*handle_pid_var(struct s_exec_context *ctx, t_gc *gc)
{
	char	*temp_str;
	char	*result;

	if (ctx)
	{
		temp_str = ft_itoa(getpid());
		result = gc_strdup(gc, temp_str);
		free(temp_str);
		return (result);
	}
	return (NULL);
}

char	*get_special_var_value(const char *var_name, struct s_exec_context *ctx)
{
	if (ft_strncmp(var_name, "?", 2) == 0 && ft_strlen(var_name) == 1)
		return (handle_exit_status_var(ctx, ctx->gc));
	if (ft_strncmp(var_name, "$", 2) == 0 && ft_strlen(var_name) == 1)
		return (handle_pid_var(ctx, ctx->gc));
	if (ft_isdigit(var_name[0]) && ft_strlen(var_name) == 1)
		return (gc_strdup(ctx->gc, ""));
	return (NULL);
}
