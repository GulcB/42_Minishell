/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:20:14 by gbodur            #+#    #+#             */
/*   Updated: 2025/04/27 10:54:49 by mdivan           ###   ########.fr       */
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
	input = readline("minishell> ");
	if (input == NULL)
	{
		printf("exit\n");
		return (0);
	}
    
    if (!input)
        return (1);
    
    if (ft_strlen(input) == 0)
    {
        t_token *eof_token = token_create(TOKEN_EOF, "", 0);
        if (!eof_token)
            return (1);
        tokens = eof_token;
    }
    else
    {
        tokens = lexer_tokenize(input);
        if (!tokens)
        {
            printf("Token listesi boş.\n");
            free(input);
            return (1);
        }
    }
    
    printf("Token Listesi:\n");
    printf("------------------------------------\n");
    token_list_print(tokens);
    printf("------------------------------------\n");
	free(input);
	return (0);
}
