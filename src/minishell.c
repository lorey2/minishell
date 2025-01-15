/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:46:13 by lorey             #+#    #+#             */
/*   Updated: 2025/01/15 14:35:31 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process(t_path_data *data, char *input)
{
	pid_t	child_pid;
	int		fd[2];

	if (pipe(fd) == -1)
		error("pipe_error", data);
	child_pid = fork();
	if (child_pid == -1)
		error("fork_error", data);
	if (child_pid == 0)
	{
		close(fd[0]);
		close(fd[1]);
		execute(input, data);
	}
	else
	{
		wait(NULL);
		close(fd[1]);
		close(fd[0]);
	}
}

/* ************************************************************************** */
/* setup the path split in the double pointer data->path_split                */
/* does an error if the malloc (ft_split) fails                               */
/* list of malloc at this point : nothing                                     */
/* added malloc : path_data->path_split                                       */
/* ************************************************************************** */

void	setup_path(t_path_data *path_data)
{
	char	*path;

	path = getenv("PATH");
	path_data->path_split = ft_split(path, ':');
	if (!path_data)
		error("Malloc error", path_data);
}

char	*setup_prompt(t_data *data)
{
	int		i;
	char	*shell_prompt;

	shell_prompt = malloc(1024 * sizeof(char));
	if (!shell_prompt)
		error("malloc error", data->path);
	if (getcwd(shell_prompt, 1024) == NULL)
		error("getcwd", data->path);
	i = -1;
	while (shell_prompt[++i])
		;
	shell_prompt[i] = '$';
	shell_prompt[i + 1] = ' ';
	shell_prompt[i + 2] = '\0';
	return (shell_prompt);
}

void	big_loop(t_data *data)
{
	char	*input;
	char	*shell_prompt;

	while (1)
	{
		shell_prompt = setup_prompt(data);
		input = readline(shell_prompt);
		if (!input)
			break ;
		add_history(input);
//		parsing(input, data);
		process(data->path, input);
		free(input);
		free(shell_prompt);
	}
}

int	main(void)
{
	t_data	data;

	data.path = malloc(sizeof(t_path_data));
	if (!data.path)
		error("malloc error", NULL);
	setup_path(data.path);
	setup_signal();
	big_loop(&data);
	return (0);
}

//en gros 	setup prompt ajoute malloc shell_promp
//			main ajoute malloc data.path
//			setup_path ajoute malloc path_data->path_split
