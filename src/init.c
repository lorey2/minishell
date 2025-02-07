/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 13:13:27 by lorey             #+#    #+#             */
/*   Updated: 2025/02/07 13:53:53 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal;

void	init_new_token(t_parsing_data *token)
{
	token->delimiter = NULL;
	token->fd_out = STDOUT_FILENO;
	token->fd_in = STDIN_FILENO;
	token->value = NULL;
	token->pos = -1;
	token->infile = NULL;
	token->outfile = NULL;
	token->here = NULL;
	token->in_file = false;
	token->out_file = false;
	token->is_cmd = false;
	token->arg = NULL;
	token->is_after_pipe = false;
	token->pipe = false;
	token->append_file = false;
}

void	init_struct(t_data *data)
{
	g_signal = 0;
	data->exit_nbr = -1;
	data->return_nbr = 0;
	data->path = malloc(sizeof(t_path_data));
	if (!data->path)
		error("malloc error", NULL);
	data->env = malloc(sizeof(t_env_data));
	if (!data->env)
		error("malloc error", NULL);
}
