/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:08:20 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/19 17:07:55 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/garbage_collector.h"

t_gc	*gc_init(void)
{
	t_gc	*gc;

	gc = (t_gc *)malloc(sizeof(t_gc));
	if (!gc)
		return (NULL);
	gc->head = NULL;
	return (gc);
}

void	gc_destroy(t_gc *gc)
{
	if (!gc)
		return ;
	gc_cleanup_all(gc);
	free(gc);
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

void	gc_add_node(t_gc *gc, t_gc_node *node)
{
	if (!gc || !node)
		return ;
	node->next = gc->head;
	gc->head = node;
}
