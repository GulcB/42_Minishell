/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_nodes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 21:37:57 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/31 17:48:39 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	free_args_array(t_gc *gc, char **args)
{
	(void)gc;
	(void)args;
}

void	free_ast(t_gc *gc, t_ast_node *node)
{
	(void)gc;
	(void)node;
}

t_ast_node	*create_ast_node(t_gc *gc, t_node_type type)
{
	t_ast_node	*node;

	node = (t_ast_node *)gc_malloc(gc, sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->redirect_file = NULL;
	node->redirect_type = 0;
	node->fd_num = -1;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast_node	*create_command_node(t_gc *gc, char **args)
{
	t_ast_node	*node;

	if (!args || !args[0])
		return (NULL);
	node = create_ast_node(gc, NODE_COMMAND);
	if (!node)
		return (NULL);
	node->args = args;
	return (node);
}
