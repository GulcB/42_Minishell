/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 21:10:00 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/06 21:11:04 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef struct 			s_gc_node
{
    void				*ptr;
    struct s_gc_node	*next;
}						t_gc_node;

typedef struct	s_gc
{
    t_gc_node	*head;
}				t_gc;

