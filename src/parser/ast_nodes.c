/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_nodes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 21:37:57 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/06 21:44:49 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "inc/parser.h"

void	free_args_array(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}
void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->args)
		free_args_array(node->args);
	if (node->redirect_file)
		free(node->redirect_file);
	if (node->left)
		free_ast(node->left);
	if (node->right)
		free_ast(node->right);
	free(node);
}

t_ast_node	*create_ast_node(t_node_type type)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->redirect_file = NULL;
	node->redirect_type = 0;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast_node	*create_command_node(char **args)
{
	t_ast_node	*node;

	if (!args || !args[0])
		return (NULL);
	node = create_ast_node(NODE_COMMAND);
	if (!node)
		return (NULL);
	node->args = args;
	return (node);
}
