/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 19:57:40 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/29 13:30:52 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

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

int	env_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->key)
			count++;
		env = env->next;
	}
	return (count);
}

char	*ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, s3);
	free(tmp);
	return (result);
}

char	**env_to_array(t_env *env, int count)
{
	char	**array;
	int		i;

	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->key)
		{
			if (env->value)
				array[i] = ft_strjoin_three(env->key, "=", env->value);
			else
				array[i] = ft_strjoin_three(env->key, "", "");
			i++;
		}
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}

void	sort_env_array(char **array, int count)
{
	int		i;
	int		swapped;
	char	*tmp;

	if (!array)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (i < count - 1)
		{
			if (ft_strncmp(array[i], array[i + 1], ft_strlen(array[i]) + 1) > 0)
			{
				tmp = array[i];
				array[i] = array[i + 1];
				array[i + 1] = tmp;
				swapped = 1;
			}
			i++;
		}
	}
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
