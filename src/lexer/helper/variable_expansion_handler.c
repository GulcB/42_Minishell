/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_handler.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 15:30:00 by mdivan            #+#    #+#             */
/*   Updated: 2025/08/04 20:05:13 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "lexer.h"

void	init_expansion_params(t_expansion_params *params, char *result,
		const char *str, struct s_exec_context *ctx)
{
	params->result = result;
	params->str = str;
	params->ctx = ctx;
}

char	*handle_escaped_char(t_char_handler_params params)
{
	t_expansion_params	exp_params;

	exp_params.result = params.result;
	exp_params.str = params.str;
	exp_params.start = *params.start;
	exp_params.ctx = params.ctx;
	params.result = handle_escaped_dollar(exp_params, *params.i);
	*params.i += 2;
	*params.start = *params.i;
	return (params.result);
}

char	*handle_var_expansion(t_char_handler_params params)
{
	t_expansion_params	exp_params;

	exp_params.result = params.result;
	exp_params.str = params.str;
	exp_params.start = *params.start;
	exp_params.ctx = params.ctx;
	params.result = handle_variable_expansion(exp_params, params.i);
	*params.start = *params.i;
	return (params.result);
}

void	init_handler_params(t_char_handler_params *handler_params,
		t_position_params pos_params)
{
	handler_params->result = pos_params.result;
	handler_params->str = pos_params.str;
	handler_params->start = pos_params.start;
	handler_params->i = pos_params.i;
	handler_params->ctx = pos_params.ctx;
}

char	*process_char_at_position(t_position_params pos_params)
{
	t_char_handler_params	handler_params;

	if (pos_params.str[*pos_params.i] == '\x01'
		&& pos_params.str[*pos_params.i + 1] == '$')
	{
		init_handler_params(&handler_params, pos_params);
		return (handle_escaped_char(handler_params));
	}
	else if (pos_params.str[*pos_params.i] == '$'
		&& pos_params.str[*pos_params.i + 1]
		&& !is_escaped(pos_params.str, *pos_params.i))
	{
		init_handler_params(&handler_params, pos_params);
		return (handle_var_expansion(handler_params));
	}
	else
	{
		(*pos_params.i)++;
		return (pos_params.result);
	}
}
