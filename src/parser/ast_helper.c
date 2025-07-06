/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:18:07 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/06 22:48:44 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "inc/parser.h"

void	write_str(char *str)
{
	if (str)
		write(STDOUT_FILENO, str, ft_strlen(str));
}

static void	write_node_type(t_node_type type)
{
	if (type == NODE_COMMAND)
		write_str("COMMAND");
	else if (type == NODE_PIPE)
		write_str("PIPE");
	else if (type == NODE_REDIRECT)
		write_str("REDIRECT");
	else if (type == NODE_AND)
		write_str("AND");
	else if (type == NODE_OR)
		write_str("OR");
	else
		write_str("UNKNOWN");
}

static void	print_node_content(t_ast_node *node)
{
	write_node_type(node->type);
	if (node->type == NODE_COMMAND && node->args)
		print_args(node->args);
	else if (node->type == NODE_REDIRECT)
	{
		write_redirect_type(node->redirect_type);
		if (node->redirect_file)
		{
			write_str("[");
			write_str(node->redirect_file);
			write_str("]");
		}
	}
	write_str("\n");
}

void	print_ast_recursive(t_ast_node *node, int depth)
{
	int	i;

	if (!node)
		return ;
	i = 0;
	while (i < depth)
	{
		write_str("  ");
		i++;
	}
	print_node_content(node);
	if (node->left)
		print_ast_recursive(node->left, depth + 1);
	if (node->right)
		print_ast_recursive(node->right, depth + 1);
}

void	print_ast(t_ast_node *node)
{
	write_str("=== AST DEBUG ===\n");
	if (!node)
	{
		write_str("AST is NULL\n");
		return ;
	}
	print_ast_recursive(node, 0);
	write_str("=== END AST ===\n");
}
