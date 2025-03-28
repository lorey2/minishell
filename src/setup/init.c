/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 13:13:27 by lorey             #+#    #+#             */
/*   Updated: 2025/03/28 16:01:27 by maambuhl         ###   LAUSANNE.ch       */
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
	token->infile_list = NULL;
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
	data->path = safe_malloc(sizeof(t_path_data), data);
	init_path(data->path);
	data->pp_data = safe_malloc(sizeof(t_pre_pars_data), data);
	data->env = safe_malloc(sizeof(t_env_data), data);
}
