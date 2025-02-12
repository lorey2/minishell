/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 21:50:53 by lorey             #+#    #+#             */
/*   Updated: 2025/02/12 18:07:26 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error(char *message, t_data *data)
{
	(void)data;
	perror(message);
	exit(EXIT_FAILURE);
}

void	free_everything(t_data *data)
{
	free(data->env);
	free_double_point(data->path->path_split);
	free(data->path);
	clear_history();
}
