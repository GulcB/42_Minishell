/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:20:14 by gbodur            #+#    #+#             */
/*   Updated: 2025/06/20 16:59:41 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_token *tokens;

	(void)ac;
	(void)av;
	(void)env;
	
	while (1) 
	{
		input = readline("minishell> ");
		if (input == NULL) 
		{
			printf("exit\n");
			break; 
		}
		
		if (input && *input)
			add_history(input);
			
		if (ft_strlen(input) == 0)
		{
			t_token *eof_token = token_create(TOKEN_EOF, "", 0);
			if (!eof_token)
			{
				free(input);
				continue; 
			}
			tokens = eof_token;
		}
		else
		{
			tokens = lexer_tokenize(input);
			if (!tokens)
			{
				printf("Token listesi boş.\n");
				free(input);
				continue; 
			}
		}
		printf("Token Listesi:\n");
		printf("------------------------------------\n");
		token_list_print(tokens);
		printf("------------------------------------\n");
		free(input);
	}
	
	return (0);
}
