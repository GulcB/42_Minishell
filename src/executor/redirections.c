/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 20:21:37 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/24 10:13:30 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	execute_input_redirect(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)filename, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	execute_output_redirect(const char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)filename, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	execute_append_redirect(const char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)filename, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
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
		return (execute_input_redirect(redirect_node->redirect_file));
	else if (redirect_node->redirect_type == REDIRECT_OUT)
		return (execute_output_redirect(redirect_node->redirect_file));
	else if (redirect_node->redirect_type == REDIRECT_APPEND)
		return (execute_append_redirect(redirect_node->redirect_file));
	else if (redirect_node->redirect_type == REDIRECT_HEREDOC)
		return (execute_heredoc(redirect_node, ctx));
	return (1);
}
