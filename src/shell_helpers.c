/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:17:34 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/20 20:29:09 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input_validation(char *input)
{
	if (!input)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		return (0);
	}
	if (*input)
		add_history(input);
	if (ft_strlen(input) == 0)
	{
		free(input);
		return (-1);
	}
	return (1);
}

int	process_input_tokens(char *input, t_token **tokens)
{
	*tokens = lexer_tokenize(input);
	if (!*tokens)
	{
		free(input);
		return (0);
	}
	printf("\n=== TOKEN DEBUG ===\n");
	token_list_print(*tokens);
	printf("=== END TOKENS ===\n\n");
	return (1);
}

void	execute_and_cleanup(t_token *tokens, char *input, t_exec_context *ctx)
{
	t_ast_node	*ast;

	ast = parse_tokens_with_context(tokens, ctx);
	if (ast)
	{
		print_ast(ast);
		execute_ast(ast, ctx);
	}
	free(input);
}

void	print_banner(void)
{
	ft_putstr_fd("╔══════════════════════════════════════╗\n", STDOUT_FILENO);
	ft_putstr_fd("║           🐚 Minishell 🐚            ║\n", STDOUT_FILENO);
	ft_putstr_fd("║        As beautiful as a shell       ║\n", STDOUT_FILENO);
	ft_putstr_fd("╚══════════════════════════════════════╝\n\n", STDOUT_FILENO);
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
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}
