/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:32:44 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/19 16:31:02 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	print_args(char **args)
{
	int	i;

	if (!args)
		return ;
	write_str(" [");
	i = 0;
	while (args[i])
	{
		write_str(args[i]);
		if (args[i + 1])
			write_str(", ");
		i++;
	}
	write_str("]");
}

void	write_redirect_type(t_redirect_type type)
{
	if (type == REDIRECT_IN)
		write_str(" <");
	else if (type == REDIRECT_OUT)
		write_str(" >");
	else if (type == REDIRECT_APPEND)
		write_str(" >>");
	else if (type == REDIRECT_HEREDOC)
		write_str(" <<");
	else
		write_str(" ?");
}

int	validate_ast(t_ast_node *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
	{
		if (!node->args || !node->args[0])
			return (0);
	}
	else if (node->type == NODE_PIPE)
	{
		if (!node->left || !node->right)
			return (0);
		if (!validate_ast(node->left) || !validate_ast(node->right))
			return (0);
	}
	else if (node->type == NODE_REDIRECT)
	{
		if (!node->redirect_file)
			return (0);
	}
	return (1);
}
