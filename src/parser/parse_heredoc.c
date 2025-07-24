/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:57:02 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/24 10:40:20 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "executor.h"

static int	is_quoted_delimiter(const char *delimiter)
{
	int	len;

	if (!delimiter)
		return (0);
	len = ft_strlen(delimiter);
	if (len < 2)
		return (0);
	return ((delimiter[0] == '\'' && delimiter[len - 1] == '\'')
		|| (delimiter[0] == '"' && delimiter[len - 1] == '"'));
}

static char	*extract_delimiter_content(const char *delimiter, t_gc *gc)
{
	int		len;
	char	*content;

	if (!delimiter || !gc)
		return (NULL);
	len = ft_strlen(delimiter);
	if (len < 2)
		return (gc_strdup(gc, delimiter));
	if ((delimiter[0] == '\'' && delimiter[len - 1] == '\'')
		|| (delimiter[0] == '"' && delimiter[len - 1] == '"'))
	{
		content = gc_substr(gc, delimiter, 1, len - 2);
		return (content);
	}
	return (gc_strdup(gc, delimiter));
}

static char	**create_heredoc_args(int quoted, t_gc *gc)
{
	char	**args;

	if (!gc)
		return (NULL);
	args = (char **)gc_malloc(gc, sizeof(char *) * 2);
	if (!args)
		return (NULL);
	if (quoted)
		args[0] = gc_strdup(gc, "1");
	else
		args[0] = gc_strdup(gc, "0");
	if (!args[0])
	{
		gc_free(gc, args);
		return (NULL);
	}
	args[1] = NULL;
	return (args);
}

static t_ast_node	*setup_heredoc_node(char *delimiter, int quoted, t_gc *gc)
{
	t_ast_node	*node;

	if (!gc)
		return (NULL);
	node = create_ast_node(gc, NODE_REDIRECT);
	if (!node)
		return (NULL);
	node->redirect_type = REDIRECT_HEREDOC;
	node->redirect_file = extract_delimiter_content(delimiter, gc);
	if (!node->redirect_file)
	{
		free_ast(gc, node);
		return (NULL);
	}
	node->args = create_heredoc_args(quoted, gc);
	if (!node->args)
	{
		free_ast(gc, node);
		return (NULL);
	}
	return (node);
}

t_ast_node	*parse_heredoc(t_token **current, struct s_exec_context *ctx)
{
	t_ast_node	*node;
	char		*delimiter;
	int			quoted;
	t_gc		*gc;

	if (!current || !*current || !ctx || !ctx->gc)
		return (NULL);
	gc = ctx->gc;
	if ((*current)->type != TOKEN_HEREDOC)
		return (NULL);
	*current = (*current)->next;
	if (!*current || (!is_word_token(*current) 
			&& (*current)->type != TOKEN_SQUOTE 
			&& (*current)->type != TOKEN_DQUOTE))
		return (NULL);
	delimiter = (*current)->value;
	quoted = is_quoted_delimiter(delimiter);
	node = setup_heredoc_node(delimiter, quoted, gc);
	if (!node)
		return (NULL);
	*current = (*current)->next;
	return (node);
}
