/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:12:04 by gbodur            #+#    #+#             */
/*   Updated: 2024/10/19 20:48:53 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*p;
	size_t	len;
	int		i;

	i = 0;
	len = ft_strlen(s1) + 1;
	p = (char *)malloc(sizeof(char) * len);
	if (!p)
		return (NULL);
	while (len)
	{
		p[i] = s1[i];
		i++;
		len--;
	}
	return (p);
}
