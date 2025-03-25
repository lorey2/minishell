/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:20:59 by lorey             #+#    #+#             */
/*   Updated: 2025/03/24 16:28:35 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mini_exit(t_data *data, t_parsing_data *p_data)
{
	if (p_data->arg[1])
	{
		if (p_data->arg[2] && p_data->pipe)
		{
			write_err("exit: Too many arguments\n");
			exit(127);
		}
		if (p_data->arg[2])
		{
			p_data->status = 127;
			return (write_err("exit: Too many arguments\n"), 1);
		}
		data->exit_nbr = (ft_atoi(p_data->arg[1]) % 256);
		if (p_data->pipe)
			exit(data->exit_nbr);
		return (0);
	}
	data->exit_nbr = 0;
	if (p_data->pipe)
		exit(data->exit_nbr);
	return (0);
}
