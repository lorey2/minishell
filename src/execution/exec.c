/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:27:37 by maambuhl          #+#    #+#             */
/*   Updated: 2025/02/19 20:22:31 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

void    execute_builtin_pipe(t_data *data, t_parsing_data *token, int pipefd[2])
{
    pid_t   pid;

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
        exec_builtin(data, token);
        exit(0);
    }
    token->pid = pid;
}

void    execute_command_pipe(t_data *data, t_parsing_data *token, int pipefd[2])
{
    pid_t   pid;

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
	if (token->value[0] == '.' || token->value[0] == '/')
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

void    pipex(t_data *data, t_parsing_data *token)
{
    int     pipefd[2];

    if (pipe(pipefd) == -1)
        error("Pipe err", data);

    if (is_builtin(token->value))
        execute_builtin_pipe(data, token, pipefd);
    else
        execute_command_pipe(data, token, pipefd);

    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
}

int	open_in_file(char *file_name)
{
	int	fd;

	fd = open(file_name, O_RDONLY, 0666);
	if (!fd)
		error("Cannot open file", NULL);
	return (fd);
}

int	open_file(t_parsing_data *token)
{
	int	fd;

	if (token->append_file)
		fd = open(token->outfile, O_WRONLY | O_APPEND | O_CREAT, 0666);
	else
		fd = open(token->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fd == -1)
		error("Cannot open file", NULL);
	return (fd);
}

int	check_out_file(t_parsing_data *token)
{
	if (token->outfile)
	{
		token->fd_out = open_file(token);
		return (1);
	}
	return (0);
}

char	*gnl(void)
{
	char	*buff;
	char	c;
	int		r;
	int		i;

	buff = malloc(sizeof(char) * MAX_HERE_LINE_SIZE);
	if (!buff)
		error("gnl malloc error", NULL);
	i = 0;
	r = 1;
	while (r)
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r == -1)
			error("cannot read here_doc", NULL);
		buff[i] = c;
		buff[i + 1] = 0;
		if (c == '\n')
			return (buff);
		i++;
	}
	return (NULL);
}

char	*conca_here_doc(char *line, t_parsing_data *token)
{
	int		len;
	char	*conca;
	int		i;
	int		j;

	len = ft_strlen(line) + ft_strlen(token->here);
	conca = malloc(sizeof(char) * (len + 1));
	if (!conca)
		error("Malloc conca error", NULL);
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
	free(token->here);
	return (conca);
}

void	here_doc_write(t_parsing_data *token, int *pipefd)
{
	close(pipefd[0]);
	ft_putstr_fd(token->here, pipefd[1]);
	close(pipefd[1]);
	exit(0);
}

void	here_doc(t_parsing_data *token)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		exit(0);
	pid = fork();
	if (pid == -1)
		exit(0);
	if (!pid)
		here_doc_write(token, pipefd);
	else
	{
		close(pipefd[1]);
		dup2(pipefd[0], token->fd_in);
		close(pipefd[0]);
		wait(NULL);
	}
}

int	check_in_file(t_parsing_data *token)
{
	if (token->infile)
	{
		token->fd_in = open_in_file(token->infile);
		dup2(token->fd_in, STDIN_FILENO);
		close(token->fd_in);
		return (1);
	}
	else if (token->here)
	{
		here_doc(token);
		return (1);
	}
	return (0);
}

void	wait_for_all(t_data *data)
{
	int				status;
	t_parsing_data	*token;

	if (!data->token)
		return ;
	token = data->token;
	while (token)
	{
		waitpid(token->pid, &status, 0);
		token = token->next;
	}
}

void    last_exec(t_data *data, t_parsing_data *token)
{
    if (is_builtin(token->value))
    {
        int stdout_backup = -1;
        
        if (token->fd_out != STDOUT_FILENO)
        {
            stdout_backup = dup(STDOUT_FILENO);
            dup2(token->fd_out, STDOUT_FILENO);
        }
        
        exec_builtin(data, token);
        
        if (stdout_backup != -1)
        {
            dup2(stdout_backup, STDOUT_FILENO);
            close(stdout_backup);
        }
        return;
    }

    pid_t child_pid = fork();
    if (child_pid == -1)
        error("fork_error", NULL);
    if (child_pid == 0)
    {
        if (token->fd_out != STDOUT_FILENO)
            dup2(token->fd_out, STDOUT_FILENO);
        execute(data, token);
    }
    token->pid = child_pid;
}

void	get_here_docs(t_parsing_data *token)
{
	char	*line;

	while (1)
	{
		line = gnl();
		if (!ft_strncmp(line, token->delimiter,
				ft_strlen(token->delimiter)))
		{
			free(token->delimiter);
			token->delimiter = NULL;
			free(line);
			return ;
		}
		token->here = conca_here_doc(line, token);
		free(line);
	}
}

void	load_here(t_parsing_data *token)
{
	while (token)
	{
		if (token->delimiter)
		{
			if (!token->here)
			{
				token->here = malloc(sizeof(char));
				if (!token->here)
					error("Malloc error", NULL);
				token->here[0] = '\0';
			}
			get_here_docs(token);

		}
		token = token->next;
	}
}

void    process(t_data *data)
{
    int             nb_pipe;
    t_parsing_data  *token;
    int             saved_stdin;

    saved_stdin = dup(STDIN_FILENO);
    if (saved_stdin == -1)
        error("dup error", data);

    nb_pipe = count_pipe(data);
    token = data->token;
    load_here(token);
    
    while (nb_pipe >= 1)
    {
        token->saved_stdin = saved_stdin;
        check_out_file(token);
        check_in_file(token);
        if (token->value)
            pipex(data, token);
        token = token->next;
        nb_pipe--;
    }
    
    check_out_file(token);
    check_in_file(token);
    if (token->value)
        last_exec(data, token);
    
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    wait_for_all(data);
}

