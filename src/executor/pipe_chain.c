/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_chain.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:47:47 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/15 18:58:05 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/executor.h"



int	count_pipe_commands(t_ast_node *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
		return (1);
	if (node->type == NODE_PIPE)
		return (count_pipe_commands(node->left)
			+ count_pipe_commands(node->right));
	return (0);
}

int validate_pipe_chain(t_ast_node *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
		return (node->args && node->args[0]);
	if (node->type == NODE_PIPE)
		return (validate_pipe_chain(node->left)
			&& validate_pipe_chain(node->right));
	return (0);
}

int wait_for_pipe_children(t_exec_context *ctx)
{
	return (wait_for_pipe_chain(ctx));
}
