/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:27:37 by maambuhl          #+#    #+#             */
/*   Updated: 2025/01/22 18:19:38 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>

int	count_pipe(t_data *data)
{
	t_parsing_data	*token;
	int	i;

	i = 0;
	token = data->token;
	while (token->pipe)
	{
		i++;
		token = token->next;
	}
	return (i);
}

void	execute(t_data *data, t_parsing_data *token)
{
	int	i;

	i = -1;
	while (data->path->path_split[++i])
	{
		data->path->path_with_com
			= ft_strjoin(data->path->path_split_slash[i], token->value);
		if (!data->path->path_with_com)
			error("malloc error", NULL);
		if (!(execve(data->path->path_with_com, \
			token->arg, data->env->env) == -1))
			break ;
		free(data->path->path_with_com);
	}
	error("command not found", NULL);
}

void	pipex(t_data *data, t_parsing_data *token)
{
	pid_t	pid;
	int		pipefd[2];

	
	if (pipe(pipefd) == -1)
		error("Pipe err", data);
	pid = fork();
	if (pid == -1)
		error("Cannot fork process", data);
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		execute(data, token);
	}
	else
	{
		token->pid = pid;
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
	}
}

void	process(t_data *data)
{
	pid_t			child_pid;
	int				nb_pipe;
	t_parsing_data	*token;
	int				saved_stdin;
	int				status;

	saved_stdin = dup(STDIN_FILENO);
	nb_pipe = count_pipe(data);
	token = data->token;
	while (nb_pipe >= 1)
	{
		pipex(data, token);
		token = token->next;
		nb_pipe--;
	}
	child_pid = fork();
	if (child_pid == -1)
		error("fork_error", NULL);
	if (child_pid == 0)
		execute(data, token);
	else
	{
		dup2(saved_stdin, STDIN_FILENO);
		token->pid = child_pid;
		token = data->token;
		while (token)
		{
			waitpid(token->pid, &status, 0);
			token = token->next;
		}
	}
}
