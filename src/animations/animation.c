/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:04:45 by lorey             #+#    #+#             */
/*   Updated: 2025/02/06 02:47:44 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//\033[?25l hide cursor]
//\033[?25h show cursor]

static void	print_algo(char **logo)
{
	int	i;
	int	j;

	i = -1;
	while (logo[++i])
	{
		j = ft_strlen(logo[i]);
		while (--j > 0)
		{
			printf("\033[k\r%s", logo[i] + j);
			usleep(2000);
		}
		printf("\n");
	}
}

void	text_animation(void)
{
	int			i;
	int			fd;
	char		**logo;

	printf(BRIGHT_GREEN"\033[2J\033[H\033[?25l");
	fd = open("src/animations/logo_minishell", O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening file");
		exit(1);
	}
	logo = malloc(sizeof(char *) * 11);
	logo[10] = NULL;
	i = -1;
	while (++i < 10)
	{
		logo[i] = gnl(fd);
		if (logo[i] && logo[i][strlen(logo[i]) - 1] == '\n')
			logo[i][strlen(logo[i]) - 1] = '\0';
	}
	print_algo(logo);
	usleep(1000000);
	free_double_point(logo);
	close(fd);
	printf(RESET"\033[H\033[J\033[?25h");
}
