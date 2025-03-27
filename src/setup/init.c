/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 13:13:27 by lorey             #+#    #+#             */
/*   Updated: 2025/03/27 16:25:20 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal[2];

void	init_new_token(t_parsing_data *token)
{
	token->fd_in = STDIN_FILENO;
	token->fd_out = STDOUT_FILENO;
	token->value = NULL;
	token->delimiter = NULL;
	token->arg = NULL;
	token->here = NULL;
	token->infile = NULL;
	token->outfile = NULL;
	token->pos = -1;
	token->saved_stdin = -1;
	token->status = -1;
	token->in_file = false;
	token->out_file = false;
	token->outfile_list = NULL;
	token->append_file = false;
	token->is_cmd = false;
	token->is_after_pipe = false;
	token->pipe = false;
	token->outfile_list = NULL;
	token->here_docs = NULL;
	token->next = NULL;
	token->previous = NULL;
	token->pid = 0;
}

void	init_struct(t_data *data)
{
	g_signal[0] = 0;
	g_signal[1] = 0;
	data->var = NULL;
	data->token = NULL;
	data->exit_nbr = -1;
	data->return_nbr = 0;
	data->last_exit = 0;
	data->path = malloc(sizeof(t_path_data));
	if (!data->path)
		error("malloc error", NULL);
	init_path(data->path);
	data->pp_data = malloc(sizeof(t_pre_pars_data));
	if (!data->pp_data)
		error("malloc error", NULL);
	data->env = malloc(sizeof(t_env_data));
	if (!data->env)
		error("malloc error", NULL);
}
