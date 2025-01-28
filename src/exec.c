/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:27:37 by maambuhl          #+#    #+#             */
/*   Updated: 2025/01/28 17:10:42 by maambuhl         ###   LAUSANNE.ch       */
/*   Updated: 2025/01/21 17:34:04 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	count_pipe(t_data *data)
{
	t_parsing_data	*token;
	int	i;

	i = 0;
	token = data->token;
	while (token)
	{
		if (token->pipe)
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
		if (token->fd_out != STDOUT_FILENO)
			dup2(token->fd_out, STDOUT_FILENO);
		else
			dup2(pipefd[1], token->fd_out);
		close(pipefd[1]);
		execute(data, token);
	}
	else
	{
		close(pipefd[1]);
		dup2(pipefd[0], token->fd_in);
		close(pipefd[0]);
	}
}

int	open_file(t_parsing_data *token)
{
	int	fd;

	if (token->append_file)
		fd = open(token->value, O_WRONLY | O_APPEND | O_CREAT, 0666);
	else
		fd = open(token->value, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fd == -1)
		error("Cannot open file", NULL);
	return (fd);
}

int	check_out_file(t_parsing_data *token)
{
	if (token->next)
	{
		if (token->next->out_file)
		{
			token->fd_out = open_file(token->next);
			return (1);
		}
	}
	return (0);
}

void	last_exec(t_data *data, t_parsing_data *token, int saved_stdin)
{
	int	child_pid;
	int	status;

	child_pid = fork();
	if (child_pid == -1)
		error("fork_error", NULL);
	if (child_pid == 0)
	{
		dup2(token->fd_out, STDOUT_FILENO);
		execute(data, token);
		close(token->fd_out);
	}
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

void	process(t_data *data)
{
	int				nb_pipe;
	t_parsing_data	*token;
	int				saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	nb_pipe = count_pipe(data);
	token = data->token;
	while (nb_pipe >= 1)
	{
		pipex(data, token);
		token = token->next;
		if (!token->is_cmd)
			token = token->next;
		nb_pipe--;
	}
	check_out_file(token);
	last_exec(data, token, saved_stdin);
}
