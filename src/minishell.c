/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:20:14 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/18 00:49:31 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

int	g_signal = 0;

static void	setup_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

static void	print_banner(void)
{
	printf("╔══════════════════════════════════════╗\n");
	printf("║           🐚 Minishell 🐚            ║\n");
	printf("║        As beautiful as a shell       ║\n");
	printf("╚══════════════════════════════════════╝\n\n");
}

void	free_env(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		if (current->key)
			gc_free(current->key);
		if (current->value)
			gc_free(current->value);
		gc_free(current);
		current = next;
	}
}

static void	shell_loop(t_env *env)
{
	char			*input;
	t_token			*tokens;
	t_ast_node		*ast;
	t_exec_context	*ctx;

	ctx = init_exec_context(env);
	if (!ctx)
		return ;
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		if (ft_strlen(input) == 0)
		{
			free(input);
			continue ;
		}
		tokens = lexer_tokenize(input);
		if (!tokens)
		{
			free(input);
			continue ;
		}
		ast = parse_tokens(tokens);
		if (ast)
		{
			print_ast(ast);
			free_ast(ast);
		}
		token_free_list(tokens);
		free(input);
		gc_cleanup();
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
		printf("Error: Failed to initialize environment\n");
		return (1);
	}
	print_banner();
	setup_signals();
	shell_loop(environment);
	free_env(environment);
	gc_cleanup();
	clear_history();
	// rl_clear_history();
	return (0);
}

// int	main(int ac, char **av, char **env)
// {
// 	char	*input;
// 	t_token *tokens;

// 	(void)ac;
// 	(void)av;
// 	(void)env;
	
// 	while (1) 
// 	{
// 		input = readline("minishell> ");
// 		if (input == NULL) 
// 		{
// 			printf("exit\n");
// 			break ; 
// 		}
// 		if (input && *input)
// 			add_history(input);
// 		if (ft_strlen(input) == 0)
// 		{
// 			t_token *eof_token = token_create(TOKEN_EOF, "", 0);
// 			if (!eof_token)
// 			{
// 				free(input);
// 				continue; 
// 			}
// 			tokens = eof_token;
// 		}
// 		else
// 		{
// 			tokens = lexer_tokenize(input);
// 			if (!tokens)
// 			{
// 				printf("Token listesi boş.\n");
// 				free(input);
// 				continue; 
// 			}
// 		}
// 		printf("Token List:\n");
// 		printf("------------------------------------\n");
// 		token_list_print(tokens);
// 		printf("------------------------------------\n");
// 		free(input);
// 	}
// 	return (0);
// }