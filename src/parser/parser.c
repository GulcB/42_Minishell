/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:27:38 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/30 17:10:14 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "executor.h"

int	is_word_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_WORD || token->type == TOKEN_DQUOTE
		|| token->type == TOKEN_SQUOTE || token->type == TOKEN_DOLLAR);
}

int	is_stop_token(t_token *token)
{
	if (!token)
		return (1);
	return (token->type == TOKEN_PIPE || token->type == TOKEN_AND
        || token->type == TOKEN_OR || token->type == TOKEN_SEMICOLON
        || token->type == TOKEN_EOF || token->type == TOKEN_ERROR);
}

t_ast_node	*parse_pipe(t_token **current, struct s_exec_context *ctx)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_ast_node	*pipe_node;
	t_gc		*gc;

	if (!current || !ctx || !ctx->gc)
		return (NULL);
	gc = ctx->gc;
	left = parse_command(current, ctx);
	if (!left)
		return (NULL);
	if (!*current || (*current)->type != TOKEN_PIPE)
		return (left);
	*current = (*current)->next;
	right = parse_pipe(current, ctx);
	if (!right)
		return (free_ast(gc, left), NULL);
	pipe_node = create_ast_node(gc, NODE_PIPE);
	if (!pipe_node)
		return (free_ast(gc, left), free_ast(gc, right), NULL);
	pipe_node->left = left;
	pipe_node->right = right;
	return (pipe_node);
}

t_ast_node	*parse_logical_and(t_token **current, struct s_exec_context *ctx)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_ast_node	*and_node;
	t_gc		*gc;

	if (!current || !ctx || !ctx->gc)
		return (NULL);
	gc = ctx->gc;
	left = parse_pipe(current, ctx);
	if (!left)
		return (NULL);
	if (!*current || (*current)->type != TOKEN_AND)
		return (left);
	*current = (*current)->next;
	right = parse_logical_and(current, ctx);
	if (!right)
		return (free_ast(gc, left), NULL);
	and_node = create_ast_node(gc, NODE_AND);
	if (!and_node)
		return (free_ast(gc, left), free_ast(gc, right), NULL);
	and_node->left = left;
	and_node->right = right;
	return (and_node);
}

t_ast_node	*parse_tokens_with_context(t_token *tokens,
				struct s_exec_context *ctx)
{
	t_token			*current;
	t_ast_node		*ast;
	t_gc			*parser_gc;
	t_exec_context	temp_ctx;

	if (!tokens)
		return (NULL);
	if (ctx && ctx->gc)
	{
		current = tokens;
		ast = parse_logical_and(&current, ctx);
		return (ast);
	}
	parser_gc = gc_init();
	if (!parser_gc)
		return (NULL);
	temp_ctx.gc = parser_gc;
	temp_ctx.env = NULL;
	temp_ctx.exit_status = 0;
	current = tokens;
	ast = parse_logical_and(&current, &temp_ctx);
	return (ast);
}
