/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:00:55 by lorey             #+#    #+#             */
/*   Updated: 2025/04/01 16:16:00 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/* setup the path split in the double pointer data->path_split				  */
/* does an error if the malloc (ft_split) fails							      */
/* list of malloc at this point : nothing									  */
/* added malloc : path_data->path_split									      */
/* ************************************************************************** */

static void	setup_arg_if_empty(t_parsing_data *p_data, t_data *data)
{
	if (!p_data->arg && p_data->value[0] != 0)
	{
		p_data->arg = safe_malloc(sizeof(char *) * 2, data);
		p_data->arg[0] = ft_strdup(p_data->value);
		if (!p_data->arg[0])
			error("malloc error", data);
		p_data->arg[1] = NULL;
	}
}

static void	big_loop_execution(t_data *data)
{
	if (!(*data->input == '\0'))
	{
		add_history(data->input);
		pre_parsing(data, 0, data->pp_data);
		if (!parsing(data->input, data))
			return ;
		if (data->token)
		{
			if (data->token->value)
				setup_arg_if_empty(data->token, data);
			process(data);
		}
	}
}

//		printf("LAST EXIT = %d\n", data->last_exit);

static void	big_loop_core(t_data *data)
{
	if (data->input[0] != '\0')
	{
		add_history(data->input);
		g_signal[0] = 1;
		big_loop_execution(data);
		wait_for_all(data);
	}
	else
	{
		data->token = safe_malloc(sizeof(t_parsing_data), data);
		init_new_token(data->token);
	}
	g_signal[0] = 0;
}

static void	big_loop(t_data *data)
{
	char	*shell_prompt;

	while (1)
	{
		g_signal[0] = 0;
		g_signal[1] = 0;
		setup_path(data, data->path);
		shell_prompt = setup_prompt(data);
		data->input = readline(shell_prompt);
		safe_free((void **)&shell_prompt);
		if (!data->input || data->exit_nbr != -1)
		{
			safe_free((void **)&shell_prompt);
			break ;
		}
		big_loop_core(data);
		if (data->exit_nbr != -1)
		{
			safe_free((void **)&shell_prompt);
			break ;
		}
		safe_free((void **)&data->input);
		safe_free((void **)&shell_prompt);
		free_tokens(data->token, data);
	}
}

/* ************************************************************************** */
/*	PUT THAT BEFORE INIT STRUCT												  */
//explosion_animation();
//text_animation();
/* ************************************************************************** */

int	main(int argc __attribute__((unused)),
		char **argv __attribute__((unused)), char **env)
{
	t_data	data;
	t_term	term;

	setup_terminal(&term);
	init_struct(&data);
	setup_env(&data, env);
	setup_signal();
	big_loop(&data);
	restore_terminal(&term);
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
