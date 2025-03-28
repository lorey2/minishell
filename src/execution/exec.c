/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lo>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 23:12:59 by lorey             #+#    #+#             */
/*   Updated: 2025/03/28 15:49:59 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

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
//but we shouldnt be able to execute file in pwd except if ./ before NOT DONE

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

int	open_in_file(char *file_name, t_data *data)
{
	int	fd;

	fd = open(file_name, O_RDONLY, 0666);
	if (!fd)
		error("Cannot open file", data);
	return (fd);
}

int	open_file(t_file *file, t_data *data)
{
	int	fd;

	if (file->append)
		fd = open(file->name, O_WRONLY | O_APPEND | O_CREAT, 0666);
	else
		fd = open(file->name, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fd == -1)
		error("Cannot open file", data);
	return (fd);
}

int	check_out_file(t_parsing_data *token, t_data *data)
{
	t_file	*file;

	file = token->outfile_list;
	if (token->outfile)
	{
		while (file)
		{
			token->fd_out = open_file(file, data);
			file = file->next;
		}
		return (1);
	}
	return (0);
}

char	*gnl(t_data *data)
{
	char	*buff;
	char	c;
	int		r;
	int		i;

	buff = safe_malloc(sizeof(char) * MAX_HERE_LINE_SIZE, data);
	i = 0;
	r = 1;
	while (r > 0)
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r <= 0)
		{
			safe_free((void **)&buff);
			return (NULL);
		}
		buff[i] = c;
		buff[i + 1] = 0;
		if (c == '\n')
			return (buff);
		i++;
		if (i > MAX_HERE_LINE_SIZE)
			break ;
	}
	if (i > 0)
	{
		buff[i] = '\0';
		return (buff);
	}
	safe_free((void **)&buff);
	return (NULL);
}

char	*conca_here_doc(char *line, t_parsing_data *token, t_data *data)
{
	int		len;
	char	*conca;
	int		i;
	int		j;

	len = ft_strlen(line) + ft_strlen(token->here);
	conca = safe_malloc(sizeof(char) * (len + 1), data);
	i = 0;
	while (token->here[i])
	{
		conca[i] = token->here[i];
		i++;
	}
	j = 0;
	while (line[j])
		conca[i++] = line[j++];
	conca[i] = '\0';
	safe_free((void **)&token->here);
	return (conca);
}

void	here_doc_write(t_parsing_data *token, int *pipefd)
{
	close(pipefd[0]);
	if (token->value)
		ft_putstr_fd(token->here, pipefd[1]);
	close(pipefd[1]);
}

void	here_doc(t_parsing_data *token, t_data *data)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		exit(0);
	pid = fork();
	if (pid == -1)
		exit(0);
	if (!pid)
	{
		here_doc_write(token, pipefd);
		free_everything(data);
		exit(0);
	}
	else
	{
		close(pipefd[1]);
		dup2(pipefd[0], token->fd_in);
		close(pipefd[0]);
		wait(NULL);
	}
}

int	check_in_file(t_parsing_data *token, t_data *data)
{
	t_file	*file;

	file = token->infile_list;
	while (file)
	{
		if (access(file->name, F_OK) != 0)
		{
			ft_putstr_fd(token->infile, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
			return (-1);
		}
		file = file->next;
	}
	if (token->infile)
	{
		token->fd_in = open_in_file(token->infile, data);
		dup2(token->fd_in, STDIN_FILENO);
		close(token->fd_in);
		return (1);
	}
	else if (token->here)
	{
		here_doc(token, data);
		return (1);
	}
	return (0);
}

t_parsing_data	*get_last_token(t_parsing_data *token)
{
	t_parsing_data	*head;

	head = token;
	while (head)
	{
		if (!head->next)
			return (head);
		head = head->next;
	}
	return (head);
}

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
	if (is_builtin(token->value) && !token->pipe)
	{
		exec_builtin(data, token);
		return ;
	}
	pid_t child_pid = fork();
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

int	get_here_docs(t_parsing_data *token, t_data *data)
{
	char	*line;

	while (1)
	{
		line = gnl(data);
		if (!line || g_signal[1] == 1)
		{
			safe_free((void **)&token->here);
			token->here = NULL;
			token->here = safe_malloc(sizeof(char), data);
			if (token->here)
				token->here[0] = '\0';
			if (g_signal[1] == 1)
				return (0);
			return (1);
		}
		if (ft_check_line(line, token->here_docs->delimiter))
		{
			safe_free((void **)&line);
			return (1);
		}
		token->here = conca_here_doc(line, token, data);
		safe_free((void **)&line);
	}
}

int	load_here(t_parsing_data *token, t_data *data)
{
	t_here_docs	*tmp;

	while (token)
	{
		while (token->here_docs)
		{
			if (!token->here_docs->delimiter)
				return (ft_putstr_fd("Invalid delimiter in here doc\n", 2), 0);
			safe_free((void **)&token->here);
			token->here = safe_malloc(sizeof(char), data);
			token->here[0] = '\0';
			if (!get_here_docs(token, data))
				return (0);
			tmp = token->here_docs;
			token->here_docs = token->here_docs->next;
			safe_free((void **)&tmp);
		}
		token = token->next;
	}
	return (1);
}

int	check_file(t_parsing_data *token, t_data *data, int saved_stdin)
{
	check_out_file(token, data);
	if (check_in_file(token, data) == -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
		return (0);
	}
	return (1);
}

void	process(t_data *data)
{
	int				nb_pipe;
	t_parsing_data	*token;
	int				saved_stdin;
	bool			is_piped;

	is_piped = false;
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
		error("dup error", data);
	nb_pipe = count_pipe(data);
	if (nb_pipe >= 1)
		is_piped = true;
	token = data->token;
	if (!load_here(token, data))
	{
		if (g_signal[1] == 1)
			get_last_token(token)->status = 130;
		else
			get_last_token(token)->status = 2;
		return ;
	}
	while (nb_pipe >= 1)
	{
		token->pipe = true;
		token->saved_stdin = saved_stdin;
		if (!check_file(token, data, saved_stdin))
			return ;
		if (token->value)
			pipex(data, token);
		token = token->next;
		nb_pipe--;
	}
	if (is_piped)
		token->pipe = true;
	if (!check_file(token, data, saved_stdin))
		return ;
	if (token->value)
		last_exec(data, token);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}

