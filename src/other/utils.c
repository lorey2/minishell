/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:58:57 by lorey             #+#    #+#             */
/*   Updated: 2025/03/24 15:57:30 by lorey            ###   LAUSANNE.ch       */
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

void	safe_free(void **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	free_double_point(char ***point)
{
	int	j;

	j = -1;
	if (*point)
	{
		while ((*point)[++j])
			safe_free((void **)&(*point)[j]);
		safe_free((void **)*point);
		*point = NULL;
	}
}
