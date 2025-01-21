/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 13:13:27 by lorey             #+#    #+#             */
/*   Updated: 2025/01/21 15:34:22 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal;

void	init_new_token(t_parsing_data *token)
{
	token->value = NULL;
	token->pos = -1;
	token->in_file = false;
	token->out_file = false;
	token->is_cmd = false;
	token->arg = NULL;
	token->is_after_pipe = false;
	token->pipe = false;
}

void	init_struct(t_data *data)
{
	g_signal = 0;
	data->exit_nbr = -1;
	data->path = malloc(sizeof(t_path_data));
	if (!data->path)
		error("malloc error", NULL);
	data->env = malloc(sizeof(t_env_data));
	if (!data->env)
		error("malloc error", NULL);
}
