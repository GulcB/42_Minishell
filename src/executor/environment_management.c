/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_management.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:48:07 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 21:36:54 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_env	*find_env_node(t_env *env, const char *key)
{
	t_env	*current;
	size_t	key_len;

	if (!env || !key)
		return (NULL);
	current = env;
	key_len = ft_strlen(key);
	while (current)
	{
		if (ft_strncmp(current->key, key, key_len + 1) == 0
			&& ft_strlen(current->key) == key_len)
			return (current);
		current = current->next;
	}
	return (NULL);
}

char	*env_get(t_env *env, const char *key)
{
	t_env	*node;

	node = find_env_node(env, key);
	if (node)
		return (node->value);
	return (NULL);
}

void	env_add(t_gc *gc, t_env **env, const char *key, const char *value)
{
	t_env	*new_node;

	new_node = gc_malloc(gc, sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = gc_strdup(gc, key);
	if (value)
		new_node->value = gc_strdup(gc, value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	if (!*env)
	{
		*env = new_node;
		return ;
	}
	while ((*env)->next)
		env = &(*env)->next;
	(*env)->next = new_node;
}

void	env_set(t_env *env, const char *key, const char *value, t_gc *gc)
{
	t_env	*existing;
	t_env	*head;

	if (!env || !key)
		return ;
	head = env;
	existing = find_env_node(head, key);
	if (existing)
	{
		if (existing->value)
			gc_free(gc, existing->value);
		existing->value = gc_strdup(gc, value);
	}
	else
	{
		env_add(gc, &head, key, value);
	}
}
