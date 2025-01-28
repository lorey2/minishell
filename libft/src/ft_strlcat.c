/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:45:58 by maambuhl          #+#    #+#             */
/*   Updated: 2024/10/03 11:33:51 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	j;
	size_t	i;
	size_t	src_len;

	src_len = ft_strlen(src);
	i = 0;
	while (dst[i] && i < size)
		i++;
	j = 0;
	while (src[j] && size && i < (size - 1))
		dst[i++] = src[j++];
	if (j)
		dst[i] = 0;
	return (src_len + i - j);
}
