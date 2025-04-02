/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:08:54 by lorey             #+#    #+#             */
/*   Updated: 2025/04/02 01:56:47 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//like cd, pwd can have two flags -L and -P for links
//maybe we will implement that later but the setup is already here
//apart of that ther command does not care of the nbr of args

static int	setup_flags(t_parsing_data *p_data, t_path_data *path_data)
{
	int	i;

	i = 0;
	init_flags(path_data);
	if (!p_data->arg)
		return (i);
	while (p_data->arg[++i] && p_data->arg[i][0] == '-' && p_data->arg[i][1])
	{
		if (does_contain_only(p_data->arg[i], "LP"))
			fill(p_data->arg[i], path_data);
		else
			return (write_err("pwd: Unknown option only LP\n"), -1);
	}
	if (path_data->is_big_l || path_data->is_big_p)
		return (write_err(" -L, -P are not implemented yet :'(\n"), \
				-1);
	return (i);
}

void	pwd(t_parsing_data *p_data, t_path_data *path_data, t_data *data)
{
	char	shell_prompt[1024];

	if (setup_flags(p_data, path_data) == -1)
	{
		p_data->status = 2;
		if (p_data->pipe)
			frexit(data, 2);
		return ;
	}
	if (getcwd(shell_prompt, sizeof(shell_prompt)) == NULL)
		write_err("getcwd retruned null, idk why\n");
	else
	{
		write(p_data->fd_out, shell_prompt, ft_strlen(shell_prompt));
		write(p_data->fd_out, "\n", 1);
	}
	p_data->status = 0;
	if (p_data->pipe)
		frexit(data, 0);
}
