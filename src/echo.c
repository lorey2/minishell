/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 17:55:52 by lorey             #+#    #+#             */
/*   Updated: 2025/01/18 20:14:30 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//here we will have to change the code to correspond to that
//$ bash
//lorey@c3r6s4:~$ echo \n
//n
//lorey@c3r6s4:~$ echo -e \n
//n
//lorey@c3r6s4:~$ echo -E \n
//n
//lorey@c3r6s4:~$ echo -e '\n'
//
//
//lorey@c3r6s4:~$ echo -E '\n'
//\n
//WARNING WARNINIG WARNINIG WARNINIG
//now it works only without quote.
//It skips backslash wich is correct with and without -e
//is_e is useless because it works only with  '' and/or ""
//the only function to modify is echo_write if we handle the '' and "" elsewhere

void	echo_write(t_parsing_data *p_data, int i, bool is_e, bool is_n)
{
	int	j;

	(void)is_e;
	i--;
	while (p_data->arg[++i])
	{
		j = -1;
		while (p_data->arg[i][++j])
		{
			if (p_data->arg[i][j] == '\\')
				j++;
			write(1, &p_data->arg[i][j], 1);
		}
	}
	if (is_n == false)
		write(1, "\n", 1);
}

bool	contains(char *to_verif, char letter)
{
	int	i;

	i = 0;
	while (to_verif[++i])
		if (to_verif[i] == letter)
			return (true);
	return (false);
}

bool	contains_only_enbig_e(char *to_verif)
{
	int	i;

	i = 0;
	while (to_verif[++i])
		if (to_verif[i] != 'e' && to_verif[i] != 'E' && to_verif[i] != 'n')
			return (false);
	return (true);
}

void	echo(t_parsing_data *p_data)
{
	int		i;
	bool	is_e;
	bool	is_n;

	i = 0;
	is_e = false;
	is_e = false;
	is_n = false;
	while (p_data->arg[++i][0] == '-')
	{
		if (contains_only_enbig_e(p_data->arg[i]) == false)
		{
			echo_write(p_data, i, is_e, is_n);
			return ;
		}
		if (contains(p_data->arg[i], 'e'))
			is_e = true;
		if (contains(p_data->arg[i], 'n'))
			is_n = true;
	}
	echo_write(p_data, i, is_e, is_n);
}
