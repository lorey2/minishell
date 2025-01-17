/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:46:13 by lorey             #+#    #+#             */
/*   Updated: 2025/01/17 23:14:39 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process(t_path_data *data, char *input)
{
	pid_t	child_pid;
	int		fd[2];

	if (pipe(fd) == -1)
		error("pipe_error", NULL);
	child_pid = fork();
	if (child_pid == -1)
		error("fork_error", NULL);
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
		if (!(*input == '\0'))
		{
			add_history(input);
			parsing(input, data);
			process(data->path, input);
		}
		free(input);
		free(shell_prompt);
	}
	free(input);
	free(shell_prompt);
}

int	main(int argc __attribute__((unused)),
		char **argv __attribute__((unused)), char **env)
{
	t_data	data;

	init_struct(&data);
	setup_env(&data, env);
	setup_path(data.path);
	setup_signal();
	big_loop(&data);
	free_everything(&data);
	return (0);
}

//en gros 	setup prompt ajoute malloc shell_promp
//			main ajoute malloc data.path
//			setup_path ajoute malloc path_data->path_split
