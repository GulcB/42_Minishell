/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 09:06:14 by gbodur            #+#    #+#             */
/*   Updated: 2025/06/23 09:30:59 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PARSER_H
# define	PARSER_H

# include	"../lexer/incs/token.h"

typedef enum			e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIRECT,
	NODE_AND,
	NODE_OR
}						t_node_type;

typedef enum			e_redirect_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC
}						t_redirect_type;

typedef struct			s_ast_node
{
	t_node_type	type;
	char				**args;
	char				*redirect_file;
	t_node_type			redirect_type;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}						t_ast_node;

t_ast_node  *parse_tokens(t_token *tokens);
t_ast_node  *parse_pipe(t_token **current);
t_ast_node  *parse_command(t_token **current);
t_ast_node  *parse_redirect(t_token **current);
char       	**parse_arguments(t_token **current);
t_ast_node  *create_ast_node(t_node_type type);
void         free_ast(t_ast_node *node);

#endif