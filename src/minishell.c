/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:20:14 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/20 20:55:46 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

static void	setup_signals(void)
{
	printf("[DEBUG: About to setup signals]\n");
	setup_interactive_signals();
	printf("[DEBUG: Signals setup complete]\n");
}

static t_gc	*init_main_gc(void)
{
	t_gc	*main_gc;

	main_gc = gc_init();
	if (!main_gc)
	{
		ft_putstr_fd("Error: Failed to initialize garbage collector\n",
			STDERR_FILENO);
		return (NULL);
	}
	return (main_gc);
}

static void	shell_loop(t_env *env, t_gc *main_gc)
{
	char			*input;
	t_token			*tokens;
	t_exec_context	*ctx;
	int				input_status;

	rl_catch_signals = 0;
	rl_catch_sigwinch = 0;
	ctx = init_exec_context(env, main_gc);
	if (!ctx)
		return ;
	while (1)
	{
		if (g_signal == SIGINT)
		{
			printf("\n");
			g_signal = 0;
		}
		input = readline("minishell> ");
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			if (input)
				free(input);
			continue ;
		}
		input_status = handle_input_validation(input);
		if (input_status == 0)
			break ;
		if (input_status == -1)
			continue ;
		if (!process_input_tokens(input, &tokens))
			continue ;
		execute_and_cleanup(tokens, input, ctx);
		reset_signal_flag();
	}
	cleanup_exec_context(ctx);
}

static void	cleanup_and_exit(t_env *environment, t_gc *main_gc)
{
	free_env(environment);
	gc_cleanup_all(main_gc);
	gc_destroy(main_gc);
	rl_clear_history();
	// clear_history();
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
	environment = init_env_from_system(env);
	if (!environment)
	{
		ft_putstr_fd("Error: Failed to initialize environment\n",
			STDERR_FILENO);
		gc_destroy(main_gc);
		return (1);
	}
	print_banner();
	setup_signals();
	shell_loop(environment, main_gc);
	cleanup_and_exit(environment, main_gc);
	return (0);
}
