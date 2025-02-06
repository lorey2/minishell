/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lorey@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 02:56:13 by lorey             #+#    #+#             */
/*   Updated: 2025/02/06 03:11:42 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (ptr == NULL)
	{
		printf("malloc error");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

char	*resize_buff(char *buff, int *size)
{
	char	*new_buff;
	int		new_size;
	int		i;

	new_size = *size * 2;
	new_buff = malloc(new_size);
	if (!new_buff)
		return (NULL);
	i = -1;
	while (++i < *size)
		new_buff[i] = buff[i];
	free(buff);
	*size = new_size;
	return (new_buff);
}

char	*gnl(int fd)
{
	char	*buff;
	char	c;
	int		r;
	int		size;
	int		i;

	i = 0;
	size = 10;
	buff = malloc(size);
	if (!buff)
		return (NULL);
	r = read(fd, &c, 1);
	while (r > 0)
	{
		if (i + 1 >= size)
		{
			buff = resize_buff(buff, &size);
			if (!buff)
				return (NULL);
		}
		buff[i++] = c;
		if (c == '\n')
			break ;
		r = read(fd, &c, 1);
	}
	if (r <= 0 && i == 0)
		return (NULL);
	buff[i] = '\0';
	return (buff);
}
