/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_signal_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 00:46:17 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/17 16:59:03 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/executor.h"

static int	extract_exit_code(int status)
{
	return ((status >> 8) & 0xFF);
}

static int	extract_signal_number(int status)
{
	return (status & 0x7F);
}

static int	is_normal_exit(int status)
{
	return ((status & 0x7F) == 0);
}

static int	is_signaled_exit(int status)
{
	return ((status & 0x7F) != 0 && ((status & 0x7F) != 0x7F));
}

void	update_exit_status(t_exec_context *ctx, int status)
{
	if (is_normal_exit(status))
		ctx->exit_status = extract_exit_code(status);
	else if (is_signaled_exit(status))
		ctx->exit_status = 128 + extract_signal_number(status);
	else
		ctx->exit_status = 1;
}
