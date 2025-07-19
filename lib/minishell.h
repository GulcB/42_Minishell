/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:54:46 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/19 18:46:51 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include "token.h"
# include "lexer.h"
# include "parser.h"
# include "executor.h"
# include "garbage_collector.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct	s_minishell
{
	
}				t_minishell;

int				handle_input_validation(char *input);
int				process_input_tokens(char *input, t_token **tokens, 
					t_exec_context *ctx);
void			execute_and_cleanup(t_token *tokens, char *input,
					t_exec_context *ctx);
void			print_banner(void);
void			free_env(t_env *env);

#endif