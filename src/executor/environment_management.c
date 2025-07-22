/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_management.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 00:35:32 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/22 14:16:01 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/executor.h"

t_env	*create_env_node_classic(const char *key, const char *value)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->next = NULL;
	if (!node->key || !node->value)
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	return (node);
}

t_env	*create_env_node(t_gc *gc, const char *key, const char *value)
{
	t_env	*node;

	node = (t_env *)gc_malloc(gc, sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->next = NULL;
	if (!node->key || !node->value)
		return (NULL);
	return (node);
}

t_env	*parse_and_create_node_classic(char *env_str, t_env **current)
{
	char	*equal_sign;
	char	*key;
	char	*value;
	t_env	*new_node;

	equal_sign = ft_strchr(env_str, '=');
	if (!equal_sign)
		return (NULL);
	key = ft_substr(env_str, 0, equal_sign - env_str);
	value = ft_strdup(equal_sign + 1);
	if (!key || !value)
	{
		free(key);
		free(value);
		return (NULL);
	}
	new_node = create_env_node_classic(key, value);
	free(key);
	free(value);
	if (!new_node)
		return (NULL);
	if (*current)
		(*current)->next = new_node;
	*current = new_node;
	return (new_node);
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
	key = ft_substr(env_str, 0, equal_sign - env_str);
	value = ft_strdup(equal_sign + 1);
	if (!key || !value)
	{
		free(key);
		free(value);
		return (NULL);
	}
	new_node = create_env_node(gc, key, value);
	free(key);
	free(value);
	if (!new_node)
		return (NULL);
	if (*current)
		(*current)->next = new_node;
	*current = new_node;
	return (new_node);
}

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

t_env	*build_env_list_classic(char **env_array)
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
		new_node = parse_and_create_node_classic(env_array[i], &current);
		if (new_node && !head)
		head = new_node;
		i++;
	}
	return (head);
}
t_env	*init_env_from_system(char **env_array)
{
	if (!env_array)
		return (NULL);
	return (build_env_list_classic(env_array));
}

char	*env_get(t_env *env, const char *key)
{
	t_env	*current;
	size_t	key_len;
	
	current = env;
	if (!key)
	return (NULL);
	key_len = ft_strlen(key);
	while (current)
	{
		if (ft_strncmp(current->key, key, key_len + 1) == 0
			&& ft_strlen(current->key) == key_len)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
