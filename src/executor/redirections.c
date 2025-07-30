/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 20:21:37 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/30 18:33:32 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	execute_input_redirect(const char *filename, int fd_num)
{
	int	fd;
	int	target_fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, (char *)filename, ft_strlen((char *)filename));
		write(STDERR_FILENO, ": ", 2);
		perror("");
		return (-1);
	}
	target_fd = (fd_num >= 0) ? fd_num : STDIN_FILENO;
	if (dup2(fd, target_fd) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	execute_output_redirect(const char *filename, int fd_num)
{
	int	fd;
	int	target_fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, (char *)filename, ft_strlen((char *)filename));
		write(STDERR_FILENO, ": ", 2);
		perror("");
		return (-1);
	}
	target_fd = (fd_num >= 0) ? fd_num : STDOUT_FILENO;
	if (dup2(fd, target_fd) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	execute_append_redirect(const char *filename, int fd_num)
{
	int	fd;
	int	target_fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, (char *)filename, ft_strlen((char *)filename));
		write(STDERR_FILENO, ": ", 2);
		perror("");
		return (-1);
	}
	target_fd = (fd_num >= 0) ? fd_num : STDOUT_FILENO;
	if (dup2(fd, target_fd) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	execute_redirect(t_ast_node *redirect_node, t_exec_context *ctx)
{
	if (!redirect_node || !ctx)
		return (1);
	if (redirect_node->type != NODE_REDIRECT)
		return (1);
	if (!redirect_node->redirect_file)
		return (1);
	if (redirect_node->redirect_type == REDIRECT_IN)
		return (execute_input_redirect(redirect_node->redirect_file, redirect_node->fd_num));
	else if (redirect_node->redirect_type == REDIRECT_OUT)
		return (execute_output_redirect(redirect_node->redirect_file, redirect_node->fd_num));
	else if (redirect_node->redirect_type == REDIRECT_APPEND)
		return (execute_append_redirect(redirect_node->redirect_file, redirect_node->fd_num));
	else if (redirect_node->redirect_type == REDIRECT_HEREDOC)
		return (execute_heredoc(redirect_node, ctx));
	return (1);
}

int	execute_redirection(t_ast_node *redirect_node, struct s_exec_context *ctx)
{
	if (!redirect_node || !ctx)
		return (1);
	if (redirect_node->type != NODE_REDIRECT)
		return (1);
	if (!redirect_node->redirect_file)
		return (1);
	if (redirect_node->redirect_type == REDIRECT_IN)
		return (execute_input_redirect(redirect_node->redirect_file, redirect_node->fd_num));
	else if (redirect_node->redirect_type == REDIRECT_OUT)
		return (execute_output_redirect(redirect_node->redirect_file, redirect_node->fd_num));
	else if (redirect_node->redirect_type == REDIRECT_APPEND)
		return (execute_append_redirect(redirect_node->redirect_file, redirect_node->fd_num));
	else if (redirect_node->redirect_type == REDIRECT_HEREDOC)
		return (execute_heredoc(redirect_node, ctx));
	return (1);
}
