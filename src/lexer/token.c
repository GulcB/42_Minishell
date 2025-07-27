/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 09:46:32 by mdivan            #+#    #+#             */
/*   Updated: 2025/07/27 18:29:46 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

t_token	*token_create(t_gc *gc, t_token_type type, char *value, int position)
{
	t_token	*token;

	token = (t_token *)gc_malloc(gc, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (value)
		token->value = gc_strdup(gc, value);
	else
		token->value = NULL;
	token->position = position;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

void	token_add_back(t_token **tokens, t_token *new_token)
{
	t_token	*last;

	if (!tokens || !new_token)
		return ;
	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	last = *tokens;
	while (last->next)
		last = last->next;
	last->next = new_token;
	new_token->prev = last;
}

void	token_free_list(t_gc *gc, t_token *head)
{
	t_token	*current;
	t_token	*temp;

	if (!gc)
		return ;
	current = head;
	while (current)
	{
		temp = current->next;
		if (current->value)
			gc_free(gc, current->value);
		gc_free(gc, current);
		current = temp;
	}
}

char	*token_type_to_str(t_token_type type)
{
	static char	*types[] = {"WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "APPEND",
		"HEREDOC", "AND", "OR", "SEMICOLON", "DQUOTE", "SQUOTE", "DOLLAR",
		"ERROR", "EOF"};

	if (type >= 0 && type <= TOKEN_EOF)
		return (types[type]);
	return ("UNKNOWN");
}

void	token_list_print(t_token *head)
{
	t_token	*current;
	int		token_count;
	char	*display_value;

	token_count = 0;
	current = head;
	printf("\033[1;36m┌───────────────────────────────────────────────────────┐\033[0m\n");
	printf("\033[1;36m│ \033[1;33m%-15s\033[0m \033[1;36m│ \033[1;33m%-15s\033[0m \033[1;36m│ \033[1;33m%-10s\033[0m \033[1;36m│\033[0m\n",
		"TYPE", "VALUE", "POSITION");
	printf("\033[1;36m├───────────────────┬───────────────────┬────────────────┤\033[0m\n");
	while (current)
	{
		token_count++;
		if (current->value)
			display_value = current->value;
		else
			display_value = "NULL";
		printf("\033[1;36m│ \033[1;32m%-15s\033[0m \033[1;36m│ \033[1;37m%-15s\033[0m \033[1;36m│ \033[1;35m%-10d\033[0m \033[1;36m│\033[0m\n",
			token_type_to_str(current->type),
			display_value, current->position);
		current = current->next;
	}
	printf("\033[1;36m└───────────────────┴───────────────────┴────────────────┘\033[0m\n");
	printf("\033[1;33mToplam %d token bulundu.\033[0m\n", token_count);
}
