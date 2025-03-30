/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lo>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 23:12:59 by lorey             #+#    #+#             */
/*   Updated: 2025/03/30 18:38:01 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//we do execve with no path if value has already one
//but we shouldnt be able to execute file in pwd except if ./ before NOT DONE

void	final_wait(t_data *data)
{
	t_parsing_data	*token;
	int				status;

	status = 0;
	token = data->token;
	while (token)
	{
		waitpid(token->pid, &status, 0);
		token = token->next;
	}
}

void	wait_for_all(t_data *data)
{
	t_parsing_data	*token;
	int				status;

	status = 0;
	if (!data->token)
	{
		data->last_exit = 0;
		return ;
	}
	token = data->token;
	while (token)
	{
		if (token->status == -1)
		{
			waitpid(token->pid, &status, 0);
			if (WIFEXITED(status))
				token->status = WEXITSTATUS(status);
			else
				token->status = 0;
		}
		token = token->next;
	}
	token = data->token;
	data->last_exit = get_last_token(token)->status;
	final_wait(data);
}

void	last_exec(t_data *data, t_parsing_data *token)
{
	pid_t	child_pid;

	if (is_builtin(token->value) && !token->pipe)
	{
		exec_builtin(data, token);
		return ;
	}
	child_pid = fork();
	if (child_pid == -1)
		error("fork_error", data);
	if (child_pid == 0)
	{
		if (token->fd_out != STDOUT_FILENO)
			dup2(token->fd_out, STDOUT_FILENO);
		execute(data, token);
	}
	token->pid = child_pid;
}

t_parsing_data	*launch_pipes(t_data *data, int saved_stdin)
{
	int				nb_pipe;
	bool			is_piped;
	t_parsing_data	*token;

	is_piped = false;
	nb_pipe = count_pipe(data);
	if (nb_pipe >= 1)
		is_piped = true;
	token = data->token;
	while (nb_pipe >= 1)
	{
		token->pipe = true;
		token->saved_stdin = saved_stdin;
		if (!check_file(token, data, saved_stdin))
			return (NULL);
		if (token->value && *token->value)
			pipex(data, token);
		token = token->next;
		nb_pipe--;
	}
	token->pipe = is_piped;
	return (token);
}

void	process(t_data *data)
{
	t_parsing_data	*token;
	int				saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
		error("dup error", data);
	token = data->token;
	if (!load_here(token, data))
	{
		if (g_signal[1] == 1)
		{
			get_last_token(token)->status = 130;
			free_heres(token);
		}
		else
			get_last_token(token)->status = 2;
		return ;
	}
	token = launch_pipes(data, saved_stdin);
	if (!token || !check_file(token, data, saved_stdin))
		return ;
	if (token->value && *token->value)
		last_exec(data, token);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}
