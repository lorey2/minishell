/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:46:13 by lorey             #+#    #+#             */
/*   Updated: 2025/02/05 03:10:12 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/* setup the path split in the double pointer data->path_split                */
/* does an error if the malloc (ft_split) fails                               */
/* list of malloc at this point : nothing                                     */
/* added malloc : path_data->path_split                                       */
/* ************************************************************************** */

//\033[?25l hide cursor]
//\033[?25h hide cursor]

void	explosion_animation(void)
{
	int			i;
	int			j;
	const char	*frames[] = {\
" ,ggg, ,ggg,_,ggg,",
"dP  Y8dP  Y88P  Y8b                                      ,dPYb,              ,dPYb, ,dPYb,",
"Yb, `88'  `88'  `88                                      IP'`Yb              IP'`Yb IP'`Yb",
" `   88    88    88   gg                  gg             I8  8I              I8  8I I8  8I",
"     88    88    88                                      I8  8'              I8  8' I8  8'",
"     88    88    88   gg    ,ggg,,ggg,    gg     ,g,     I8 dPgg,    ,ggg,   I8 dP  I8 dP",
"     88    88    88   88   ,8   8P   8,   88    ,8'8,    I8dP   8I  i8   8i  I8dP   I8dP",
"     88    88    88   88   I8   8I   8I   88   ,8'  Yb   I8P    I8  I8, ,8I  I8P    I8P",
"     88    88    Y8,_,88,_,dP   8I   Yb,_,88,_,8'_   8) ,d8     I8, `YbadP' ,d8b,_ ,d8b,_",
"     88    88    `Y88P  Y88P'   8I   `Y88P  Y8P'  YY8P8P88P     `Y8888P Y8888P' Y888P' Y88",
	};

	printf("\033[H\033[J\033[?25l");
	i = -1;
	while (++i < 10)
	{
		j = ft_strlen(frames[i]) + 1;
		while (--j > 0)
		{
			printf("\r%s", frames[i] + j);
			usleep(2000);
		}
		printf("\n");
	}
	usleep(1000000);
}

void	big_loop(t_data *data)
{
	char	*input;
	char	*shell_prompt;

	while (1)
	{
		shell_prompt = setup_prompt(data);
		input = readline(shell_prompt);
		if (!input || data->exit_nbr != -1)
			break ;
		if (!(*input == '\0'))
		{
			add_history(input);
			parsing(input, data);
			if (check_builtin(data, data->token) == false)
				process(data);
		}
		wait(NULL);
		if (data->exit_nbr != -1)
			break ;
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

	explosion_animation();
	printf("\033[H\033[J\033[?25h");
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
