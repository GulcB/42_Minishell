/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:51:00 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 21:57:15 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "lexer.h"

extern int	g_signal;

char	*create_temp_filename(void)
{
	static int	counter = 0;
	char		*num_str;
	char		*filename;

	num_str = ft_itoa(counter++);
	if (!num_str)
		return (NULL);
	filename = ft_strjoin("/tmp/.minishell_heredoc_", num_str);
	free(num_str);
	return (filename);
}

int	is_delimiter_match(const char *line, const char *delimiter)
{
	return (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
		&& ft_strlen(line) == ft_strlen(delimiter));
}

int	is_heredoc_quoted(t_ast_node *heredoc_node)
{
	return (heredoc_node->args && heredoc_node->args[0]
		&& ft_strncmp(heredoc_node->args[0], "1", 2) == 0
		&& ft_strlen(heredoc_node->args[0]) == 1);
}

char	*read_heredoc_line(void)
{
	char	*line;

	line = readline("> ");
	if (!line)
	{
		if (g_signal == 2)
			return (NULL);
		write(STDOUT_FILENO, "\n", 1);
	}
	return (line);
}

int	handle_readline_result(char *line)
{
	if (!line)
	{
		if (g_signal == 2)
			return (130);
		else
			return (0);
	}
	if (g_signal == 2)
	{
		free(line);
		return (130);
	}
	return (-1);
}
