/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 13:13:27 by lorey             #+#    #+#             */
/*   Updated: 2025/03/24 16:56:19 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdbool.h>

volatile sig_atomic_t	g_signal;

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

void	init_new_var(t_var *var)
{
	var->var_name = NULL;
	var->var_value = NULL;
	var->next = NULL;
}

void	init_path(t_path_data *path_data)
{
	path_data->env_path = NULL;
	path_data->path_split = NULL;
	path_data->path_split_slash = NULL;
	path_data->path_with_com = NULL;
	path_data->is_at = false;
	path_data->is_e = false;
	path_data->is_n = false;
	path_data->is_v = false;
	path_data->is_f = false;
	path_data->is_p = false;
	path_data->is_big_l = false;
	path_data->is_big_p = false;
	path_data->is_big_e = false;
}

void	init_new_file(t_file *file)
{
	file->name = NULL;
	file->append = false;
	file->next = NULL;
}

void	init_new_here(t_here_docs *here)
{
	here->delimiter = NULL;
	here->next = NULL;
}

void	init_struct(t_data *data)
{
	g_signal = 0;
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
