/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 01:17:44 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/31 14:06:18 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	is_executable_file(const char *path)
{
	if (!path)
		return (0);
	return (access(path, F_OK) == 0 && access(path, X_OK) == 0);
}

static char	*create_full_path(const char *dir, const char *cmd)
{
	char	*temp;
	char	*full_path;

	if (!dir || !cmd)
		return (NULL);
	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

static char	*search_in_path_dirs(char **path_dirs, const char *cmd)
{
	char	*full_path;
	int		i;

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
		if (is_executable_file(full_path))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

static char	**get_path_directories(t_env *env)
{
	char	*path_env;
	char	**path_dirs;

	path_env = env_get(env, "PATH");
	if (!path_env)
		return (NULL);
	path_dirs = ft_split(path_env, ':');
	return (path_dirs);
}

static void	free_path_dirs(char **path_dirs)
{
	int	i;

	if (!path_dirs)
		return ;
	i = 0;
	while (path_dirs[i])
	{
		free(path_dirs[i]);
		i++;
	}
	free(path_dirs);
}

char	*resolve_executable(t_exec_context *ctx, const char *cmd, t_env *env)
{
	char	**path_dirs;
	char	*result;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (is_executable_file(cmd))
			return (gc_strdup(ctx->gc, cmd));
		return (NULL);
	}
	path_dirs = get_path_directories(env);
	if (!path_dirs)
		return (NULL);
	result = search_in_path_dirs(path_dirs, cmd);
	free_path_dirs(path_dirs);
	return (result);
}
