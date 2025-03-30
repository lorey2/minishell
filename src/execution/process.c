/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:31:03 by maambuhl          #+#    #+#             */
/*   Updated: 2025/03/30 20:32:53 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command_pipe(t_data *data, t_parsing_data *token, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error("Cannot fork process", data);
	if (pid == 0)
	{
		close(pipefd[0]);
		if (token->fd_out != STDOUT_FILENO)
			dup2(token->fd_out, STDOUT_FILENO);
		else
			dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		execute(data, token);
	}
	token->pid = pid;
}

void	execute(t_data *data, t_parsing_data *token)
{
	int	i;

	i = -1;
	if (is_builtin(token->value))
		exec_builtin(data, token);
	if (token->value[0] == '.' || token->value[0] == '/')
		if (!(execve(token->value, token->arg, data->env->env) == -1))
			return ;
	while (data->path->path_split_slash[++i])
	{
		data->path->path_with_com
			= ft_strjoin(data->path->path_split_slash[i], token->value);
		if (!data->path->path_with_com)
			error("malloc error", data);
		if (!(execve(data->path->path_with_com, \
			token->arg, data->env->env) == -1))
			break ;
		safe_free((void **)&data->path->path_with_com);
		data->path->path_with_com = NULL;
	}
	ft_putstr_fd("Command not found\n", STDERR_FILENO);
	free_everything(data);
	exit(127);
}

void	pipex(t_data *data, t_parsing_data *token)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		error("Pipe err", data);
	execute_command_pipe(data, token, pipefd);
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
}
