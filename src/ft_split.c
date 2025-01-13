/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 21:39:33 by lorey             #+#    #+#             */
/*   Updated: 2025/01/13 21:39:52 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_wordlen(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static int	ft_wordcount(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s == c)
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		s++;
	}
	return (count);
}

static void	assignation_variable(int *i, int *j)
{
	*i = 0;
	*j = 0;
}

static void	*free_array(char **array, int j)
{
	int	i;

	i = 0;
	while (i < j)
		free(array[i++]);
	free(array);
	return (NULL);
}

char	**ft_split(const char *s, char c)
{
	char	**tab;
	int		i;
	int		j;
	int		k;

	if (!s)
		return (NULL);
	assignation_variable(&i, &j);
	tab = malloc(sizeof(char *) * (ft_wordcount(s, c) + 1));
	if (!tab)
		return (NULL);
	while (j < ft_wordcount(s, c))
	{
		while (s[i] == c)
			i++;
		tab[j] = malloc(ft_wordlen(&s[i], c) + 1);
		if (!tab[j])
			return (free_array(tab, j));
		k = 0;
		while (s[i] && s[i] != c)
			tab[j][k++] = s[i++];
		tab[j++][k] = '\0';
	}
	tab[j] = 0;
	return (tab);
}
