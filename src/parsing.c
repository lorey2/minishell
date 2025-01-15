/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:23:58 by lorey             #+#    #+#             */
/*   Updated: 2025/01/15 16:54:14 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO error gestion

void	parsing(char *input, t_data *data)
{
	t_parsing_data	*pars;
	int				i;

	pars = malloc(sizeof(t_parsing_data));
	if (!pars)
		error("malloc error", data);
	i = 0;
	while (input[i])
	{
		if (input[i] == '<')
	}
}






//     ls    -la    >    output.txt |   cat   -e 
