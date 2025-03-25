/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*													+:+ +:+		 +:+	 */
/*   By: lorey <loic.rey.vs@gmail.com>			  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/03/21 17:59:29 by lorey			 #+#	#+#			 */
/*   Updated: 2025/03/24 16:35:44 by maambuhl         ###   LAUSANNE.ch       */
/*																			*/
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
			error("malloc error", NULL);
		if (!(execve(data->path->path_with_com, \
			token->arg, data->env->env) == -1))
			break ;
		safe_free((void **)&data->path->path_with_com);
		data->path->path_with_com = NULL;
	}
	ft_putstr_fd("Command not found\n", STDERR_FILENO);
	exit(127);
}

void	pipex(t_data *data, t_parsing_data *token)
{
	int	 pipefd[2];

	if (pipe(pipefd) == -1)
		error("Pipe err", data);

	// if (is_builtin(token->value))
	//	 execute_builtin_pipe(data, token, pipefd);
	// else
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

int	open_file(t_file *file)
{
	int	fd;

	if (file->append)
		fd = open(file->name, O_WRONLY | O_APPEND | O_CREAT, 0666);
	else
		fd = open(file->name, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fd == -1)
		error("Cannot open file", NULL);
	return (fd);
}

int	check_out_file(t_parsing_data *token)
{
	t_file	*file;

	file = token->outfile_list;
	if (token->outfile)
	{
		while (file)
		{
			token->fd_out = open_file(file);
			file = file->next;
		}
		return (1);
	}
	return (0);
}

char *gnl(void)
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
		if (i >= MAX_HERE_LINE_SIZE - 1)
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
	safe_free((void **)&token->here);
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
		if (!line)
		{
			safe_free((void **)&token->here);
			token->here = NULL;
			token->here = malloc(sizeof(char));
			if (token->here)
				token->here[0] = '\0';
			return ;
		}
		if (ft_check_line(line, token->delimiter))
		{
			safe_free((void **)&line);
			return ;
		}
		token->here = conca_here_doc(line, token);
		safe_free((void **)&line);
	}
}

int	load_here(t_parsing_data *token)
{
	while (token)
	{
		if (token->delimiter)
		{
			if (*token->delimiter == '|' || !token->delimiter)
			{
				ft_putstr_fd("Invalid delimiter in here doc\n", 2);
				return (0);
			}
			while (token->here_docs)
			{
				safe_free((void **)&token->here);
				token->here = NULL;
				token->here = malloc(sizeof(char));
				if (!token->here)
					error("Malloc error", NULL);
				token->here[0] = '\0';
				get_here_docs(token);
				token->here_docs = token->here_docs->next;
			}
		}
		token = token->next;
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
	if (!load_here(token))
	{
		get_last_token(token)->status = 2;
		return ;
	}
	
	while (nb_pipe >= 1)
	{
		token->pipe = true;
		token->saved_stdin = saved_stdin;
		check_out_file(token);
		check_in_file(token);
		if (token->value)
			pipex(data, token);
		token = token->next;
		nb_pipe--;
	}
	if (is_piped)
		token->pipe = true;
	check_out_file(token);
	check_in_file(token);
	if (token->value)
		last_exec(data, token);
	
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}

