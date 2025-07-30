/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:03:19 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/30 18:22:17 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "lexer.h"

static char	*create_temp_filename(void)
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

static int	write_heredoc_content(int fd, const char *delimiter,
		int quoted, t_exec_context *ctx)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		printf("anan\n");
		line = readline("> ");
		if (!line)
		{
			ft_putchar_fd('\n', STDOUT_FILENO);
			return (1);
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& ft_strlen(line) == ft_strlen(delimiter))
		{
			free(line);
			break ;
		}
		if (quoted)
			expanded_line = ft_strdup(line);
		else
			expanded_line = expand_variables(line, ctx);
		write(fd, expanded_line, ft_strlen(expanded_line));
		write(fd, "\n", 1);
		free(expanded_line);
		free(line);
	}
	return (0);
}

static int	create_heredoc_file(const char *filename, const char *delimiter,
		int quoted, t_exec_context *ctx)
{
	int	fd;
	int	result;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: heredoc");
		return (1);
	}
	result = write_heredoc_content(fd, delimiter, quoted, ctx);
	close(fd);
	return (result);
}

static int	setup_heredoc_input(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell: heredoc");
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
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
	quoted = (heredoc_node->args && heredoc_node->args[0]
			&& ft_strncmp(heredoc_node->args[0], "1", 2) == 0
			&& ft_strlen(heredoc_node->args[0]) == 1);
	filename = create_temp_filename();
	if (!filename)
		return (1);
	result = create_heredoc_file(filename, delimiter, quoted, ctx);
	if (result == 0)
		result = setup_heredoc_input(filename);
	unlink(filename);
	free(filename);
	return (result);
}
