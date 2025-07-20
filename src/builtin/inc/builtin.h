/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:45:19 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/20 21:52:52 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "libft.h"
# include <unistd.h>
# include <stdlib.h>

# define SHELL_EXIT -42

struct	s_exec_context;
struct	s_env;

int		builtin_echo(char **args);
int		builtin_pwd(char **args);
int		builtin_env(char **args, struct s_env *env);
int		builtin_exit(char **args, struct s_exec_context *ctx);
int		builtin_cd(char **args, struct s_exec_context *ctx);
int		builtin_export(char **args, struct s_exec_context *ctx);
int		builtin_unset(char **args, struct s_exec_context *ctx);
int		builtin_exit(char **args, struct s_exec_context *ctx);
int		is_builtin_command(const char *cmd);
int		execute_builtin_dispatcher(char **args, struct s_exec_context *ctx);

#endif