/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:58:57 by lorey             #+#    #+#             */
/*   Updated: 2025/03/31 13:48:29 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*safe_malloc(size_t size, t_data *data)
{
	void	*ptr;

	ptr = malloc(size);
	if (ptr == NULL)
	{
		free_everything(data);
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
		safe_free((void **)point);
	}
}

void	error(char *message, t_data *data)
{
	if (data)
		free_everything(data);
	perror(message);
	exit(EXIT_FAILURE);
}
