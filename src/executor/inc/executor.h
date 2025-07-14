/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:06:34 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/15 01:46:13 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include	"../../parser/inc/parser.h"
# include "../../garbage_collector/inc/garbage_collector.h"
# include <sys/wait.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>

# define MAX_CHILDREN 1024
# define PATH_MAX_LEN 4096

extern int	g_signal;

typedef struct		s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct 		s_exec_context
{
    t_env   		*env;
    int     		exit_status;
    int     		stdin_backup;
    int     		stdout_backup;
	int				pipe_fd[2];
	int				has_active_pipe;
	pid_t			*child_pids;
	int				child_count;
	int				max_children;
}					t_exec_context;

void				setup_parent_signals(void);
void				restore_parent_signals(void);
int					execute_ast(t_ast_node *ast, t_exec_context *ctx);

int					execute_command(t_ast_node *cmd_node, t_exec_context *ctx);

void				cleanup_exec_context(t_exec_context *ctx);

void				cleanup_exec_context(t_exec_context *ctx);

t_exec_context		*init_exec_context(t_env *env);
int					backup_std_fds(t_exec_context *ctx);
void				restore_std_fds(t_exec_context *ctx);

t_env				*init_env_from_system(char **env_array);
char				*env_get(t_env *env, const char *key);

int					extract_exit_code(int status);
int					extract_signal_number(int status);
int					is_normal_exit(int status);
int					is_signaled_exit(int status);

void				setup_parent_signals(void);
int					execute_ast(t_ast_node *ast, t_exec_context *ctx);

// void				init_exec_context(t_exec_context *ctx);
// int					execute_command(t_ast_node *node, t_exec_context *ctx);
// int					execute_logical_and(t_ast_node *node, t_exec_context *ctx);
// int					execute_logical_or(t_ast_node *node, t_exec_context *ctx);
// int					handle_redirection_node(t_ast_node *ast, t_exec_context *ctx);

int					add_child_pid(t_exec_context *ctx, pid_t pid);
int					wait_for_children(t_exec_context *ctx);
void				update_exit_status(t_exec_context *ctx, int status);
void				cleanup_children(t_exec_context *ctx);

char				*resolve_executable(const char *cmd, t_env *env);
int					is_executable_file(const char *path);
char				*search_in_path(const char *cmd, const char *path_env);

#endif