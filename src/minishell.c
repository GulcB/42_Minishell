/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:20:14 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/19 15:20:31 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

static void	setup_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

static void	shell_loop(t_env *env)
{
	char			*input;
	t_token			*tokens;
	t_exec_context	*ctx;
	int				input_status;

	ctx = init_exec_context(env);
	if (!ctx)
		return ;
	while (1)
	{
		input = readline("minishell> ");
		input_status = handle_input_validation(input);
		if (input_status == 0)
			break ;
		if (input_status == -1)
			continue ;
		if (!process_input_tokens(input, &tokens))
			continue ;
		execute_and_cleanup(tokens, input);
	}
	cleanup_exec_context(ctx);
}

int	main(int ac, char **av, char **env)
{
	t_env	*environment;

	(void)ac;
	(void)av;
	gc_init();
	environment = init_env_from_system(env);
	if (!environment)
	{
		ft_putstr_fd("Error: Failed to initialize environment\n",
			STDERR_FILENO);
		return (1);
	}
	print_banner();
	setup_signals();
	shell_loop(environment);
	free_env(environment);
	gc_cleanup();
	rl_clear_history();
	// clear_history();
	return (0);
}
