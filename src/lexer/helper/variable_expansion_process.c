/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_process.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:42:26 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/28 11:31:37 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "executor.h"

static char	*process_variable(char *result, const char *str, int *i,
		struct s_exec_context *ctx)
{
	int		consumed;
	char	*var_name;
	char	*var_value;
	char	*new_result;

	var_name = extract_var_name(&str[*i], &consumed);
	if (var_name)
	{
		var_value = get_env_value(var_name, ctx);
		new_result = join_and_free(result, var_value);
		free(var_name);
		*i += consumed;
		return (new_result);
	}
	/* If no valid variable name found, treat $ as literal */
	new_result = join_and_free(result, ft_strdup("$"));
	*i += 1;
	return (new_result);
}

static int	is_escaped(const char *str, int pos)
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

static char	*process_expansion_loop(char *result, const char *str,
		struct s_exec_context *ctx)
{
	int	i;
	int	start;

	i = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == '\x01' && str[i + 1] == '$')
		{
			result = add_literal_part(result, str, start, i);
			result = join_and_free(result, ft_strdup("$"));
			i += 2;
			start = i;
		}
		else if (str[i] == '$' && str[i + 1] && !is_escaped(str, i))
		{
			result = add_literal_part(result, str, start, i);
			result = process_variable(result, str, &i, ctx);
			start = i;
		}
		else
			i++;
	}
	result = add_literal_part(result, str, start, i);
	return (result);
}

char	*expand_variables(const char *str, struct s_exec_context *ctx)
{
	char	*result;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	return (process_expansion_loop(result, str, ctx));
}

char	*process_token_expansion(t_token *token, struct s_exec_context *ctx)
{
	if (!token || !token->value)
		return (NULL);
	if (token->type == TOKEN_DQUOTE)
		return (expand_variables(token->value, ctx));
	if (token->type == TOKEN_SQUOTE)
		return (ft_strdup(token->value));
	if (token->type == TOKEN_WORD)
		return (expand_variables(token->value, ctx));
	return (ft_strdup(token->value));
}
