/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_table.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 18:04:31 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/24 18:08:40 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

static void	print_header(void)
{
	ft_putstr_fd("TYPE", STDOUT_FILENO);
	ft_putstr_fd("\t\t", STDOUT_FILENO);
	ft_putstr_fd("VALUE", STDOUT_FILENO);
	ft_putstr_fd("\t\t", STDOUT_FILENO);
	ft_putstr_fd("POSITION", STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

static void	print_token_info(t_token *token)
{
	char	*display_value;

	ft_putstr_fd(token_type_to_str(token->type), STDOUT_FILENO);
	ft_putstr_fd("\t\t", STDOUT_FILENO);
	if (token->value)
		display_value = token->value;
	else
		display_value = "NULL";
	ft_putstr_fd(display_value, STDOUT_FILENO);
	ft_putstr_fd("\t\t", STDOUT_FILENO);
	ft_putnbr_fd(token->position, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

static void	print_token_count(int count)
{
	ft_putstr_fd("Toplam ", STDOUT_FILENO);
	ft_putnbr_fd(count, STDOUT_FILENO);
	ft_putstr_fd(" token bulundu.\n", STDOUT_FILENO);
}

void	token_list_print(t_token *head)
{
	t_token	*current;
	int		token_count;

	token_count = 0;
	current = head;
	print_header();
	while (current)
	{
		token_count++;
		print_token_info(current);
		current = current->next;
	}
	print_token_count(token_count);
}
