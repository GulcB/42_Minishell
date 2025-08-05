/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:01 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/05 09:49:45 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_execution_exit(t_token *tokens, char *input,
		t_exec_context *ctx, t_gc *main_gc)
{
	int	exit_status;

	if (execute_and_cleanup(tokens, input, ctx) == -42)
	{
		exit_status = ctx->exit_status;
		restore_std_fds(ctx);
		gc_cleanup_all(main_gc);
		safe_cleanup_and_exit(main_gc);
		exit(exit_status);
	}
	return (0);
}
