/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:46:13 by lorey             #+#    #+#             */
/*   Updated: 2025/02/19 16:06:33 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/* setup the path split in the double pointer data->path_split                */
/* does an error if the malloc (ft_split) fails                               */
/* list of malloc at this point : nothing                                     */
/* added malloc : path_data->path_split                                       */
/* ************************************************************************** */

void	setup_arg_if_empty(t_parsing_data *p_data)
{
	if (!p_data->arg && p_data->value[0] != 0)
	{
		p_data->arg = malloc(sizeof(char *) * 2);
		p_data->arg[0] = ft_strdup(p_data->value);
		p_data->arg[1] = NULL;
	}
}

void	big_loop(t_data *data)
{
	char	*input;
	char	*shell_prompt;

	while (1)
	{
		shell_prompt = setup_prompt(data);
		printf("\r\033[K");
		input = readline(shell_prompt);
		if (!input || data->exit_nbr != -1)
			break ;
		if (!(*input == '\0'))
		{
			add_history(input);
			pre_parsing(&input, data);
			parsing(input, data);
			if (data->token)
			{
				setup_arg_if_empty(data->token);
				if (check_builtin(data, data->token) == false)
					process(data);
			}
		}
		wait_for_all(data);
		wait_for_all(data);
		printf("LAST EXIT = %d\n", data->last_exit);
		if (data->exit_nbr != -1)
			break ;
		free(input);
		free(shell_prompt);
	}
	// wait_for_all(data);
	free(input);
	free(shell_prompt);
}

int	main(int argc __attribute__((unused)),
		char **argv __attribute__((unused)), char **env)
{
	t_data	data;

//	explosion_animation();
//	text_animation();
	init_struct(&data);
	setup_env(&data, env);
	setup_path(data.path);
	setup_signal();
	big_loop(&data);
	free_everything(&data);
	if (data.exit_nbr == -1)
		return (0);
	else
		return (data.exit_nbr);
}

//en gros 	setup prompt ajoute malloc shell_promp
//			main ajoute malloc data.path
//			setup_path ajoute malloc path_data->path_split
