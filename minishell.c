/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lorey@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 01:52:52 by lorey             #+#    #+#             */
/*   Updated: 2024/12/30 03:22:21 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <string.h>

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

int	get_next_line(char **line)
{
	char	*buffer;
	int		i;
	int		bytes_read;

	i = 0;
	buffer = (char *)malloc(1024);
	bytes_read = read(0, &buffer[i], 1);
	if (!buffer || bytes_read < 1)
		return (bytes_read);
	while (bytes_read > 0 && buffer[i] != '\n')
		bytes_read = read(0, &buffer[++i], 1);
	if (buffer[i] == '\n')
		i++;
	buffer[i] = '\0';
	*line = buffer;
	if (bytes_read == 0 && i == 0)
	{
		free(buffer);
		*line = NULL;
		return (0);
	}
	return (1);
}

int	main(void)
{
	char		*line;
	const char	*path;
	char		**path_split;

	path = getenv("PATH");
	path_split = ft_split(path, ':');
	write(1, path, strlen(path));
	while (1)
	{
		write(1, "42@minishell>", 13);
		get_next_line(&line);
	}
}
