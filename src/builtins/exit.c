/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:20:59 by lorey             #+#    #+#             */
/*   Updated: 2025/03/18 00:15:23 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mini_exit(t_data *data, t_parsing_data *p_data)
{
	if (p_data->arg[1])
	{
		if (p_data->arg[2])
			return (write_err("exit: Too many arguments"), 1);
		data->exit_nbr = (ft_atoi(p_data->arg[1]) % 256);
		write(1, "exit\n", 5);
		return (0);
	}
	data->exit_nbr = 0;
	write(1, "exit\n", 5);
	return (0);
}
