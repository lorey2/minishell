/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:27:54 by maambuhl          #+#    #+#             */
/*   Updated: 2024/10/02 17:51:43 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen((src));
	if (!src || !dst)
		return (0);
	if (dstsize <= 0)
		return (len);
	while (i < (dstsize - 1) && i < len)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
	return (len);
}
