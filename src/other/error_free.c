/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 21:50:53 by lorey             #+#    #+#             */
/*   Updated: 2025/03/24 01:30:39 by lorey            ###   LAUSANNE.ch       */
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
		if (token->value)
			safe_free((void **)&token->value);
		if (token->here)
			safe_free((void **)&token->here);
		if (token->delimiter)
			safe_free((void **)&token->delimiter);
		if (token->arg)
			free_double_point(token->arg);
		if (token)
			safe_free((void **)&token);
		token = token_cpy;
	}
	token = NULL;
}

void	free_everything(t_data *data)
{
	safe_free((void **)&data->input);
	safe_free((void **)&data->env);
//	free_double_point(data->path->path_split);
	if (data->path)
		free_path(data->path);
	safe_free((void **)&data->path);
	safe_free((void **)&data->pp_data);
//	if (data->token)
//		free_tokens(data->token);
	clear_history();
}
