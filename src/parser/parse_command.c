/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 21:46:01 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/29 20:01:48 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "executor.h"

static int	tokens_are_adjacent(t_token *current, t_token *next)
{
	int	current_end;
	
	if (!current || !next)
		return (0);
	
	/* Calculate the end position of the current token */
	if (current->type == TOKEN_DQUOTE)
	{
		/* Double quoted strings: 2 quote chars + content length */
		current_end = current->position + 2 + (current->value ? ft_strlen(current->value) : 0);
	}
	else if (current->type == TOKEN_SQUOTE)
	{
		/* Single quoted strings: 2 quote chars + content length */
		current_end = current->position + 2 + (current->value ? ft_strlen(current->value) : 0);
	}
	else if (current->type == TOKEN_DOLLAR)
	{
		/* Check if this is a $"..." construct by looking for LITERAL: prefix */
		if (current->value && ft_strncmp(current->value, "LITERAL:", 8) == 0)
		{
			/* For $"content", the span is: $ + " + content + " = 3 + content_length */
			int content_len = ft_strlen(current->value + 8); /* Skip "LITERAL:" */
			current_end = current->position + 3 + content_len;
		}
		else if (current->value && ft_strlen(current->value) == 1 && 
			(current->value[0] == '?' || ft_isdigit(current->value[0])))
		{
			/* Special vars like $? or $1 span 2 characters */
			current_end = current->position + 2;
		}
		else
		{
			/* Regular variables like $USER span $ + variable_name */
			current_end = current->position + 1 + (current->value ? ft_strlen(current->value) : 0);
		}
	}
	else
	{
		/* Regular tokens - use the length of their value */
		current_end = current->position + (current->value ? ft_strlen(current->value) : 1);
	}
	return (current_end == next->position);
}

static int	count_arguments(t_token *current)
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
				continue;
			}
			count++;
			while (current && current->next && is_word_token(current->next) 
				&& !is_stop_token(current->next)
				&& tokens_are_adjacent(current, current->next))
			{
				current = current->next;
			}
		}
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
	char	*concatenated_arg;
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
	while (i < count && *current && !is_stop_token(*current))
	{
		if (!is_word_token(*current))
		{
			*current = (*current)->next;
			continue;
		}
		if ((*current)->prev && is_redirect_token((*current)->prev))
		{
			*current = (*current)->next;
			continue;
		}
		expanded_value = expand_token_value(*current, ctx);
		if (!expanded_value)
			return (free_args_on_error(gc, args, i), NULL);
		concatenated_arg = expanded_value;
		*current = (*current)->next;
		while (*current && is_word_token(*current) && !is_stop_token(*current)
			&& (*current)->prev && tokens_are_adjacent((*current)->prev, *current))
		{
			expanded_value = expand_token_value(*current, ctx);
			if (!expanded_value)
			{
				gc_free(gc, concatenated_arg);
				return (free_args_on_error(gc, args, i), NULL);
			}
			concatenated_arg = gc_strjoin(gc, concatenated_arg, expanded_value);
			gc_free(gc, expanded_value);
			if (!concatenated_arg)
				return (free_args_on_error(gc, args, i), NULL);
			*current = (*current)->next;
		}
		
		args[i] = concatenated_arg;
		i++;
	}
	args[count] = NULL;
	return (args);
}

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
	while (*current && is_redirect_token(*current))
	{
		redirect_node = parse_redirect(current, ctx);
		if (!redirect_node)
			break;
		if (!cmd_node->right)
			cmd_node->right = redirect_node;
		else
		{
			t_ast_node *last_redirect = cmd_node->right;
			while (last_redirect->right)
				last_redirect = last_redirect->right;
			last_redirect->right = redirect_node;
		}
	}
	return (cmd_node);
}
