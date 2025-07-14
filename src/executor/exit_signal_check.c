/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_signal_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 00:46:17 by gbodur            #+#    #+#             */
/*   Updated: 2025/07/15 01:53:27 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/executor.h"

int	extract_exit_code(int status)
{
	return ((status >> 8) & 0xFF);
}

int	extract_signal_number(int status)
{
	return (status & 0x7F);
}

int	is_normal_exit(int status)
{
	return ((status & 0x7F) == 0);
}

int	is_signaled_exit(int status)
{
	return ((status & 0x7F) != 0 && ((status & 0x7F) != 0x7F));
}
