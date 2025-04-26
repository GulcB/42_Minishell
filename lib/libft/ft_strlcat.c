/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 12:44:10 by gbodur            #+#    #+#             */
/*   Updated: 2024/10/19 20:41:12 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	g;
	size_t	d_len;

	i = 0;
	g = 0;
	d_len = 0;
	while (*dst)
	{
		dst++;
		d_len++;
	}
	while (src[i])
		i++;
	if (dstsize == 0 || dstsize <= d_len)
		return (i + dstsize);
	while (src[g] && (g < dstsize - d_len - 1))
	{
		dst[g] = src[g];
		g++;
	}
	dst[g] = '\0';
	return (d_len + i);
}
