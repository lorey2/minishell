/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 21:50:53 by lorey             #+#    #+#             */
/*   Updated: 2025/03/25 16:49:43 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error(char *message, t_data *data)
{
	(void)data;
	perror(message);
	exit(EXIT_FAILURE);
}

void	free_tokens(t_parsing_data *token)
{
	t_parsing_data	*token_cpy;

	token_cpy = token;
	while (token)
	{
		token_cpy = token->next;
		safe_free((void **)&token->value);
		safe_free((void **)&token->here);
		safe_free((void **)&token->delimiter);
		free_double_point(&token->arg);
		
		token = token_cpy;
	}
	token = NULL;
}

void	free_everything(t_data *data)
{
	free_path(data->path);
	safe_free((void **)&data->input);
	free_double_point(&data->env->env);
	safe_free((void **)&data->env);
///	free_double_point(&data->path->path_split);
	safe_free((void **)&data->path);
	safe_free((void **)&data->pp_data);
	free_tokens(data->token);
	safe_free((void **)&data->token);
	clear_history();
}
