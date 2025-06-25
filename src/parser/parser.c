/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:27:38 by gbodur            #+#    #+#             */
/*   Updated: 2025/06/24 13:21:49 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "inc/parser.h"

static int	is_word_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_WORD || token->type == TOKEN_DQUOTE
		|| token->type == TOKEN_SQUOTE);
}

static int	is_stop_token(t_token *token)
{
	if (!token)
		return (1);
	return (token->type == TOKEN_PIPE || token->type == TOKEN_REDIR_IN
		|| token->type == TOKEN_REDIR_OUT || token->type == TOKEN_APPEND
		|| token->type == TOKEN_HEREDOC || token->type == TOKEN_AND
		|| token->type == TOKEN_OR || token->type == TOKEN_SEMICOLON
		|| token->type == TOKEN_EOF || token->type == TOKEN_ERROR);
}

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

static void	free_args_on_error(char **args, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

char	**parse_arguments(t_token **current)
{
	char	**args;
	int		count;
	int		i;

	if (!current || !*current)
		return (NULL);
	printf("DEBUG - İlk token: type=%s, value='%s'\n", 
		token_type_to_str((*current)->type), 
		(*current)->value ? (*current)->value : "NULL");
	count = count_arguments(*current);
	printf("DEBUG - Count: %d\n", count);
	if (count == 0)
		return (NULL);
	args = (char **)malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (i < count && *current && is_word_token(*current))
	{
		printf("DEBUG - Copying token[%d]: '%s'\n", i, (*current)->value);
		args[i] = ft_strdup((*current)->value);
		if (!args[i])
		{
			free_args_on_error(args, i);
			return (NULL);
		}
		printf("DEBUG - Copied: '%s'\n", args[i]);
		*current = (*current)->next;
		i++;
	}
	args[count] = NULL;
	return (args);
}


// char **parse_arguments(t_token **current)
// {
// 	char	**args;
// 	int		count;
// 	int		i;

// 	if (!current || !*current)
// 		return (NULL);
// 	count = count_arguments(*current);
// 	if (count == 0)
// 		return (NULL);
// 	args = (char **)malloc(sizeof (char *) * (count - 1));
// 	if (!args)
// 		return (NULL);
// 	i = 0;
// 	while (i < count && *current && is_word_token(*current))
// 	{
// 		args[i] = ft_strdup((*current)->value);
// 		if (!args[i])
// 			return (free_args_on_error(args, i), NULL);
// 		*current = (*current)->next;
// 		i++;
// 	}
// 	args[count] = NULL;
// 	return (args);
// }
