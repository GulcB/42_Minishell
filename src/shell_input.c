/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:01 by gbodur            #+#    #+#             */
/*   Updated: 2025/08/05 10:15:19 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	configure_readline(void)
{
	rl_catch_signals = 0;
	rl_catch_sigwinch = 0;
}

int	handle_input_validation(char *input)
{
	if (!input)
		return (0);
	if (*input)
		add_history(input);
	if (ft_strlen(input) == 0)
		return (-1);
	return (1);
}

void	safe_cleanup_and_exit(t_gc *main_gc)
{
	if (main_gc)
		gc_destroy(main_gc);
	rl_clear_history();
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	if (isatty(STDOUT_FILENO))
		write(STDOUT_FILENO, "\n", 1);
}

int	process_input_tokens(char *input, t_token **tokens, t_gc *main_gc)
{
	*tokens = lexer_tokenize_with_context(input, main_gc);
	if (!*tokens)
		return (0);
	return (1);
}

int	handle_input_processing(char *input, t_token **tokens, t_gc *main_gc)
{
	int	input_status;

	input_status = handle_input_validation(input);
	if (input_status == -1)
		return (-1);
	if (input_status == 0)
		return (0);
	if (!process_input_tokens(input, tokens, main_gc))
		return (-1);
	return (1);
}
