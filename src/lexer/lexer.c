/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 09:27:54 by mdivan            #+#    #+#             */
/*   Updated: 2025/07/14 23:41:42 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/lexer.h"

t_lexer	*lexer_create(char *input)
{
	t_lexer	*lexer;

	if (!input)
		return (NULL);
	lexer = (t_lexer *)gc_malloc(sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->input = ft_strdup(input);
	lexer->position = 0;
	lexer->read_position = 0;
	lexer->current_char = '\0';
	lexer_read_char(lexer);
	return (lexer);
}

void	lexer_free(t_lexer *lexer)
{
	if (!lexer)
		return ;
	if (lexer->input)
		gc_free(lexer->input);
	gc_free(lexer);	
}

void	lexer_read_char(t_lexer *lexer)
{
	if (lexer->read_position >= (int)ft_strlen(lexer->input))
		lexer->current_char = '\0';
	else
		lexer->current_char = lexer->input[lexer->read_position];
	lexer->position = lexer->read_position;
	lexer->read_position++;
}

char	lexer_peek_char(t_lexer *lexer)
{
	if (lexer->read_position >= (int)ft_strlen(lexer->input))
		return ('\0');
	return (lexer->input[lexer->read_position]);
}

t_token	*create_heredoc_token(t_lexer *lexer, int start_pos)
{
	char		*delimiter;
	t_token		*token;

	lexer_read_char(lexer);
	lexer_read_char(lexer);
	delimiter = read_heredoc_delimiter(lexer);
	if (!delimiter)
		return (token_create(TOKEN_ERROR, "Invalid heredoc delimiter", start_pos));
	token = token_create(TOKEN_HEREDOC, delimiter, start_pos);
	gc_free(delimiter);
	return (token);
}

t_token	*lexer_next_token(t_lexer *lexer)
{
	t_token			*token;
	char			*value;
	t_token_type	type;
	int				start_pos;

	if (!lexer)
		return (NULL);
	skip_whitespace(lexer);
	if (lexer->current_char == '\0')
		return (token_create(TOKEN_EOF, "", lexer->position));
	if (lexer->current_char == '\\')
		lexer_read_char(lexer);
	start_pos = lexer->position;
	type = get_token_type(lexer->current_char, lexer_peek_char(lexer));
	if (type == TOKEN_PIPE)
	{
		token = token_create(TOKEN_PIPE, "|", start_pos);
		lexer_read_char(lexer);
	}
	else if (type == TOKEN_OR)
	{
		token = token_create(TOKEN_OR, "||", start_pos);
		lexer_read_char(lexer);
		lexer_read_char(lexer);
	}
	else if (type == TOKEN_REDIR_IN)
	{
		token = token_create(TOKEN_REDIR_IN, "<", start_pos);
		lexer_read_char(lexer);
	}
	else if (type == TOKEN_HEREDOC)
	{
		token = token_create(TOKEN_HEREDOC, "<<", start_pos);
		lexer_read_char(lexer);
		lexer_read_char(lexer);
	}
	else if (type == TOKEN_REDIR_OUT)
	{
		token = token_create(TOKEN_REDIR_OUT, ">", start_pos);
		lexer_read_char(lexer);
	}
	else if (type == TOKEN_APPEND)
	{
		token = token_create(TOKEN_APPEND, ">>", start_pos);
		lexer_read_char(lexer);
		lexer_read_char(lexer);
	}
	else if (type == TOKEN_AND)
	{
		token = token_create(TOKEN_AND, "&&", start_pos);
		lexer_read_char(lexer);
		lexer_read_char(lexer);
	}
	else if (type == TOKEN_SEMICOLON)
	{
		token = token_create(TOKEN_SEMICOLON, ";", start_pos);
		lexer_read_char(lexer);
	}
	else if (type == TOKEN_DQUOTE)
	{
		value = read_double_quoted_string(lexer);
		if (!value)
			return (token_create(TOKEN_ERROR, "Unclosed double quote", start_pos));
		token = token_create(TOKEN_DQUOTE, value, start_pos);
		gc_free(value);
	}
	else if (type == TOKEN_SQUOTE)
	{
		value = read_single_quoted_string(lexer);
		if (!value)
			return (token_create(TOKEN_ERROR, "Unclosed single quote", start_pos));
		token = token_create(TOKEN_SQUOTE, value, start_pos);
		gc_free(value);
	}
	else if (type == TOKEN_DOLLAR)
	{
		token = token_create(TOKEN_DOLLAR, "$", start_pos);
		lexer_read_char(lexer);
	}
	else
	{
		value = read_word(lexer);
		if (!value)
			return (token_create(TOKEN_ERROR, "Invalid token", start_pos));
		token = token_create(TOKEN_WORD, value, start_pos);
		gc_free(value);
	}
	return (token);
}

t_token	*lexer_tokenize(char *input)
{
	t_lexer	*lexer;
	t_token	*token_list;
	t_token	*current_token;
	t_token	*prev_token;

	lexer = lexer_create(input);
	if (!lexer)
		return (NULL);
	token_list = NULL;
	while (1)
	{
		current_token = lexer_next_token(lexer);
		if (!current_token)
			break ;
		if (!is_valid_token_sequence(prev_token, current_token))
		{
			if (current_token->type == TOKEN_ERROR)
			{
				token_free_list(current_token);
				continue ;
			}
		}
		token_add_back(&token_list, current_token);
		if (current_token->type == TOKEN_EOF
			|| current_token->type == TOKEN_ERROR)
			break ;
	}
	return (token_list);
}
