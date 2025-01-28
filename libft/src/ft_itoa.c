/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 13:30:24 by maambuhl          #+#    #+#             */
/*   Updated: 2024/10/04 13:47:47 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(long n)
{
	int	c;

	c = 1;
	while (n >= 10)
	{
		n = n / 10;
		c++;
	}
	return (c);
}

char	*ft_itoa(int nb)
{
	char	*str;
	int		c;
	int		neg;
	long	n;

	n = (long)nb;
	neg = 0;
	if (n < 0)
	{
		n = n * -1;
		neg = 1;
	}
	c = ft_count(n) + neg;
	str = malloc(sizeof(char) * (c + 1));
	if (!str)
		return (0);
	str[c] = 0;
	while (--c >= 0)
	{
		str[c] = (n % 10) + '0';
		n = n / 10;
	}
	if (neg)
		str[0] = '-';
	return (str);
}
