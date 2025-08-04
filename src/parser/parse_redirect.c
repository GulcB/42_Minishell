/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 21:46:26 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 18:37:06 by gbodur           ###   ########.fr       */
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

static int	extract_fd_number(char *redirect_value)
{
	if (redirect_value && ft_strlen(redirect_value) >= 2
		&& ft_isdigit(redirect_value[0]))
		return (redirect_value[0] - '0');
	return (-1);
}

static char	*build_filename(t_token **current, struct s_exec_context *ctx)
{
	char	*filename;
	char	*expanded_value;
	t_gc	*gc;

	gc = ctx->gc;
	expanded_value = expand_token_value(*current, ctx);
	if (!expanded_value)
		return (NULL);
	filename = expanded_value;
	*current = (*current)->next;
	while (*current && is_word_token(*current) && (*current)->prev
		&& tokens_are_adjacent((*current)->prev, *current))
	{
		expanded_value = expand_token_value(*current, ctx);
		if (!expanded_value)
			return (NULL);
		filename = gc_strjoin(gc, filename, expanded_value);
		if (!filename)
			return (NULL);
		*current = (*current)->next;
	}
	return (filename);
}

t_ast_node	*parse_redirect(t_token **current, struct s_exec_context *ctx)
{
	t_token_type	redirect_type;
	int				fd_num;
	char			*filename;

	if (!current || !*current || !ctx || !ctx->gc)
		return (NULL);
	if (!is_redirect_token(*current))
		return (NULL);
	redirect_type = (*current)->type;
	fd_num = extract_fd_number((*current)->value);
	if (redirect_type == TOKEN_HEREDOC)
		return (parse_heredoc(current, ctx));
	*current = (*current)->next;
	if (!*current || !is_word_token(*current))
		return (NULL);
	filename = build_filename(current, ctx);
	if (!filename)
		return (NULL);
	return (create_redirect_node(ctx->gc, get_redirect_type(redirect_type),
			filename, fd_num));
}
