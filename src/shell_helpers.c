/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:17:34 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/24 15:35:30 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input_validation(char *input)
{
	if (!input)
		return (0);
	if (*input)
		add_history(input);
	if (ft_strlen(input) == 0)
		return (-1);
	return (1);
}

int process_input_tokens(char *input, t_token **tokens, t_gc *main_gc)
{
    *tokens = lexer_tokenize_with_context(input, main_gc);
    if (!*tokens)
        return (0);
    token_list_print(*tokens);
    return (1);
}

int	execute_and_cleanup(t_token *tokens, char *input, t_exec_context *ctx)
{
	t_ast_node	*ast;
	int			result;

	ast = parse_tokens_with_context(tokens, ctx);
	result = 0;
	if (ast)
	{
		print_ast(ast);
		result = execute_ast(ast, ctx);
	}
	free(input);
	return (result);
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

	if (!env)
		return ;
	current = env;
	while (current)
	{
		next = current->next;
		if (current->key)
		{
			free(current->key);
			current->key = NULL;
		}
		if (current->value)
		{
			free(current->value);
			current->value = NULL;
		}
		free(current);
		current = next;
	}
}
