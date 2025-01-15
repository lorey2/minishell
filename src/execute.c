/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 22:00:38 by lorey             #+#    #+#             */
/*   Updated: 2025/01/15 16:58:29 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/* ex: ls -la execve(/bin/ls, {ls, -la}). We check all path here          */
/* no error possible                                                          */
/* list of malloc at this point : path_data->path->split and a lot more later */
/* added malloc : nothing                                                     */
/* ************************************************************************** */

void	execute(char *command, t_path_data *data)
{
	int		i;

	i = -1;
	while (data->path_split[++i])
	{
		data->path_with_com = ft_strjoin(data->path_split[i], command);
		if (!data->path_with_com)
			error("malloc error", NULL);
		data->path_with_com_split = ft_split(data->path_with_com, ' ');
		if (!data->path_with_com_split)
			error("malloc error", NULL);
		data->cmd_split = ft_split(command, ' ');
		if (!data->cmd_split)
			error("malloc error", NULL);
		if (execve(data->path_with_com_split[0], data->cmd_split, NULL) != -1)
			break ;
		free_double_point(data->path_with_com_split);
		free_double_point(data->cmd_split);
		free(data->path_with_com);
	}
	error("command not found", NULL);
}
