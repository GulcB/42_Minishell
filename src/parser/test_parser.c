/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:49:56 by gbodur            #+#    #+#             */
/*   Updated: 2025/06/24 13:21:56 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/parser.h"
#include <stdio.h>

static void	debug_token_list(t_token *tokens)
{
	t_token	*current;
	int		i;

	current = tokens;
	i = 0;
	printf("=== TOKEN DEBUG ===\n");
	while (current)
	{
		printf("Token[%d]: type=%s, value='%s', pos=%d\n", 
			i, token_type_to_str(current->type), 
			current->value ? current->value : "NULL", 
			current->positon);
		current = current->next;
		i++;
	}
	printf("=== END DEBUG ===\n");
}

static void	test_single_case_debug(char *input)
{
	t_token	*tokens;
	t_token	*current;
	char	**args;

	printf("\n=== TESTING: '%s' ===\n", input);
	
	// 1. Tokenize
	tokens = lexer_tokenize(input);
	if (!tokens)
	{
		printf("ERROR: lexer_tokenize failed\n");
		return;
	}
	
	// 2. Debug token'ları
	debug_token_list(tokens);
	
	// 3. Parse arguments
	current = tokens;
	args = parse_arguments(&current);
	
	// 4. Sonuçları yazdır
	printf("Result args: ");
	if (!args)
	{
		printf("NULL\n");
	}
	else
	{
		int i = 0;
		while (args[i])
		{
			printf("[%s] ", args[i]);
			i++;
		}
		printf("\n");
	}
	
	if (current)
		printf("Next token: %s\n", token_type_to_str(current->type));
	else
		printf("Next token: NULL\n");
	
	printf("=== END TEST ===\n");
}

void	test_parse_arguments_debug(void)
{
	printf("=== PARSER DEBUG TEST ===\n");
	test_single_case_debug("echo hello world");
	test_single_case_debug("ls -la");
	printf("=== DEBUG TEST COMPLETED ===\n");
}
