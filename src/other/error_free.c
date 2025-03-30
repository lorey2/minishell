/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 21:50:53 by lorey             #+#    #+#             */
/*   Updated: 2025/03/30 16:23:10 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error(char *message, t_data *data)
{
	if (data)
		free_everything(data);
	perror(message);
	exit(EXIT_FAILURE);
}

void	free_var(t_var *var)
{
	t_var	*var_cpy;

	var_cpy = var;
	while (var)
	{
		var_cpy = var->next;
		safe_free((void **)&var->var_name);
		safe_free((void **)&var->var_value);
		var = var_cpy;
	}
}

void	free_out_files(t_parsing_data *token)
{
	t_file	*file;
	t_file	*tmp;

	file = token->outfile_list;
	while (file)
	{
		safe_free((void **)&file->name);
		tmp = file;
		file = file->next;
		safe_free((void **)&tmp);
	}
}

void	free_in_files(t_parsing_data *token)
{
	t_file	*file;
	t_file	*tmp;

	file = token->infile_list;
	while (file)
	{
		safe_free((void **)&file->name);
		tmp = file;
		file = file->next;
		safe_free((void **)&tmp);
	}
}

void	free_tokens(t_parsing_data *token, t_data *data)
{
	t_parsing_data	*token_cpy;

	if (token)
	{
		token_cpy = token;
		while (token)
		{
			token_cpy = token->next;
			free_out_files(token);
			free_in_files(token);
			safe_free((void **)&token->value);
			safe_free((void **)&token->here);
			free_double_point(&token->arg);
			safe_free((void **)&token);
			token = token_cpy;
		}
	}
	data->token = NULL;
}

void	free_everything(t_data *data)
{
	free_var(data->var);
	free_path(data->path);
	safe_free((void **)&data->input);
	free_double_point(&data->env->env);
	safe_free((void **)&data->env);
	safe_free((void **)&data->path);
	safe_free((void **)&data->pp_data);
	free_tokens(data->token, data);
	safe_free((void **)&data->var);
	safe_free((void **)&data->token);
	clear_history();
}
