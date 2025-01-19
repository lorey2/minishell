/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:27:37 by maambuhl          #+#    #+#             */
/*   Updated: 2025/01/17 17:18:31 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute(t_data *data)
{
	int		i;

	i = -1;
	while (data->path->path_split[++i])
	{
		data->path->path_with_com = ft_strjoin(data->path->path_split[i], data->token->value);
		if (!data->path->path_with_com)
			error("malloc error", NULL);
		data->path->path_with_com_split = ft_split(data->path->path_with_com, ' ');
		if (!data->path->path_with_com_split)
			error("malloc error", NULL);
		if (execve(data->path->path_with_com_split[0], data->token->arg, NULL) != -1)
			break ;
		free_double_point(data->path->path_with_com_split);
		free(data->path->path_with_com);
	}
	error("command not found", NULL);
}

void	process(t_data *data)
{
	pid_t	child_pid;
	int		fd[2];
	(void)data;

	if (pipe(fd) == -1)
		error("pipe_error", NULL);
	child_pid = fork();
	if (child_pid == -1)
		error("fork_error", NULL);
	if (child_pid == 0)
	{
		close(fd[0]);
		close(fd[1]);
		execute(data);
	}
	else
	{
		wait(NULL);
		close(fd[1]);
		close(fd[0]);
	}
}
