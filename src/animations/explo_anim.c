/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explo_anim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lorey@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 00:34:56 by lorey             #+#    #+#             */
/*   Updated: 2025/03/27 23:03:00 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define LINE 175

static void	print_algo(char **explo)
{
	int	i;
	int	j;

	i = -1;
	while (i < LINE - 1)
	{
		printf("\033[2J\033[H\033[?25l");
		while (i < LINE - 1 && explo[++i][0] != '\0')
		{
			j = -1;
			while (explo[i][++j])
			{
				if (explo[i][j] == '@')
					printf("\033[38;2;216;86;2m""@");
				else
					printf("\033[38;2;249;242;205m""%c", explo[i][j]);
			}
			printf("\n");
		}
		usleep(200000);
	}
}

void	explosion_animation(void)
{
	int			i;
	int			fd;
	char		**explo;

	printf("\033[2J\033[H\033[?25l");
	fd = open("src/animations/explo", O_RDONLY);
	if (fd < 0)
		error("cannot open file", NULL);
	explo = malloc(sizeof(char *) * (LINE + 1));
	if (!explo)
		error("malloc error", NULL);
	explo[LINE] = NULL;
	i = -1;
	while (++i < LINE)
	{
		explo[i] = get_next_line(fd);
		if (explo[i] && explo[i][strlen(explo[i]) - 1] == '\n')
			explo[i][strlen(explo[i]) - 1] = '\0';
	}
	print_algo(explo);
	free_double_point(&explo);
	close(fd);
	printf(RESET"\033[H\033[J\033[?25h");
}
