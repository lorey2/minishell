/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lorey@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:12:19 by lorey             #+#    #+#             */
/*   Updated: 2024/10/30 11:12:25 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*safe_free(char **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
	return (NULL);
}

static char	*handle_rest(char **rest)
{
	char	*line;
	int		i_bksp;
	char	*temp;

	line = NULL;
	i_bksp = ft_strchr(*rest, '\n');
	if (i_bksp != -1)
	{
		line = ft_substr(*rest, 0, i_bksp + 1);
		if (!line)
			return (safe_free(rest));
		temp = ft_strdup(*rest);
		if (!temp)
			return (safe_free(&line), safe_free(rest), NULL);
		free(*rest);
		*rest = ft_substr(temp, i_bksp + 1, ft_strlen(temp) - i_bksp - 1);
		free(temp);
	}
	else if (*rest)
	{
		line = ft_strdup(*rest);
		free(*rest);
		*rest = NULL;
	}
	return (line);
}

static int	tests_and_setup(char **temp, char **buffer, char **rest, int fd)
{
	*temp = NULL;
	*buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!*buffer || fd < 0 || BUFFER_SIZE <= 0)
	{
		free(*buffer);
		return (1);
	}
	if (!(*rest))
	{
		*rest = ft_strdup("");
		if (!(*rest))
		{
			free(*buffer);
			return (1);
		}
	}
	return (0);
}

char	*get_next_line(int fd)
{
	char		*buffer;
	static char	*rest[1024];
	char		*temp;
	int			nbread;

	if (tests_and_setup(&temp, &buffer, &rest[fd], fd))
		return (NULL);
	while (1)
	{
		if (rest[fd] && ft_strchr(rest[fd], '\n') != -1)
			break ;
		nbread = read(fd, buffer, BUFFER_SIZE);
		if (nbread < 0 || (nbread == 0 && *rest[fd] == '\0'))
			return (safe_free(&rest[fd]), safe_free(&buffer), NULL);
		if (nbread == 0)
			break ;
		buffer[nbread] = '\0';
		temp = ft_strjoin(rest[fd], buffer);
		if (!temp)
			return (safe_free(&buffer));
		free(rest[fd]);
		rest[fd] = temp;
	}
	free(buffer);
	return (handle_rest(&rest[fd]));
}
/*
int	main(void)
{
	int		fd;
	char	*line;
	int		i;

	i = 0;
	fd = open("test.txt", O_RDONLY);
	while (i < 6)
	{
		line = get_next_line(fd);
		printf("%s", line);
		free(line);
		i++;
	}
	//line = get_next_line(fd);
	//printf("%s\n", line);
	//free(line);
	close(fd);
	return (0);
}
*/
