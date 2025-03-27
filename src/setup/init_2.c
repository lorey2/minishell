/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:24:26 by lorey             #+#    #+#             */
/*   Updated: 2025/03/27 16:25:09 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
