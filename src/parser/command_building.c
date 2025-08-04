/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_building.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:01 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 20:54:06 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"

static void	free_args_on_error(char **args, int count, t_gc *gc)
{
	(void)args;
	(void)count;
	(void)gc;
}

static int	should_skip_token(t_token *current)
{
	if (!is_word_token(current))
		return (1);
	if (current->prev && is_redirect_token(current->prev))
		return (1);
	return (0);
}

static char	*process_current_token(t_token **current,
	struct s_exec_context *ctx)
{
	char	*expanded_value;
	char	*concatenated_arg;

	expanded_value = expand_token_value(*current, ctx);
	if (!expanded_value)
		return (NULL);
	concatenated_arg = expanded_value;
	*current = (*current)->next;
	while (*current && is_word_token(*current) && !is_stop_token(*current)
		&& (*current)->prev && parse_tokens_are_adjacent((*current)->prev,
			*current))
	{
		expanded_value = expand_token_value(*current, ctx);
		if (!expanded_value)
			return (NULL);
		concatenated_arg = gc_strjoin(ctx->gc, concatenated_arg,
				expanded_value);
		if (!concatenated_arg)
			return (NULL);
		*current = (*current)->next;
	}
	return (concatenated_arg);
}

static int	fill_args_array(char **args, int count, t_token **current,
		struct s_exec_context *ctx)
{
	char	*concatenated_arg;
	int		i;

	i = 0;
	while (i < count && *current && !is_stop_token(*current))
	{
		if (should_skip_token(*current))
		{
			*current = (*current)->next;
			continue ;
		}
		concatenated_arg = process_current_token(current, ctx);
		if (!concatenated_arg)
			return (0);
		args[i] = concatenated_arg;
		i++;
	}
	return (1);
}

char	**parse_arguments(t_token **current, struct s_exec_context *ctx)
{
	int		count;
	char	**args;

	if (!current || !*current || !ctx || !ctx->gc)
		return (NULL);
	count = count_arguments(*current);
	if (count == 0)
		return (NULL);
	args = (char **)gc_malloc(ctx->gc, sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	if (!fill_args_array(args, count, current, ctx))
		return (free_args_on_error(args, 0, ctx->gc), NULL);
	args[count] = NULL;
	return (args);
}
