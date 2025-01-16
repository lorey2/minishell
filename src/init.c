/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 13:13:27 by lorey             #+#    #+#             */
/*   Updated: 2025/01/16 16:54:49 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
