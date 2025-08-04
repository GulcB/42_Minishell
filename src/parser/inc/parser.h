/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 09:06:14 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 18:14:55 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "token.h"
# include "lexer.h"
# include "garbage_collector.h"

struct	s_exec_context;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIRECT,
	NODE_AND,
	NODE_OR
}							t_node_type;

typedef enum e_redirect_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC
}							t_redirect_type;

typedef struct s_ast_node
{
	t_node_type				type;
	char					**args;
	char					*redirect_file;
	t_redirect_type			redirect_type;
	int						fd_num;
	struct s_ast_node		*left;
	struct s_ast_node		*right;
}							t_ast_node;

typedef struct s_parser_context
{
	t_gc					*gc;
	struct s_exec_context	*exec_ctx;
}							t_parser_context;

void						write_str(char *str);
void						print_ast_recursive(t_ast_node *node, int depth);
void						print_ast(t_ast_node *node);

void						free_args_array(t_gc *gc, char **args);
void						free_ast(t_gc *gc, t_ast_node *node);
t_ast_node					*create_ast_node(t_gc *gc, t_node_type type);
t_ast_node					*create_command_node(t_gc *gc, char **args);

char						**parse_arguments(t_token **current,
								struct s_exec_context *ctx);
t_ast_node					*parse_command(t_token **current,
								struct s_exec_context *ctx);

t_ast_node					*parse_heredoc(t_token **current,
								struct s_exec_context *ctx);

int							is_redirect_token(t_token *token);
t_ast_node					*create_redirect_node(t_gc *gc,
								t_redirect_type type, char *filename,
								int fd_num);
t_ast_node					*parse_redirect(t_token **current,
								struct s_exec_context *ctx);

int							is_word_token(t_token *token);
int							is_stop_token(t_token *token);

t_ast_node					*parse_pipe(t_token **current,
								struct s_exec_context *ctx);
t_ast_node					*parse_logical_and(t_token **current,
								struct s_exec_context *ctx);
t_ast_node					*parse_tokens_with_context(t_token *tokens,
								struct s_exec_context *ctx);

void						print_args(char **args);
void						write_redirect_type(t_redirect_type type);
int							validate_ast(t_ast_node *node);

#endif
