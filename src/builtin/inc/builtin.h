/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:45:19 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 21:08:28 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "garbage_collector.h"
# include "libft.h"
# include <limits.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define SHELL_EXIT -42

struct	s_exec_context;
struct	s_env;

typedef struct s_builtin_cmd
{
	char			**args;
	struct s_env	*env;
	int				*exit_status;
	t_gc			*gc;
}					t_builtin_cmd;

int					execute_builtin_dispatcher(char **args,
						struct s_exec_context *ctx);
int					is_builtin_command(const char *cmd);
int					builtin_cd(t_builtin_cmd *cmd);
int					builtin_echo(t_builtin_cmd *cmd);
int					builtin_env(t_builtin_cmd *cmd);
int					builtin_exit(t_builtin_cmd *cmd);
char				*trim_quotes(const char *s);
int					check_digits_only(const char *trimmed, int start_idx);
int					check_overflow(long result, int sign);
int					parse_and_convert(const char *str, int sign, int *out);
int					builtin_export(t_builtin_cmd *cmd);
int					builtin_pwd(t_builtin_cmd *cmd);
int					env_remove(struct s_env **env, const char *key, t_gc *gc);
int					builtin_unset(t_builtin_cmd *cmd);

#endif
