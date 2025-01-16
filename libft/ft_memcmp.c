/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:17:16 by maambuhl          #+#    #+#             */
/*   Updated: 2024/10/03 16:21:04 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*bu1;
	unsigned char	*bu2;

	bu1 = (unsigned char *)s1;
	bu2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (bu1[i] != bu2[i])
			return (bu1[i] - bu2[i]);
		i++;
	}
	return (0);
}
