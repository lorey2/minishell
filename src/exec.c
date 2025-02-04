/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:27:37 by maambuhl          #+#    #+#             */
/*   Updated: 2025/02/03 15:18:30 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>

int	count_pipe(t_data *data)
{
	t_parsing_data	*token;
	int				i;

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

//we do execve with no path if value has already one

void	execute(t_data *data, t_parsing_data *token)
{
	int	i;

	i = -1;
	if (!(execve(token->value, token->arg, data->env->env) == -1))
		return ;
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
		token->pid = pid;
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
	}
}

int	open_in_file(t_parsing_data *token)
{
	int	fd;

	fd = open(token->value, O_RDONLY, 0666);
	if (!fd)
		error("Cannot open file", NULL);
	return (fd);
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
			// dup2(token->fd_in, STDIN_FILENO);
			// close(token->fd_in);
			return (1);
		}
	}
	return (0);
}

int	check_in_file(t_parsing_data *token)
{
	if (token->in_file)
	{
		token->next->fd_in = open_in_file(token);
		dup2(token->next->fd_in, STDIN_FILENO);
		close(token->next->fd_in);
		return (1);
	}
	return (0);
}

void	wait_for_all(t_data *data)
{
	int				status;
	t_parsing_data	*token;

	token = data->token;
	while (token)
	{
		waitpid(token->pid, &status, 0);
		token = token->next;
	}
}

void	last_exec(t_data *data, t_parsing_data *token, int saved_stdin)
{
	int	child_pid;

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
		check_out_file(token);
		if (check_in_file(token))
			token = token->next;
		pipex(data, token);
		token = token->next;
		if (!token->is_cmd)
			token = token->next;
		nb_pipe--;
	}
	check_out_file(token);
	check_in_file(token);
	while (!token->is_cmd)
		token = token->next;
	last_exec(data, token, saved_stdin);
	// wait_for_all(data);
}
