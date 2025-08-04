/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 22:39:47 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 22:40:15 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	handle_directory_error(t_exec_context *ctx, const char *cmd)
{
	ctx->exit_status = 126;
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, ": Is a directory\n", 17);
}

void	handle_permission_error(t_exec_context *ctx, const char *cmd)
{
	ctx->exit_status = 126;
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, ": Permission denied\n", 20);
}

void	handle_not_found_error(t_exec_context *ctx, const char *cmd)
{
	ctx->exit_status = 127;
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, ": No such file or directory\n", 28);
}
