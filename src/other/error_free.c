/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 21:50:53 by lorey             #+#    #+#             */
/*   Updated: 2025/01/15 18:27:29 by lorey            ###   LAUSANNE.ch       */
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

void	error(char *message, t_data *data)
{
	(void)data;
	perror(message);
	exit(EXIT_FAILURE);
}
