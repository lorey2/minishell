/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:20:08 by maambuhl          #+#    #+#             */
/*   Updated: 2024/10/02 11:03:39 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	void	*ret;

	ret = dst;
	if (len <= 0 || (!dst && !src))
		return (ret);
	if (dst < src || dst >= (src + len))
		ret = ft_memcpy(dst, src, len);
	else
	{
		while (len--)
		{
			*(unsigned char *)(dst + len) = *(unsigned char *)(src + len);
		}
	}
	return (ret);
}
