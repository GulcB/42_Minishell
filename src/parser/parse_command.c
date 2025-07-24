/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 21:46:01 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/24 10:28:51 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "executor.h"

static int	count_arguments(t_token *current)
{
	int	count;

	count = 0;
	while (current && is_word_token(current) && !is_stop_token(current))
	{
		count++;
		current = current->next;
	}
	return (count);
}

static void	free_args_on_error(t_gc *gc, char **args, int count)
{
	int	i;

	if (!args || !gc)
		return ;
	i = 0;
	while (i < count)
	{
		if (args[i])
			gc_free(gc, args[i]);
		i++;
	}
	gc_free(gc, args);
}

static char	*expand_token_value(t_token *token, struct s_exec_context *ctx)
{
	char	*expanded_value;

	if (!token || !token->value)
		return (NULL);
	if (token->type == TOKEN_DOLLAR)
	{
		expanded_value = get_env_value(token->value, ctx);
		if (!expanded_value)
			return (ft_strdup(""));
		return (expanded_value);
	}
	else if (token->type == TOKEN_DQUOTE)
		return (expand_variables(token->value, ctx));
	else if (token->type == TOKEN_SQUOTE)
		return (ft_strdup(token->value));
	else
		return (expand_variables(token->value, ctx));
}

char	**parse_arguments(t_token **current, struct s_exec_context *ctx)
{
	int		count;
	int		i;
	char	**args;
	char	*expanded_value;
	t_gc	*gc;

	if (!current || !*current || !ctx || !ctx->gc)
		return (NULL);
	gc = ctx->gc;
	count = count_arguments(*current);
	if (count == 0)
		return (NULL);
	args = (char **)gc_malloc(gc, sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (i < count && *current && is_word_token(*current))
	{
		expanded_value = expand_token_value(*current, ctx);
		if (!expanded_value)
			return (free_args_on_error(gc, args, i), NULL);
		args[i] = expanded_value;
		*current = (*current)->next;
		i++;
	}
	args[count] = NULL;
	return (args);
}

// static int	execute_command_with_redirect(t_ast_node *cmd_node,
// 		t_ast_node *redirect_node, t_exec_context *ctx)
// {
// 	int	result;

// 	if (execute_redirect(redirect_node, ctx) != 0)
// 		return (1);
// 	result = execute_command(cmd_node, ctx);
// 	restore_std_fds(ctx);
// 	return (result);
// }

t_ast_node	*parse_command(t_token **current, struct s_exec_context *ctx)
{
	t_ast_node	*cmd_node;
	t_ast_node	*redirect_node;
	char		**args;
	t_gc		*gc;

	if (!current || !*current || !ctx || !ctx->gc)
		return (NULL);
	gc = ctx->gc;
	if (is_redirect_token(*current))
		return (parse_redirect(current, ctx));
	if (!is_word_token(*current))
		return (NULL);
	args = parse_arguments(current, ctx);
	if (!args)
		return (NULL);
	cmd_node = create_command_node(gc, args);
	if (!cmd_node)
		return (free_args_array(gc, args), NULL);
	if (*current && is_redirect_token(*current))
	{
		redirect_node = parse_redirect(current, ctx);
		if (!redirect_node)
			return (cmd_node);
		cmd_node->right = redirect_node;
	}
	return (cmd_node);
}
