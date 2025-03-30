/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:20:59 by lorey             #+#    #+#             */
/*   Updated: 2025/03/29 18:54:15 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_arg_number(t_parsing_data *p_data)
{
	int		i;

	i = -1;
	while (p_data->arg[1][++i])
		if (p_data->arg[1][i] > '0' && p_data->arg[1][i] < '9')
			return (false);
	return (true);
}

void	too_many_arg(t_parsing_data *p_data, t_data *data)
{
	if (p_data->arg[2] && p_data->pipe)
	{
		write_err("exit: Too many arguments\n");
		frexit(data, 127);
	}
}

int	mini_exit(t_data *data, t_parsing_data *p_data)
{
	if (p_data->arg[1])
	{
		too_many_arg(p_data, data);
		if (p_data->arg[2])
		{
			p_data->status = 127;
			return (write_err("exit: Too many arguments\n"), 1);
		}
		if (!is_arg_number(p_data)
			|| ft_strlen(p_data->arg[1]) > 9223372036854775807)
			data->exit_nbr = (ft_atoi(p_data->arg[1]) % 256);
		else
		{
			write(1, "numeric argument required\n", 26);
			data->exit_nbr = 2;
		}
		if (p_data->pipe)
			frexit(data, data->exit_nbr);
		return (0);
	}
	data->exit_nbr = 0;
	if (p_data->pipe)
		frexit(data, data->exit_nbr);
	return (0);
}
