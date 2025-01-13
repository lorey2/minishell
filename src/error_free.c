/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 21:50:53 by lorey             #+#    #+#             */
/*   Updated: 2025/01/13 21:51:42 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_point(char **point)
{
	int	j;

	j = -1;
	while (point[++j])
		free(point[j]);
	free(point);
}

void	error(char *message, t_path_data *data)
{
	if (data->nbr_malloc >= 2)
		free_double_point(data->path_split);
	if (message == NULL)
		exit(EXIT_SUCCESS);
	perror(message);
	exit(EXIT_FAILURE);
}
