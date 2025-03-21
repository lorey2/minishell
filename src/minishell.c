/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:46:13 by lorey             #+#    #+#             */
/*   Updated: 2025/03/21 16:51:19 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

//

#include "minishell.h"

/* ************************************************************************** */
/* setup the path split in the double pointer data->path_split                */
/* does an error if the malloc (ft_split) fails                               */
/* list of malloc at this point : nothing                                     */
/* added malloc : path_data->path_split                                       */
/* ************************************************************************** */

static void	setup_arg_if_empty(t_parsing_data *p_data)
{
	if (!p_data->arg && p_data->value[0] != 0)
	{
		p_data->arg = malloc(sizeof(char *) * 2);
		p_data->arg[0] = ft_strdup(p_data->value);
		p_data->arg[1] = NULL;
	}
}

static void	big_loop_execution(t_data *data)
{
	if (!(*data->input == '\0'))
	{
		add_history(data->input);
		pre_parsing(data, 0, data->pp_data);
		parsing(data->input, data);
		if (data->token)
		{
			if (data->token->value)
				setup_arg_if_empty(data->token);
			process(data);
		}
	}
}

static void	big_loop(t_data *data)
{
	char	*shell_prompt;

	while (1)
	{
		g_signal = 0;
		setup_path(data, data->path);
		shell_prompt = setup_prompt(data);
//		printf("\r\033[K");
		data->input = readline(shell_prompt);
		if (!data->input || data->exit_nbr != -1)
			break ;
		if (!g_signal)
		{
			big_loop_execution(data);
			wait_for_all(data);
			final_wait(data);
			printf("LAST EXIT = %d\n", data->last_exit);
		}
		if (data->exit_nbr != -1)
			break ;
		free(data->input);
		free(shell_prompt);
	}
	free(data->input);
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
	setup_signal();
	big_loop(&data);
	free_everything(&data);
	write(1, "exit\n", 5);
	if (data.exit_nbr == -1)
		return (0);
	else
		return (data.exit_nbr);
}

//en gros 	setup prompt ajoute malloc shell_promp
//			main ajoute malloc data.path
//			setup_path ajoute malloc path_data->path_split
