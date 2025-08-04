/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_content.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:52:00 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 21:57:15 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "lexer.h"

int	process_and_write_line(int fd, char *line, int quoted,
		t_exec_context *ctx)
{
	char	*expanded_line;

	if (quoted)
		expanded_line = gc_strdup(ctx->gc, line);
	else
		expanded_line = expand_variables(line, ctx);
	if (write(fd, expanded_line, ft_strlen(expanded_line)) == -1
		|| write(fd, "\n", 1) == -1)
		return (1);
	return (0);
}

int	process_heredoc_line(int fd, const char *delimiter, int quoted,
		t_exec_context *ctx)
{
	char	*line;
	int		readline_result;

	line = read_heredoc_line();
	readline_result = handle_readline_result(line);
	if (readline_result != -1)
		return (readline_result);
	if (is_delimiter_match(line, delimiter))
	{
		free(line);
		return (0);
	}
	if (process_and_write_line(fd, line, quoted, ctx))
	{
		free(line);
		return (1);
	}
	free(line);
	return (-1);
}

int	write_heredoc_content(int fd, const char *delimiter, int quoted,
		t_exec_context *ctx)
{
	int	signal_result;
	int	process_result;

	while (1)
	{
		signal_result = check_heredoc_signal(ctx);
		if (signal_result)
			return (signal_result);
		process_result = process_heredoc_line(fd, delimiter, quoted, ctx);
		if (process_result != -1)
			return (process_result);
	}
	return (0);
}
