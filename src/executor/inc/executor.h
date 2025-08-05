/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:06:34 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/05 09:38:22 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "builtin.h"
# include "garbage_collector.h"
# include "parser.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

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
int					preprocess_heredoc(t_ast_node *heredoc_node,
						t_exec_context *ctx);

void				heredoc_signal_handler(int sig);
void				setup_heredoc_signals(void (**old_handler)(int));
void				restore_heredoc_signals(void (*old_handler)(int));
int					check_heredoc_signal(t_exec_context *ctx);

char				*create_temp_filename(void);
int					is_delimiter_match(const char *line,
						const char *delimiter);
int					is_heredoc_quoted(t_ast_node *heredoc_node);
char				*read_heredoc_line(void);
int					handle_readline_result(char *line);

int					process_and_write_line(int fd, char *line, int quoted,
						t_exec_context *ctx);
int					process_heredoc_line(int fd, const char *delimiter,
						int quoted, t_exec_context *ctx);
int					write_heredoc_content(int fd, const char *delimiter,
						int quoted, t_exec_context *ctx);

int					create_heredoc_file(const char *filename,
						const char *delimiter, int quoted,
						t_exec_context *ctx);
int					setup_heredoc_input(const char *filename);

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

char				*resolve_executable(t_exec_context *ctx, const char *cmd,
						t_env *env);
char				*expand_variables(const char *str,
						struct s_exec_context *ctx);

int					execute_redirection(t_ast_node *redirect_node,
						struct s_exec_context *ctx);

char				**convert_env_to_array(t_env *env, t_gc *gc);
int					handle_executable_not_found(char **args);
int					execute_child_process(char *executable_path, char **args,
						char **env_array);
int					execute_external_command(char **args, t_exec_context *ctx);

int					execute_heredoc_consume_only(t_ast_node *heredoc_node,
						t_exec_context *ctx);
t_ast_node			*find_last_heredoc(t_ast_node *redirect_node);
void				process_non_last_heredocs(t_ast_node *redirect_node,
						t_ast_node *last_heredoc, t_exec_context *ctx);

void				setup_child_redirection(int input_fd, int output_fd);
void				close_fds_in_child(void);
void				execute_child_process_with_redirect(t_ast_node *node,
						t_exec_context *ctx, int input_fd, int output_fd);
int					process_command_heredocs(t_ast_node *node,
						t_exec_context *ctx);
int					process_pipe_heredocs(t_ast_node *node,
						t_exec_context *ctx);
int					preprocess_heredocs_in_pipe_chain(t_ast_node *node,
						t_exec_context *ctx);

int					is_directory(const char *path);
int					is_executable_file(const char *path);
char				*create_full_path(const char *dir, const char *cmd);
char				**get_path_directories(t_env *env);
void				free_path_dirs(char **path_dirs);
void				handle_directory_error(t_exec_context *ctx,
						const char *cmd);
void				handle_permission_error(t_exec_context *ctx,
						const char *cmd);
void				handle_not_found_error(t_exec_context *ctx,
						const char *cmd);

void				handle_sigint_interactive(int sig);
void				handle_sigint_exec(int sig);
void				setup_main_signals(void);
void				setup_exec_signals(void);
void				ignore_signals(void);
void				close_all_fds_except(int keep_fd1, int keep_fd2,
						int keep_fd3);

#endif