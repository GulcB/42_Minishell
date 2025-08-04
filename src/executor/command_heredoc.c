/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 22:19:00 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 22:20:02 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	execute_heredoc_consume_only(t_ast_node *heredoc_node,
		t_exec_context *ctx)
{
	char	*delimiter;
	char	*line;

	(void)ctx;
	if (!heredoc_node || heredoc_node->redirect_type != REDIRECT_HEREDOC)
		return (1);
	delimiter = heredoc_node->redirect_file;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putchar_fd('\n', STDOUT_FILENO);
			return (0);
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& ft_strlen(line) == ft_strlen(delimiter))
		{
			free(line);
			break ;
		}
		free(line);
	}
	return (0);
}

t_ast_node	*find_last_heredoc(t_ast_node *redirect_node)
{
	t_ast_node	*last_heredoc;

	last_heredoc = NULL;
	while (redirect_node)
	{
		if (redirect_node->redirect_type == REDIRECT_HEREDOC)
			last_heredoc = redirect_node;
		redirect_node = redirect_node->right;
	}
	return (last_heredoc);
}

void	process_non_last_heredocs(t_ast_node *redirect_node,
		t_ast_node *last_heredoc, t_exec_context *ctx)
{
	while (redirect_node)
	{
		if (redirect_node->redirect_type == REDIRECT_HEREDOC
			&& redirect_node != last_heredoc)
			execute_heredoc_consume_only(redirect_node, ctx);
		redirect_node = redirect_node->right;
	}
}
