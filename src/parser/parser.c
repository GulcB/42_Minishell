/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:27:38 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/19 16:31:07 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_word_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_WORD || token->type == TOKEN_DQUOTE
		|| token->type == TOKEN_SQUOTE);
}

int	is_stop_token(t_token *token)
{
	if (!token)
		return (1);
	return (token->type == TOKEN_PIPE || token->type == TOKEN_REDIR_IN
		|| token->type == TOKEN_REDIR_OUT || token->type == TOKEN_APPEND
		|| token->type == TOKEN_HEREDOC || token->type == TOKEN_AND
		|| token->type == TOKEN_OR || token->type == TOKEN_SEMICOLON
		|| token->type == TOKEN_EOF || token->type == TOKEN_ERROR);
}

t_ast_node	*parse_pipe(t_token **current)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_ast_node	*pipe_node;

	left = parse_command(current);
	if (!left)
		return (NULL);
	if (!*current || (*current)->type != TOKEN_PIPE)
		return (left);
	*current = (*current)->next;
	right = parse_pipe(current);
	if (!right)
		return (free_ast(left), NULL);
	pipe_node = create_ast_node(NODE_PIPE);
	if (!pipe_node)
		return (free_ast(left), free_ast(right), NULL);
	pipe_node->left = left;
	pipe_node->right = right;
	return (pipe_node);
}

t_ast_node	*parse_logical_and(t_token **current)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_ast_node	*and_node;

	left = parse_pipe(current);
	if (!left)
		return (NULL);
	if (!*current || (*current)->type != TOKEN_AND)
		return (left);
	*current = (*current)->next;
	right = parse_logical_and(current);
	if (!right)
		return (free_ast(left), NULL);
	and_node = create_ast_node(NODE_AND);
	if (!and_node)
		return (free_ast(left), free_ast(right), NULL);
	and_node->left = left;
	and_node->right = right;
	return (and_node);
}

t_ast_node	*parse_tokens(t_token *tokens)
{
	t_token		*current;
	t_ast_node	*ast;

	if (!tokens)
		return (NULL);
	current = tokens;
	ast = parse_logical_and(&current);
	return (ast);
}
