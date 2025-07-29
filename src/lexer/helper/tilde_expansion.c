/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:00:00 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/29 15:08:10 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "lexer.h"

static char	*get_home_directory(struct s_exec_context *ctx)
{
	char	*home;

	if (!ctx)
		return (NULL);
	home = search_env_var("HOME", ctx);
	if (!home)
	{
		home = getenv("HOME");
		if (home)
			return (ft_strdup(home));
		return (NULL);
	}
	return (home);
}

static char	*expand_tilde_at_start(const char *str, struct s_exec_context *ctx)
{
	char	*home;
	char	*result;

	if (!str || str[0] != '~')
		return (ft_strdup(str));
	if (str[1] == '\0' || str[1] == '/')
	{
		home = get_home_directory(ctx);
		if (!home)
			return (ft_strdup(str));
		if (str[1] == '\0')
			return (home);
		result = ft_strjoin(home, &str[1]);
		free(home);
		return (result);
	}
	return (ft_strdup(str));
}

char	*expand_tilde(const char *str, struct s_exec_context *ctx)
{
	if (!str)
		return (NULL);
	if (str[0] == '~')
		return (expand_tilde_at_start(str, ctx));
	return (ft_strdup(str));
}
