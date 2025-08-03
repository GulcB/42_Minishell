/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:06:34 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/03 12:00:53 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "parser.h"
# include "builtin.h"
# include "garbage_collector.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <dirent.h>

# define MAX_CHILDREN 1024
# define PATH_MAX_LEN 4096

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_exec_context
{
	t_env			*env;
	t_gc			*gc;
	int				exit_status;
	int				stdin_backup;
	int				stdout_backup;
	int				pipe_fd[2];
	int				has_active_pipe;
	pid_t			*child_pids;
	int				child_count;
	int				max_children;
}					t_exec_context;

int					execute_ast(t_ast_node *ast, t_exec_context *ctx);
int					execute_command(t_ast_node *cmd_node, t_exec_context *ctx);
int					execute_heredoc(t_ast_node *heredoc_node,
						t_exec_context *ctx);

void				cleanup_exec_context(t_exec_context *ctx);
t_exec_context		*init_exec_context(t_env *env, t_gc *gc);
int					backup_std_fds(t_exec_context *ctx);
void				restore_std_fds(t_exec_context *ctx);

void				env_add(t_gc *gc, t_env **env, const char *key,
						const char *value);
int					env_count(t_env *env);
char				*ft_strjoin_three(const char *s1, const char *s2,
						const char *s3);
char				**env_to_array(t_env *env, int count);
void				sort_env_array(char **array, int count);
void				env_set(t_env *env, const char *key, const char *value,
						t_gc *gc);

t_env				*create_env_node_classic(const char *key,
						const char *value);
t_env				*create_env_node(t_gc *gc, const char *key,
						const char *value);
t_env				*parse_and_create_node_classic(char *env_str,
						t_env **current);
t_env				*parse_and_create_node(t_gc *gc, char *env_str,
						t_env **current);
t_env				*find_env_node(t_env *env, const char *key);
t_env				*build_env_list(t_gc *gc, char **env_array);
t_env				*init_env_from_system_with_gc(t_gc *gc, char **env_array);
t_env				*build_env_list_classic(char **env_array);
t_env				*init_env_from_system(char **env_array);
char				*env_get(t_env *env, const char *key);

void				update_exit_status(t_exec_context *ctx, int status);
int					add_child_pid(t_exec_context *ctx, pid_t pid);
int					wait_for_children(t_exec_context *ctx);
void				cleanup_children(t_exec_context *ctx);

int					execute_pipe_chain(t_ast_node *pipe_node,
						t_exec_context *ctx);
int					count_pipe_commands(t_ast_node *node);
int					validate_pipe_chain(t_ast_node *node);
int					wait_for_pipe_children(t_exec_context *ctx);
int					execute_pipe(t_ast_node *pipe_node, t_exec_context *ctx);

char				*resolve_executable(t_exec_context *ctx, const char *cmd, t_env *env);
char				*expand_variables(const char *str,
						struct s_exec_context *ctx);

int					execute_redirection(t_ast_node *redirect_node,
						struct s_exec_context *ctx);

void			handle_sigint_interactive(int sig);
void			handle_sigint_exec(int sig);
void			setup_main_signals(void);
void			setup_exec_signals(void);
void			ignore_signals(void);
void			close_all_fds_except(int keep_fd1, int keep_fd2, int keep_fd3);

#endif