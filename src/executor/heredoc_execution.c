/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:03:19 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 21:57:11 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "lexer.h"

int	create_heredoc_file(const char *filename, const char *delimiter,
		int quoted, t_exec_context *ctx)
{
	int		fd;
	int		result;
	void	(*old_handler)(int);

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		write(STDERR_FILENO, "minishell: heredoc", 18);
		return (1);
	}
	setup_heredoc_signals(&old_handler);
	result = write_heredoc_content(fd, delimiter, quoted, ctx);
	restore_heredoc_signals(old_handler);
	close(fd);
	return (result);
}

int	setup_heredoc_input(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		write(STDERR_FILENO, "minishell: heredoc", 18);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		write(STDERR_FILENO, "minishell: dup2", 15);
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	execute_heredoc(t_ast_node *heredoc_node, t_exec_context *ctx)
{
	char	*filename;
	char	*delimiter;
	int		quoted;
	int		result;

	if (!heredoc_node || heredoc_node->redirect_type != REDIRECT_HEREDOC)
		return (1);
	delimiter = heredoc_node->redirect_file;
	quoted = is_heredoc_quoted(heredoc_node);
	filename = create_temp_filename();
	if (!filename)
		return (1);
	result = create_heredoc_file(filename, delimiter, quoted, ctx);
	if (result == 130)
	{
		unlink(filename);
		free(filename);
		return (130);
	}
	if (result == 0)
		result = setup_heredoc_input(filename);
	unlink(filename);
	free(filename);
	return (result);
}

int	preprocess_heredoc(t_ast_node *heredoc_node, t_exec_context *ctx)
{
	char	*filename;
	char	*delimiter;
	int		quoted;
	int		result;

	if (!heredoc_node || heredoc_node->redirect_type != REDIRECT_HEREDOC)
		return (1);
	delimiter = heredoc_node->redirect_file;
	quoted = is_heredoc_quoted(heredoc_node);
	filename = create_temp_filename();
	if (!filename)
		return (1);
	result = create_heredoc_file(filename, delimiter, quoted, ctx);
	if (result == 130)
	{
		free(filename);
		return (130);
	}
	if (result == 0)
	{
		heredoc_node->redirect_type = REDIRECT_IN;
		heredoc_node->redirect_file = gc_strdup(ctx->gc, filename);
	}
	free(filename);
	return (result);
}
