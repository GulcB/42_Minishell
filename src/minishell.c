/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:20:14 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/31 19:08:40 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

static void	configure_readline(void)
{
	rl_catch_signals = 0;
	rl_catch_sigwinch = 0;
	rl_clear_signals();
	rl_set_signals();
}

static t_gc	*init_main_gc(void)
{
	t_gc	*main_gc;

	main_gc = gc_init_auto();
	if (!main_gc)
	{
		write(STDERR_FILENO, "Error: Failed to initialize garbage collector\n", 47);
		return (NULL);
	}
	return (main_gc);
}

static void	safe_cleanup_and_exit(t_gc *main_gc)
{
	if (main_gc)
	{
		gc_destroy(main_gc);
	}
	rl_clear_history();
	rl_cleanup_after_signal();
}

static void	handle_signal_in_loop(t_exec_context *ctx)
{
	extern int	g_signal;

	if (g_signal == SIGINT)
	{
		ctx->exit_status = 130;
		env_set(ctx->env, "?", "130", ctx->gc);
		g_signal = 0;
	}
}

static void	shell_loop(t_env *env, t_gc *main_gc)
{
	char			*input;
	t_token			*tokens;
	t_exec_context	*ctx;
	int				input_status;
	extern int		g_signal;

	ctx = init_exec_context(env, main_gc);
	if (!ctx)
		return ;
	while (1)
	{
		setup_main_signals();
		g_signal = 0;
		input = readline("minishell> ");
		handle_signal_in_loop(ctx);
		if (input == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		ignore_signals();
		input_status = handle_input_validation(input);
		if (input_status == -1)
			continue ;
		if (input_status == 0)
			break ;
		if (!process_input_tokens(input, &tokens, main_gc))
			continue ;
		if (execute_and_cleanup(tokens, input, ctx) == -42)
		{
			restore_std_fds(ctx);
			gc_cleanup_all(main_gc);
			safe_cleanup_and_exit(main_gc);
			exit(ctx->exit_status);
		}
	}
	restore_std_fds(ctx);
	gc_cleanup_all(main_gc);
}

int	main(int ac, char **av, char **env)
{
	t_env	*environment;
	t_gc	*main_gc;

	(void)ac;
	(void)av;
	main_gc = init_main_gc();
	if (!main_gc)
		return (1);
	environment = init_env_from_system_with_gc(main_gc, env);
	if (!environment)
	{
		write(STDERR_FILENO, "Error: Failed to initialize environment\n", 41);
		gc_destroy(main_gc);
		return (1);
	}
	configure_readline();
	shell_loop(environment, main_gc);
	safe_cleanup_and_exit(main_gc);
	return (0);
}
