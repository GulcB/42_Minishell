/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:00:43 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/20 21:53:10 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h" 
#include "executor.h"

int	builtin_exit(char **args, struct s_exec_context *ctx)
{
	int exit_code = 0;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (args[1])
		exit_code = ft_atoi(args[1]);
	if (ctx)
		ctx->exit_status = exit_code;
	return (SHELL_EXIT);
}
