/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_clean.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:15:16 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/31 16:01:43 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

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

void	gc_destroy(t_gc *gc)
{
	if (!gc)
		return ;
	gc_cleanup_all(gc);
	free(gc);
}
