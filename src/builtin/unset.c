/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:01:03 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/30 14:23:19 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

static int	is_valid_identifier(const char *key)
{
	int	i;

	if (!key || !(ft_isalpha(key[0]) || key[0] == '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	env_remove(struct s_env **env, const char *key, t_gc *gc)
{
	struct s_env	*prev;
	struct s_env	*curr;

	if (!env || !*env || !key)
		return (1);
	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (ft_strncmp(curr->key, key, ft_strlen(curr->key) + 1) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			gc_free(gc, curr->key);
			if (curr->value)
				gc_free(gc, curr->value);
			gc_free(gc, curr);
			return (0);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}

int	builtin_unset(t_builtin_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_identifier(cmd->args[i]))
		{
			*cmd->exit_status = 1;
		}
		else
			env_remove(&cmd->env, cmd->args[i], cmd->gc);
		i++;
	}
	return (0);
}
