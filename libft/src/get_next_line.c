/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 15:54:37 by maambuhl          #+#    #+#             */
/*   Updated: 2024/11/07 17:46:08 by doublegras       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_slineret(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_freer(void *ptr1, void *ptr2)
{
	free(ptr1);
	free(ptr2);
	return (0);
}

char	*read_file(int fd, char *conca)
{
	char	*buff;
	ssize_t	r;

	buff = ft_calloc_char(BUFFER_SIZE + 1);
	if (!buff)
		return (ft_freer(conca, 0));
	if (fd < 0 || read(fd, buff, 0) < 0)
		return (ft_freer(conca, buff));
	if (!conca)
		conca = ft_calloc_char(1);
	r = 1;
	while (r)
	{
		r = read(fd, buff, BUFFER_SIZE);
		if (r == -1)
			return (ft_freer(conca, buff));
		buff[r] = 0;
		conca = ft_conca(conca, buff);
		if (ft_slineret(conca))
			r = 0;
	}
	free(buff);
	return (conca);
}

char	*get_next_line(int fd)
{
	static char	*conca;
	char		*line;
	char		*rest;

	conca = read_file(fd, conca);
	if (!conca)
		return (0);
	line = ft_getline(conca);
	if (!line)
		return (ft_freer(conca, 0));
	rest = ft_getrest(conca);
	free(conca);
	conca = rest;
	if (!*line)
		return (ft_freer(rest, line));
	return (line);
}

// int	main(void)
// {
// 	int		fd;
// 	char	*str;
//
// 	fd = open("read_error.txt", O_RDONLY);
//
// 	// str = get_next_line(fd);
// 	// printf("%s", str);
// 	// free(str);
// 	//
// 	// str = get_next_line(fd);
// 	// printf("%s", str);
// 	// free(str);
// 	//
// 	// str = get_next_line(fd);
// 	// printf("%s", str);
// 	// free(str);
// 	//
// 	// str = get_next_line(fd);
// 	// printf("%s", str);
// 	// free(str);
// 	//
// 	// str = get_next_line(fd);
// 	// printf("%s", str);
// 	// free(str);
// 	//
// 	// str = get_next_line(fd);
// 	// printf("%s", str);
// 	// free(str);
// 	printf("%s", get_next_line(fd));
// 	printf("%s", get_next_line(fd));
// 	printf("%s", get_next_line(fd));
// 	printf("%s", get_next_line(fd));
// 	printf("%s", get_next_line(fd));
// 	printf("%s", get_next_line(fd));
// 	printf("%s", get_next_line(fd));
// 	printf("%s", get_next_line(fd));
// 	printf("%s", get_next_line(fd));
// 	printf("%s", get_next_line(fd));
// 	printf("%s", get_next_line(fd));
// 	return (0);
// }
