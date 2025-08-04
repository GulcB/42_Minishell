/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:28:07 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 18:35:01 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "executor.h"

int	tokens_are_adjacent(t_token *current, t_token *next)
{
	int	current_end;
	int	value_len;

	if (!current || !next)
		return (0);
	if (current->type == TOKEN_DQUOTE || current->type == TOKEN_SQUOTE)
	{
		if (current->value)
			value_len = ft_strlen(current->value);
		else
			value_len = 0;
		current_end = current->position + 2 + value_len;
	}
	else
	{
		if (current->value)
			value_len = ft_strlen(current->value);
		else
			value_len = 1;
		current_end = current->position + value_len;
	}
	return (current_end == next->position);
}

char	*expand_token_value(t_token *token, struct s_exec_context *ctx)
{
	if (!token || !token->value)
		return (NULL);
	if (token->type == TOKEN_DQUOTE)
		return (expand_variables(token->value, ctx));
	else if (token->type == TOKEN_SQUOTE)
		return (gc_strdup(ctx->gc, token->value));
	else
		return (expand_variables(token->value, ctx));
}

t_redirect_type	get_redirect_type(t_token_type token_type)
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
				char *filename, int fd_num)
{
	t_ast_node	*node;

	if (!filename || !gc)
		return (NULL);
	node = create_ast_node(gc, NODE_REDIRECT);
	if (!node)
		return (NULL);
	node->redirect_type = type;
	node->fd_num = fd_num;
	node->redirect_file = gc_strdup(gc, filename);
	if (!node->redirect_file)
	{
		free_ast(gc, node);
		return (NULL);
	}
	return (node);
}
