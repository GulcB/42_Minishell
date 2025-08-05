/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:20:14 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/05 09:53:09 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_signal = 0;

static t_gc	*init_main_gc(void)
{
	t_gc	*main_gc;

	main_gc = gc_init_auto();
	if (!main_gc)
	{
		write(STDERR_FILENO, "Error: Failed to initialize garbage collector\n",
			47);
		return (NULL);
	}
	return (main_gc);
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

static int	process_shell_input(t_exec_context *ctx, t_gc *main_gc)
{
	char		*input;
	t_token		*tokens;
	int			process_result;
	extern int	g_signal;

	setup_main_signals();
	g_signal = 0;
	input = readline("minishell> ");
	handle_signal_in_loop(ctx);
	if (input == NULL)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (0);
	}
	ignore_signals();
	process_result = handle_input_processing(input, &tokens, main_gc);
	if (process_result == -1)
		return (1);
	if (process_result == 0)
		return (0);
	handle_execution_exit(tokens, input, ctx, main_gc);
	return (1);
}

static void	shell_loop(t_env *env, t_gc *main_gc)
{
	t_exec_context	*ctx;
	int				continue_loop;

	ctx = init_exec_context(env, main_gc);
	if (!ctx)
		return ;
	while (1)
	{
		continue_loop = process_shell_input(ctx, main_gc);
		if (continue_loop == 0)
			break ;
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
