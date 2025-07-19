/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_clean.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:15:16 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/19 17:08:21 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/garbage_collector.h"

void	*gc_malloc(t_gc *gc, size_t size)
{
	void		*ptr;
	t_gc_node	*node;

	if (!gc)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	node = gc_create_node(ptr);
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	gc_add_node(gc, node);
	return (ptr);
}

int	gc_remove_node(t_gc *gc, void *ptr)
{
	t_gc_node	*current;
	t_gc_node	*prev;

	if (!gc || !ptr || !gc->head)
		return (0);
	current = gc->head;
	prev = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				gc->head = current->next;
			free(current);
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

void	gc_free(t_gc *gc, void *ptr)
{
	if (!gc || !ptr)
		return ;
	if (gc_remove_node(gc, ptr))
		free(ptr);
}

void	gc_cleanup_all(t_gc *gc)
{
	t_gc_node	*current;
	t_gc_node	*temp;

	if (!gc)
		return ;
	current = gc->head;
	while (current)
	{
		temp = current->next;
		if (current->ptr)
			free(current->ptr);
		free(current);
		current = temp;
	}
	gc->head = NULL;
}
