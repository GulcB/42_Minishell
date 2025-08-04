/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 01:17:44 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/04 22:39:33 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	*search_in_path_dirs(char **path_dirs, const char *cmd)
{
	char	*full_path;
	int		i;
	int		exec_result;

	if (!path_dirs || !cmd)
		return (NULL);
	i = 0;
	while (path_dirs[i])
	{
		full_path = create_full_path(path_dirs[i], cmd);
		if (!full_path)
		{
			i++;
			continue ;
		}
		exec_result = is_executable_file(full_path);
		if (exec_result == 1)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

static char	*handle_absolute_path(t_exec_context *ctx, const char *cmd)
{
	int	exec_result;

	if (is_directory(cmd))
	{
		handle_directory_error(ctx, cmd);
		return ((char *)-1);
	}
	exec_result = is_executable_file(cmd);
	if (exec_result == 1)
		return (gc_strdup(ctx->gc, cmd));
	else if (exec_result == -1)
	{
		handle_permission_error(ctx, cmd);
		return ((char *)-1);
	}
	handle_not_found_error(ctx, cmd);
	return ((char *)-1);
}

static char	*handle_path_search(t_exec_context *ctx, const char *cmd,
		t_env *env)
{
	char	**path_dirs;
	char	*result;
	char	*gc_result;

	path_dirs = get_path_directories(env);
	if (!path_dirs)
		return (NULL);
	result = search_in_path_dirs(path_dirs, cmd);
	free_path_dirs(path_dirs);
	if (result)
	{
		gc_result = gc_strdup(ctx->gc, result);
		free(result);
		return (gc_result);
	}
	return (NULL);
}

char	*resolve_executable(t_exec_context *ctx, const char *cmd, t_env *env)
{
	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (handle_absolute_path(ctx, cmd));
	return (handle_path_search(ctx, cmd, env));
}
