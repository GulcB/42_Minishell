/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_clean.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:15:16 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/19 16:20:49 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/garbage_collector.h"

static t_gc	g_gc = {NULL}; // GLOBAL OLARAK GEÇİYO DİKKAKAATTTT printf

int	gc_remove_node(void *ptr)
{
	t_gc_node	*current;
	t_gc_node	*prev;

	if (!ptr || !g_gc.head)
		return (0);
	current = g_gc.head;
	prev = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				g_gc.head = current->next;
			free(current);
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

void	gc_free(void *ptr)
{
	if (!ptr)
		return ;
	if (gc_remove_node(ptr))
		free(ptr);
}

void	gc_cleanup(void)
{
	t_gc_node	*current;
	t_gc_node	*temp;

	current = g_gc.head;
	while (current)
	{
		temp = current->next;
		if (current->ptr)
			free(current->ptr);
		free(current);
		current = temp;
	}
	g_gc.head = NULL;
}
