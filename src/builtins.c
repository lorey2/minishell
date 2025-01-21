/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 17:03:28 by lorey             #+#    #+#             */
/*   Updated: 2025/01/21 23:09:17 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	does_contain_only(char *data, char *list_args)
{
	int		i;
	int		j;
	bool	check;

	i = 0;
	while (data[++i])
	{
		j = -1;
		check = false;
		while (list_args[++j])
		{
			if (data[i] == list_args[j])
				check = true;
		}
		if (check == false)
			return (false);
	}
	return (true);
}

void	fill(char *data, t_path_data *path_data)
{
	int	i;

	i = 0;
	while (data[++i])
	{
		if (data[i] == 'L')
			path_data->is_big_l = true;
		else if (data[i] == 'P')
			path_data->is_big_p = true;
		else if (data[i] == 'E')
			path_data->is_big_e = true;
		else if (data[i] == 'e')
			path_data->is_e = true;
		else if (data[i] == '@')
			path_data->is_at = true;
		else if (data[i] == 'n')
			path_data->is_n = true;
	}
}

void	init_flags(t_path_data *path_data)
{
	path_data->is_big_l = false;
	path_data->is_big_p = false;
	path_data->is_big_e = false;
	path_data->is_at = false;
	path_data->is_e = false;
	path_data->is_n = false;
	path_data->home = getenv("HOME");
}

void	write_err(char *message)
{
	write(1, message, ft_strlen(message));
}

bool	check_builtin(t_data *data, t_parsing_data *p_data)
{
	if (ft_isequal(p_data->value, "echo"))
		return (echo(p_data, data->path), true);
	else if (ft_isequal(p_data->value, "cd"))
		return (cd(p_data, data->path), true);
	else if (ft_isequal(p_data->value, "pwd"))
		return (pwd(p_data, data->path), true);
	else if (ft_isequal(p_data->value, "export"))
		return (write(1, "not yet implemented", 19), true);
	else if (ft_isequal(p_data->value, "unset"))
		return (unset(data, p_data), true);
	else if (ft_isequal(p_data->value, "env"))
		return (env(data->env), true);
	else if (ft_isequal(p_data->value, "exit"))
		return (mini_exit(data, p_data), true);
	return (false);
}
