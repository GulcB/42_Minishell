/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 20:38:18 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/07 21:23:31 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/executor.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

static int	call_builtin_func(char **args, t_exec_context *ctx)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args, ctx));
	if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, ctx));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd(args, ctx));
	if (ft_strcmp(args[0], "export") == 0)
		return (builtin_export(args, ctx));
	if (ft_strcmp(args[0], "unset") == 0)
		return (builtin_unset(args, ctx));
	if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env(args, ctx));
	if (ft_strcmp(args[0], "exit") == 0)
		return (builtin_exit(args, ctx));
	return (1);
}

int	exec_builtin(char **args, t_exec_context *ctx)
{
	int	result;

	if (!args || !args[0] || !ctx)
		return (1);
	result = call_builtin_func(args, ctx);
	ctx->exit_status = result;
	return (result);
}




// GPT'NIN VERDİĞİ SİNYAL FONKSİYONUNDAN HİÇBİR ŞEY ANLAMADIM ÇÖZÜMLEMEK İÇİN BURDA KALSIN

// void	handle_child_signals(int status, t_exec_context *ctx)
// {
// 	int	signal_num;
// 	int	exit_code;

// 	if ((status & 0x7F) != 0)
// 	{
// 		signal_num = status & 0x7F;
// 		if (signal_num == SIGINT)
// 		{
// 			ctx->exit_status = 130;
// 			if (isatty(STDIN_FILENO))
// 				write(STDERR_FILENO, "\n", 1);
// 		}
// 		else if (signal_num == SIGQUIT)
// 		{
// 			ctx->exit_status = 131;
// 			if (isatty(STDIN_FILENO))
// 				write(STDERR_FILENO, "Quit (core dumped)\n", 19);
// 		}
// 		else
// 			ctx->exit_status = 128 + signal_num;
// 	}
// 	else
// 	{
// 		exit_code = (status & 0xFF00) >> 8;
// 		ctx->exit_status = exit_code;
// 	}
// }
