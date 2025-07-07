/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:20:14 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/07 20:05:14 by gbodur           ###   ########.fr       */
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

static void	shell_loop(void)
{
	char		*input;
	t_token		*tokens;
	t_ast_node	*ast;
	
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
}


int	main(int ac, char **av, char **env)
{
	
	(void)ac;
	(void)av;
	(void)env;
	
	gc_init();
	print_banner();
	setup_signals();
	shell_loop();
	gc_cleanup();
	clear_history();
	// rl_clear_history();
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