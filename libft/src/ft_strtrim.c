/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 20:10:36 by maambuhl          #+#    #+#             */
/*   Updated: 2024/10/05 11:04:57 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_end(char const *s1, char const *set, int len)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i++] == s1[len])
		{
			len--;
			i = 0;
		}
	}
	return (len);
}

char	*ft_checks1(void)
{
	char	*str;

	str = malloc(sizeof(char));
	if (!str)
		return (0);
	*str = 0;
	return (str);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	size_t	len;
	char	*str;

	i = 0;
	while (set[i])
	{
		if (set[i++] == *s1)
		{
			s1++;
			i = 0;
		}
	}
	if (!*s1)
		return (ft_checks1());
	len = ft_strlen(s1) - 1;
	len = get_end(s1, set, len);
	str = malloc((len + 2) * sizeof(char));
	if (!str)
		return (0);
	ft_strlcpy(str, s1, len + 2);
	return (str);
}
