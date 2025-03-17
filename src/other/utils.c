/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:58:57 by lorey             #+#    #+#             */
/*   Updated: 2025/03/17 23:52:24 by lorey            ###   LAUSANNE.ch       */
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

char	*safe_free(char **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
	return (NULL);
}

void	free_double_point(char **point)
{
	int	j;

	j = -1;
	while (point[++j])
		free(point[j]);
	free(point);
	point = NULL;
}
