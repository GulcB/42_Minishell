/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:03:19 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 15:38:27 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "lexer.h"

extern int	g_signal;

static void	heredoc_signal_handler(int sig)
{
    g_signal = sig;
	write(STDOUT_FILENO, "^C", 2);
    close(STDIN_FILENO);
}

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

static void	setup_heredoc_signals(void (**old_handler)(int))
{
	*old_handler = signal(2, heredoc_signal_handler);
}

static void	restore_heredoc_signals(void (*old_handler)(int))
{
    signal(2, old_handler);
    if (g_signal == 2)
    {
        int new_stdin = open("/dev/tty", O_RDONLY);
        if (new_stdin != -1)
            dup2(new_stdin, STDIN_FILENO);
        if (new_stdin > 2)
            close(new_stdin);
    }
    g_signal = 0;
}

static int	write_heredoc_content(int fd, const char *delimiter,
        int quoted, t_exec_context *ctx)
{
    char	*line;
    char	*expanded_line;
    while (1)
    {
        if (g_signal == 2)
		{
			ctx->exit_status = 130;
            return (130);
		}   
        line = readline("> ");
        
        if (!line)
        {
			if (g_signal == 2)
                return (130);
            write(STDOUT_FILENO, "\n", 1);
            return (0);
        }
        
        if (g_signal == 2)
        {
            free(line);
            return (130);
        }
        
        if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
            && ft_strlen(line) == ft_strlen(delimiter))
        {
            free(line);
            break ;
        }
        
        if (quoted)
            expanded_line = gc_strdup(ctx->gc, line);
        else
            expanded_line = expand_variables(line, ctx);
            
        if (write(fd, expanded_line, ft_strlen(expanded_line)) == -1 ||
            write(fd, "\n", 1) == -1)
        {
            free(line);
            return (1);
        }
        free(line);
    }
    return (0);
}
static int	create_heredoc_file(const char *filename, const char *delimiter,
		int quoted, t_exec_context *ctx)
{
	int			fd;
	int			result;
	void		(*old_handler)(int);

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

static int	setup_heredoc_input(const char *filename)
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
	quoted = (heredoc_node->args && heredoc_node->args[0]
			&& ft_strncmp(heredoc_node->args[0], "1", 2) == 0
			&& ft_strlen(heredoc_node->args[0]) == 1);
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
	quoted = (heredoc_node->args && heredoc_node->args[0]
			&& ft_strncmp(heredoc_node->args[0], "1", 2) == 0
			&& ft_strlen(heredoc_node->args[0]) == 1);
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
