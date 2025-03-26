/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 18:02:25 by lorey             #+#    #+#             */
/*   Updated: 2025/03/26 17:49:36 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	does_contain_only(char *data, char *list_args)
{
	int		i;
	int		j;
	bool	check;

	i = 0;
	while (data[++i])
	{
		j = -1;
		check = false;
		while (list_args[++j])
		{
			if (data[i] == list_args[j])
				check = true;
		}
		if (check == false)
			return (false);
	}
	return (true);
}

void	fill(char *data, t_path_data *path_data)
{
	int	i;

	i = 0;
	while (data[++i])
	{
		if (data[i] == 'L')
			path_data->is_big_l = true;
		else if (data[i] == 'P')
			path_data->is_big_p = true;
		else if (data[i] == 'E')
			path_data->is_big_e = true;
		else if (data[i] == 'e')
			path_data->is_e = true;
		else if (data[i] == '@')
			path_data->is_at = true;
		else if (data[i] == 'n')
			path_data->is_n = true;
		else if (data[i] == 'p')
			path_data->is_p = true;
		else if (data[i] == 'f')
			path_data->is_f = true;
		else if (data[i] == 'v')
			path_data->is_v = true;
	}
}

void	init_flags(t_path_data *path_data)
{
	path_data->is_big_l = false;
	path_data->is_big_p = false;
	path_data->is_big_e = false;
	path_data->is_at = false;
	path_data->is_p = false;
	path_data->is_e = false;
	path_data->is_n = false;
	path_data->is_v = false;
	path_data->is_f = false;
}

void	write_err(char *message)
{
	write(1, message, ft_strlen(message));
}

void	frexit(t_data *data, int exit_number)
{
	free_everything(data);
	exit(exit_number);
}
