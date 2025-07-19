/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:08:20 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/19 16:21:36 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/garbage_collector.h"

static	t_gc g_gc = {NULL};

void	gc_init(void)
{
	g_gc.head = NULL;
}

t_gc_node	*gc_create_node(void *ptr)
{
	t_gc_node	*node;

	if (!ptr)
		return (NULL);
	node = (t_gc_node *)malloc(sizeof(t_gc_node));
	if (!node)
		return (NULL);
	node->ptr = ptr;
	node->next = NULL;
	return (node);
}

void	gc_add_node(t_gc_node *node)
{
	if (!node)
		return ;
	node->next = g_gc.head;
	g_gc.head = node;
}

void	*gc_malloc(size_t size)
{
	void		*ptr;
	t_gc_node	*node;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	node = gc_create_node(ptr);
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	gc_add_node(node);
	return (ptr);
}
