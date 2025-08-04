/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:50:00 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 21:57:14 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "lexer.h"

extern int	g_signal;

void	heredoc_signal_handler(int sig)
{
	g_signal = sig;
	write(STDOUT_FILENO, "^C", 2);
	close(STDIN_FILENO);
}

void	setup_heredoc_signals(void (**old_handler)(int))
{
	*old_handler = signal(2, heredoc_signal_handler);
}

void	restore_heredoc_signals(void (*old_handler)(int))
{
	int	new_stdin;

	signal(2, old_handler);
	if (g_signal == 2)
	{
		new_stdin = open("/dev/tty", O_RDONLY);
		if (new_stdin != -1)
			dup2(new_stdin, STDIN_FILENO);
		if (new_stdin > 2)
			close(new_stdin);
	}
	g_signal = 0;
}

int	check_heredoc_signal(t_exec_context *ctx)
{
	if (g_signal == 2)
	{
		ctx->exit_status = 130;
		return (130);
	}
	return (0);
}
