/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_counting.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:01 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 20:54:05 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"

int	get_value_length(char *value)
{
	if (value)
		return (ft_strlen(value));
	return (0);
}

static void	skip_redirect_tokens(t_token **temp)
{
	*temp = (*temp)->next;
	if (*temp && is_word_token(*temp))
		*temp = (*temp)->next;
}

static void	skip_adjacent_word_tokens(t_token **temp)
{
	while (*temp && (*temp)->next && is_word_token((*temp)->next)
		&& !is_stop_token((*temp)->next) && !is_redirect_token((*temp)->next)
		&& parse_tokens_are_adjacent(*temp, (*temp)->next))
		*temp = (*temp)->next;
	*temp = (*temp)->next;
}

int	count_command_args(t_token *start_token)
{
	t_token	*temp;
	int		arg_count;

	arg_count = 0;
	temp = start_token;
	while (temp && !is_stop_token(temp))
	{
		if (is_redirect_token(temp))
			skip_redirect_tokens(&temp);
		else if (is_word_token(temp))
		{
			arg_count++;
			skip_adjacent_word_tokens(&temp);
		}
		else
			temp = temp->next;
	}
	return (arg_count);
}

int	count_arguments(t_token *current)
{
	int	count;

	count = 0;
	while (current && !is_stop_token(current))
	{
		if (is_word_token(current))
		{
			if (current->prev && is_redirect_token(current->prev))
			{
				current = current->next;
				continue ;
			}
			count++;
			while (current && current->next && is_word_token(current->next)
				&& !is_stop_token(current->next)
				&& parse_tokens_are_adjacent(current, current->next))
			{
				current = current->next;
			}
		}
		current = current->next;
	}
	return (count);
}
