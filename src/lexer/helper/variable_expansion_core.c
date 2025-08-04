/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_core.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 15:30:00 by mdivan            #+#    #+#             */
/*   Updated: 2025/08/04 20:05:13 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "lexer.h"

char	*process_variable(char *result, const char *str, int *i,
	struct s_exec_context *ctx)
{
	int		consumed;
	char	*var_name;
	char	*var_value;
	char	*new_result;

	var_name = extract_var_name(ctx->gc, &str[*i], &consumed);
	if (var_name)
	{
		var_value = get_env_value(var_name, ctx);
		new_result = gc_strjoin(ctx->gc, result, var_value);
		*i += consumed;
		return (new_result);
	}
	new_result = gc_strjoin(ctx->gc, result, gc_strdup(ctx->gc, "$"));
	*i += 1;
	return (new_result);
}

int	is_escaped(const char *str, int pos)
{
	int	backslash_count;

	backslash_count = 0;
	while (pos > 0 && str[pos - 1] == '\\')
	{
		backslash_count++;
		pos--;
	}
	return (backslash_count % 2 == 1);
}

char	*handle_escaped_dollar(t_expansion_params params, int i)
{
	t_literal_params	literal_params;

	literal_params.str = params.str;
	literal_params.start = params.start;
	literal_params.end = i;
	params.result = add_literal_part(params.ctx, params.result, literal_params);
	params.result = gc_strjoin(params.ctx->gc, params.result, "$");
	return (params.result);
}

char	*handle_variable_expansion(t_expansion_params params, int *i)
{
	t_literal_params	literal_params;

	literal_params.str = params.str;
	literal_params.start = params.start;
	literal_params.end = *i;
	params.result = add_literal_part(params.ctx, params.result, literal_params);
	params.result = process_variable(params.result, params.str, i, params.ctx);
	return (params.result);
}

char	*add_final_literal(t_expansion_params params, int i)
{
	t_literal_params	literal_params;

	literal_params.str = params.str;
	literal_params.start = params.start;
	literal_params.end = i;
	params.result = add_literal_part(params.ctx, params.result, literal_params);
	return (params.result);
}
