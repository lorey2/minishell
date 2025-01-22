/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:46:13 by lorey             #+#    #+#             */
/*   Updated: 2025/01/22 17:59:41 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <unistd.h>

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
			if (check_builtin(data, data->token) == false)
				process(data);
		}
		wait(NULL);
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
