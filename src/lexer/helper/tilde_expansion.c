/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:00:00 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/31 15:04:23 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "lexer.h"

static char	*get_home_directory(t_exec_context *ctx)
{
	char	*home;

	if (!ctx)
		return (NULL);
	home = search_env_var("HOME", ctx);
	if (!home)
	{
		home = getenv("HOME");
		if (home)
			return (gc_strdup(ctx->gc, home));
		return (NULL);
	}
	return (home);
}

static char	*expand_tilde_at_start(const char *str, t_exec_context *ctx)
{
	char	*home;
	char	*result;

	if (!str || str[0] != '~')
		return (gc_strdup(ctx->gc, str));
	if (str[1] == '\0' || str[1] == '/')
	{
		home = get_home_directory(ctx);
		if (!home)
			return (gc_strdup(ctx->gc, str));
		if (str[1] == '\0')
			return (home);
		result = gc_strjoin(ctx->gc, home, &str[1]);
		return (result);
	}
	return (gc_strdup(ctx->gc, str));
}

char	*expand_tilde(const char *str, t_exec_context *ctx)
{
	if (!str)
		return (NULL);
	if (str[0] == '~')
		return (expand_tilde_at_start(str, ctx));
	return (gc_strdup(ctx->gc, str));
}
