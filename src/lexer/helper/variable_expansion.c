/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 23:58:34 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/15 00:02:01 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/lexer.h"

static char	*extract_simple_var(const char *str, int *consumed)
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

static char	*extract_braced_var(const char *str, int *consumed)
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

static char	*extract_special_var(const char *str, int *consumed)
{
	if (str[1] == '?')
	{
		*consumed = 2;
		return (ft_strdup("?"));
	}
	return (NULL);
}

static char	*extract_var_name(const char *str, int *consumed)
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

static char	*get_special_var_value(const char *var_name, t_exec_context *ctx)
{
	if (ft_strcmp(var_name, "?") == 0)
	{
		if (ctx)
			return (ft_itoa(ctx->exit_status));
		return (ft_itoa(0));
	}
	return (NULL);
}

static char	*search_env_var(const char *var_name, t_exec_context *ctx)
{
	t_env	*current;

	if (!ctx || !ctx->env)
		return (NULL);
	current = ctx->env;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
		{
			if (current->value)
				return (ft_strdup(current->value));
			return (ft_strdup(""));
		}
		current = current->next;
	}
	return (NULL);
}

static char	*get_env_value(const char *var_name, t_exec_context *ctx)
{
	char	*result;
	char	*env_val;

	if (!var_name)
		return (ft_strdup(""));
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

static char	*join_and_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	gc_free(s1);
	gc_free(s2);
	return (result);
}

static char	*add_literal_part(char *result, const char *str, int start, int end)
{
	char	*temp;
	char	*new_result;

	if (end <= start)
		return (result);
	temp = ft_substr(str, start, end - start);
	new_result = join_and_free(result, temp);
	return (new_result);
}

static char	*process_variable(char *result, const char *str, int *i,
		t_exec_context *ctx)
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
		gc_free(var_name);
		*i += consumed;
		return (new_result);
	}
	*i += 1;
	return (result);
}

char	*expand_variables(const char *str, t_exec_context *ctx)
{
	char	*result;
	int		i;
	int		start;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
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

char	*process_token_expansion(t_token *token, t_exec_context *ctx)
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
