/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:57:02 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/19 15:23:47 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

static char	*extract_delimiter_content(const char *delimiter)
{
	int		len;
	char	*content;

	if (!delimiter)
		return (NULL);
	len = ft_strlen(delimiter);
	if (len < 2)
		return (ft_strdup(delimiter));
	if ((delimiter[0] == '\'' && delimiter[len - 1] == '\'')
		|| (delimiter[0] == '"' && delimiter[len - 1] == '"'))
	{
		content = ft_substr(delimiter, 1, len - 2);
		return (content);
	}
	return (ft_strdup(delimiter));
}

static char	**create_heredoc_args(int quoted)
{
	char	**args;

	args = (char **)gc_malloc(sizeof(char *) * 2);
	if (!args)
		return (NULL);
	if (quoted)
		args[0] = ft_strdup("1");
	else
		args[0] = ft_strdup("0");
	if (!args[0])
	{
		gc_free(args);
		return (NULL);
	}
	args[1] = NULL;
	return (args);
}

static t_ast_node	*setup_heredoc_node(char *delimiter, int quoted)
{
	t_ast_node	*node;

	node = create_ast_node(NODE_REDIRECT);
	if (!node)
		return (NULL);
	node->redirect_type = REDIRECT_HEREDOC;
	node->redirect_file = extract_delimiter_content(delimiter);
	if (!node->redirect_file)
	{
		free_ast(node);
		return (NULL);
	}
	node->args = create_heredoc_args(quoted);
	if (!node->args)
	{
		free_ast(node);
		return (NULL);
	}
	return (node);
}

t_ast_node	*parse_heredoc(t_token **current)
{
	t_ast_node	*node;
	char		*delimiter;
	int			quoted;

	if (!current || !*current)
		return (NULL);
	if ((*current)->type != TOKEN_HEREDOC)
		return (NULL);
	*current = (*current)->next;
	if (!*current || !is_word_token(*current))
		return (NULL);
	delimiter = (*current)->value;
	quoted = is_quoted_delimiter(delimiter);
	node = setup_heredoc_node(delimiter, quoted);
	if (!node)
		return (NULL);
	*current = (*current)->next;
	return (node);
}
