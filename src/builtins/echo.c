/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 17:55:52 by lorey             #+#    #+#             */
/*   Updated: 2025/03/29 15:08:24 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//here we will have to change the code to correspond to that
//$ bash
//lorey@c3r6s4:~$ echo \n
//n
//lorey@c3r6s4:~$ echo -e \n
//n
//lorey@c3r6s4:~$ echo -E \n
//n
//lorey@c3r6s4:~$ echo -e '\n'
//
//
//lorey@c3r6s4:~$ echo -E '\n'
//\n
//WARNING WARNINIG WARNINIG WARNINIG
//now it works only without quote.
//It skips backslash wich is correct with and without -e
//is_e is useless because it works only with  '' and/or ""
//the only function to modify is echo_write if we handle the '' and "" elsewhere

void	echo_write(t_parsing_data *p_data, int i)
{
	int	j;

	j = -1;
	while (p_data->arg[i][++j])
	{
		if (p_data->arg[i][j] == '\\')
			j++;
		write(p_data->fd_out, &p_data->arg[i][j], 1);
	}
}

static int	setup_flags(t_parsing_data *p_data, t_path_data *path_data)
{
	int	i;

	i = 0;
	init_flags(path_data);
	while (p_data->arg[++i] && p_data->arg[i][1] && p_data->arg[i][0] == '-' && \
		does_contain_only(p_data->arg[i], "eEn"))
		fill(p_data->arg[i], path_data);
	if (path_data->is_e || path_data->is_big_e)
		return (write_err("e and E are not implemented yet :'(\n"), -1);
	return (i);
}

void	do_echo(t_parsing_data *p_data, t_path_data *path_data, int i)
{
	if (p_data->arg[i])
		echo_write(p_data, i);
	while (p_data->arg[++i])
	{
		write(p_data->fd_out, " ", 1);
		echo_write(p_data, i);
	}
	if (path_data->is_n == false)
		write(p_data->fd_out, "\n", 1);
}

void	echo(t_parsing_data *p_data, t_path_data *path_data, t_data *data)
{
	int	i;

//	printf("p_data[0] : %s\n", p_data->arg[0]);
//	printf("p_data[1] : %s\n", p_data->arg[1]);
//	printf("p_data[2] : %s\n", p_data->arg[2]);
	p_data->status = 0;
	i = setup_flags(p_data, path_data);
	if (p_data->arg[i] == NULL)
	{
		if (path_data->is_n == false)
			write(p_data->fd_out, "\n", 1);
		if (p_data->pipe)
			frexit(data, 0);
		return ;
	}
	if (i == -1)
	{
		p_data->status = 2;
		if (p_data->pipe)
			frexit(data, 2);
		return ;
	}
	do_echo(p_data, path_data, i);
	if (p_data->pipe)
		frexit(data, 0);
}
