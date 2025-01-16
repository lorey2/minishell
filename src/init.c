/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 13:13:27 by lorey             #+#    #+#             */
/*   Updated: 2025/01/16 18:18:40 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_new_token(t_parsing_data token)
{
	token.value = NULL;
	token.nbr = -1;
	token.is_open_file = false;
	token.is_close_file = false;
	token.is_command = false;
	token.is_arg = false;
	token.is_after_pipe = false;
}

void	init_struct(t_data *data)
{
	data->path = malloc(sizeof(t_path_data));
	if (!data->path)
		error("malloc error", NULL);
	data->env = malloc(sizeof(t_env_data));
	if (!data->env)
		error("malloc error", NULL);
}
