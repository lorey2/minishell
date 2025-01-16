/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 11:43:48 by maambuhl          #+#    #+#             */
/*   Updated: 2024/10/05 11:28:20 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(char const *s, char c)
{
	int		i;
	size_t	count;

	i = 0;
	count = 1;
	if (!*s)
		return (0);
	while (s[i] && s[i] == c)
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (s[i] == c)
		{
			while (s[i] && s[i] == c)
				i++;
			if (s[i])
				count++;
		}
		else
			i++;
	}
	return (count);
}

static char	*ft_dup(int start, char const *s, char c)
{
	int		i;
	char	*str;

	i = 0;
	while (s[start + i] != c && s[start + i])
		i++;
	str = malloc(sizeof(char) * (i + 1));
	if (!str)
		return (0);
	i = 0;
	while (s[start + i] != c && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = 0;
	return (str);
}

static int	ft_word(char **tab, char const *s, char c, int count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < count)
	{
		while (s[j] == c)
			j++;
		tab[i] = ft_dup(j, s, c);
		if (!tab[i])
			return (0);
		j += ft_strlen(tab[i]);
		i++;
	}
	tab[i] = 0;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		count;
	int		x;

	count = ft_count(s, c);
	if (!count)
	{
		tab = malloc(sizeof(char *));
		if (!tab)
			return (0);
		tab[0] = 0;
		return (tab);
	}
	tab = malloc(sizeof(char *) * (count + 1));
	if (!tab)
		return (0);
	x = ft_word(tab, s, c, count);
	if (!x)
		return (0);
	return (tab);
}
