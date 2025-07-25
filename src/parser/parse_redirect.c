/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 21:46:26 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/24 21:13:16 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "executor.h"

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

t_ast_node	*create_redirect_node(t_gc *gc, t_redirect_type type,
				char *filename)
{
	t_ast_node	*node;

	if (!filename || !gc)
		return (NULL);
	node = create_ast_node(gc, NODE_REDIRECT);
	if (!node)
		return (NULL);
	node->redirect_type = type;
	node->redirect_file = gc_strdup(gc, filename);
	if (!node->redirect_file)
	{
		free_ast(gc, node);
		return (NULL);
	}
	return (node);
}

t_ast_node	*parse_redirect(t_token **current, struct s_exec_context *ctx)
{
	t_ast_node		*node;
	t_token_type	redirect_type;
	t_gc			*gc;

	if (!current || !*current || !ctx || !ctx->gc)
		return (NULL);
	gc = ctx->gc;
	if (!is_redirect_token(*current))
		return (NULL);
	redirect_type = (*current)->type;
	if (redirect_type == TOKEN_HEREDOC)
		return (parse_heredoc(current, ctx));
	*current = (*current)->next;
	if (!*current || !is_word_token(*current))
		return (NULL);
	node = create_redirect_node(gc, get_redirect_type(redirect_type),
			(*current)->value);
	if (!node)
		return (NULL);
	*current = (*current)->next;
	return (node);
}
