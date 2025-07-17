/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 21:46:26 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/17 23:34:02 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/parser.h"

int	is_redirect_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_REDIR_IN
		|| token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_APPEND
		|| token->type == TOKEN_HEREDOC);
}

static t_redirect_type	get_redirect_type(t_token_type token_type)
{
	if (token_type == TOKEN_REDIR_IN)
		return (REDIRECT_IN);
	else if (token_type == TOKEN_REDIR_OUT)
		return (REDIRECT_OUT);
	else if (token_type == TOKEN_APPEND)
		return (REDIRECT_APPEND);
	else if (token_type == TOKEN_HEREDOC)
		return (REDIRECT_HEREDOC);
	return (REDIRECT_IN);
}

t_ast_node	*create_redirect_node(t_redirect_type type, char *filename)
{
	t_ast_node	*node;

	if (!filename)
		return (NULL);
	node = create_ast_node(NODE_REDIRECT);
	if (!node)
		return (NULL);
	node->redirect_type = type;
	node->redirect_file = ft_strdup(filename);
	if (!node->redirect_file)
	{
		free_ast(node);
		return (NULL);
	}
	return (node);
}

t_ast_node	*parse_redirect(t_token **current)
{
	t_ast_node		*node;
	t_token_type	redirect_type;

	if (!current || !*current)
		return (NULL);
	if (!is_redirect_token(*current))
		return (NULL);
	redirect_type = (*current)->type;
	*current = (*current)->next;
	if (!*current || !is_word_token(*current))
		return (NULL);
	node = create_redirect_node(get_redirect_type(redirect_type), 
			(*current)->value);
	if (!node)
		return (NULL);
	*current = (*current)->next;
	return (node);
}
