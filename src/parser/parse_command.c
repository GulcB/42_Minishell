/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 21:46:01 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/17 23:57:09 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/parser.h"

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

static void	free_args_on_error(char **args, int count)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (i < count)
	{
		if (args[i])
			gc_free(args[i]);
		i++;
	}
	gc_free(args);
}

char **parse_arguments(t_token **current)
{
	int		count;
	int		i;
	char	**args;
	
	if (!current || !*current)
		return (NULL);
	count = count_arguments(*current);
	if (count == 0)
		return (NULL);
	args = (char **)gc_malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (i < count && *current && is_word_token(*current))
	{
		args[i] = ft_strdup((*current)->value);
		if (!args[i])
			return (free_args_on_error(args, i), NULL);
		*current = (*current)->next;
		i++;
	}
	args[count] = NULL;
	return (args);
}

t_ast_node	*parse_command(t_token **current)
{
	t_ast_node	*node;
	char		**args;

	if (!current || !*current)
		return (NULL);
	if (!is_word_token(*current))
		return (NULL);
	args = parse_arguments(current);
	if (!args)
		return (NULL);
	node = create_command_node(args);
	if (!node)
	{
		free_args_array(args);
		return (NULL);
	}
	return (node);
}
