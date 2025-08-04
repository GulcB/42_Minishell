/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:20:45 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 21:35:57 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_env	*create_env_node(t_gc *gc, const char *key, const char *value)
{
	t_env	*node;

	node = (t_env *)gc_malloc(gc, sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = gc_strdup(gc, key);
	node->value = gc_strdup(gc, value);
	node->next = NULL;
	if (!node->key || !node->value)
		return (NULL);
	return (node);
}

t_env	*parse_and_create_node(t_gc *gc, char *env_str, t_env **current)
{
	char	*equal_sign;
	char	*key;
	char	*value;
	t_env	*new_node;

	equal_sign = ft_strchr(env_str, '=');
	if (!equal_sign)
		return (NULL);
	key = gc_substr(gc, env_str, 0, equal_sign - env_str);
	value = gc_strdup(gc, equal_sign + 1);
	if (!key || !value)
		return (NULL);
	new_node = create_env_node(gc, key, value);
	if (!new_node)
		return (NULL);
	if (*current)
		(*current)->next = new_node;
	*current = new_node;
	return (new_node);
}

t_env	*build_env_list(t_gc *gc, char **env_array)
{
	t_env	*head;
	t_env	*current;
	t_env	*new_node;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (env_array[i])
	{
		new_node = parse_and_create_node(gc, env_array[i], &current);
		if (new_node && !head)
			head = new_node;
		i++;
	}
	return (head);
}

t_env	*init_env_from_system_with_gc(t_gc *gc, char **env_array)
{
	if (!gc || !env_array)
		return (NULL);
	return (build_env_list(gc, env_array));
}
