/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 18:08:17 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 14:40:16 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "executor.h"

extern int	g_signal;

void	handle_sigint_interactive(int sig)
{
	
	g_signal = sig;
	if (isatty(STDOUT_FILENO))
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		write(STDOUT_FILENO, "^C\n", 3);
		rl_redisplay();
	}
}

void	handle_sigint_exec(int sig)
{
	g_signal = sig;
	if (isatty(STDOUT_FILENO))
		write(STDOUT_FILENO, "\n", 1);
}

void	setup_main_signals(void)
{
	signal(SIGINT, handle_sigint_interactive);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_exec_signals(void)
{
	signal(SIGINT, handle_sigint_exec);
	signal(SIGQUIT, SIG_IGN);
}

void	ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
