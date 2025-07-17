/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:45:19 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/18 01:37:48 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../../../lib/libft/libft.h"
# include <unistd.h>
# include <stdlib.h>

int		builtin_echo(char **args);
int		builtin_pwd(char **args);
int		builtin_env(char **args, t_env *env);
int		builtin_cd(char **args, t_exec_context *ctx);
int		builtin_export(char **args, t_exec_context *ctx);
int		builtin_unset(char **args, t_exec_context *ctx);
int		builtin_exit(char **args, t_exec_context *ctx);
int		is_builtin_command(const char *cmd);
int		execute_builtin_dispatcher(char **args, t_exec_context *ctx);

#endif