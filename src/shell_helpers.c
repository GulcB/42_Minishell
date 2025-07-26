/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:17:34 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/26 19:26:33 by gbodur           ###   ########.fr       */
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

int	process_input_tokens(char *input, t_token **tokens, t_gc *main_gc)
{
	*tokens = lexer_tokenize_with_context(input, main_gc);
	if (!*tokens)
		return (0);
	// token_list_print(*tokens);
	return (1);
}

static void	update_exit_status_env(t_exec_context *ctx, int result)
{
	char	*exit_status_str;

	exit_status_str = ft_itoa(result);
	if (exit_status_str)
	{
		env_set(ctx->env, "?", exit_status_str, ctx->gc);
		free(exit_status_str);
	}
}

int	execute_and_cleanup(t_token *tokens, char *input, t_exec_context *ctx)
{
	t_ast_node	*ast;
	int			result;
	extern int	g_signal;

	ast = parse_tokens_with_context(tokens, ctx);
	result = 0;
	if (ast)
	{
		// print_ast(ast);
		result = execute_ast(ast, ctx);
		if (g_signal == SIGINT)
		{
			result = 130;
			ctx->exit_status = 130;
		}
		update_exit_status_env(ctx, result);
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
