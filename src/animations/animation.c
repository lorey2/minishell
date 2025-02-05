/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:04:45 by lorey             #+#    #+#             */
/*   Updated: 2025/02/05 21:43:19 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//\033[?25l hide cursor]
//\033[?25h show cursor]

void	explosion_animation(void)
{
	int			i;
	int			j;
	int			fd;
	char		**logo;

	printf("\033[2J\033[H\033[?25l");
	fd = open("src/animations/logo_minishell", O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening file");
		exit(1);
	}
	logo = malloc(sizeof(char *) * 10);
	i = -1;
	while (++i < 10)
	{
		logo[i] = get_next_line(fd);
		if (logo[i] && logo[i][strlen(logo[i]) - 1] == '\n')
			logo[i][strlen(logo[i]) - 1] = '\0';
	}
	i = -1;
	while (++i < 10)
	{
		j = ft_strlen(logo[i]);
		while (--j > 0)
		{
			printf("\033[k\r%s", logo[i] + j);
			usleep(2000);
		}
		printf("\n");
	}
	usleep(1000000);
	close(fd);
	fd = open("src/animations/explo", O_RDONLY);
	printf("%d", fd);
	if (fd < 0)
	{
		perror("Error opening file");
		exit(1);
	}
}
