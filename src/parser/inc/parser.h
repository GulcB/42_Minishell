/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 09:06:14 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/06 22:51:31 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PARSER_H
# define PARSER_H

# include"../../lexer/incs/token.h"
# include"../../lexer/incs/lexer.h"

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
	t_node_type			type;
	char				**args;
	char				*redirect_file;
	t_node_type			redirect_type;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}						t_ast_node;

void		write_str(char *str);
void		print_ast_recursive(t_ast_node *node, int depth);
void		print_ast(t_ast_node *node);

void		free_args_array(char **args);
void		free_ast(t_ast_node *node);
t_ast_node	*create_ast_node(t_node_type type);
t_ast_node	*create_command_node(char **args);

char       	**parse_arguments(t_token **current);
t_ast_node  *parse_command(t_token **current);

int			is_redirect_token(t_token *token);
t_ast_node	*create_redirect_node(t_redirect_type type, char *filename);
t_ast_node  *parse_redirect(t_token **current);

int			is_word_token(t_token *token);
int			is_stop_token(t_token *token);
t_ast_node  *parse_pipe(t_token **current);
t_ast_node	*parse_logical_and(t_token **current);
t_ast_node  *parse_tokens(t_token *tokens);

void		print_args(char **args);
void		write_redirect_type(t_redirect_type type);
int			validate_ast(t_ast_node *node);







void	test_parse_arguments_debug(void);

#endif