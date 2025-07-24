/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 21:10:00 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/24 21:26:57 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H

# include "libft.h"
# include <stdlib.h>
# include <unistd.h>

typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}						t_gc_node;

typedef struct s_gc
{
	t_gc_node			*head;
}						t_gc;

t_gc					*gc_init(void);
void					gc_add_node(t_gc *gc, t_gc_node *node);
t_gc_node				*gc_create_node(void *ptr);
void					*gc_malloc(t_gc *gc, size_t size);

int						gc_remove_node(t_gc *gc, void *ptr);
void					gc_free(t_gc *gc, void *ptr);
void					gc_cleanup_all(t_gc *gc);
void					gc_destroy(t_gc *gc);

char					*gc_strdup(t_gc *gc, const char *s);
char					*gc_strjoin(t_gc *gc, const char *s1, const char *s2);
char					*gc_substr(t_gc *gc, const char *s, unsigned int start,
							size_t len);

#endif
