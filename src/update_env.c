/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:34:03 by lorey             #+#    #+#             */
/*   Updated: 2025/01/21 18:25:24 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_env_error(char *var_name, char *message)
{
	write_err(message);
	write(1, var_name, ft_strlen(var_name));
	write(1, "\n", 1);
}

bool	is_char_equal(char	*arg)
{
	int	i;

	i = -1;
	while (arg[++i])
		if (arg[i] == '=')
			return (true);
	return (false);
}

int	find_index(t_env_data *e_data, char *var_name)
{
	int	i;
	int	j;
	int	test;

	i = -1;
	if (is_char_equal(var_name))
		return (write_env_error(var_name, "env name cannot have = in it"), -1);
	while (e_data->path[++i])
	{
		test = 0;
		j = -1;
		while (var_name[++j])
			if (e_data->path[i][j] != var_name[j])
				test = 1;
		if (test == 0)
			break ;
	}
	if (test == 0)
		return (i);
	return (write_env_error(var_name, "cannot find variable"), -2);
}

char	*get_env(t_env_data *e_data, char *var_name)
{
	int		i;
	int		backup;
	char	*value;
	int		j;

	i = find_index(e_data, var_name);
	if (i < 0)
		return (NULL);
	while (e_data->env[i][++j] != '=')
		;
	backup = j;
	if (e_data->env[i][j + 1])
	{
		while (e_data->env[i][++j])
			;
		value = malloc(sizeof(char) * (j - backup));
		j = -1;
		while (++backup)
			value[++j] = e_data->env[i][backup];
		value[j] = '\0';
		return (value);
	}
	return (NULL);
}

void	set_env(t_env_data *e_data, char *var_name, char *value)
{
	int	i;
	int	j;
	int	k;

	i = find_index(e_data, var_name);
	if (i < 0)
		return ;
	free(e_data->env[i]);
	e_data->env[i] = malloc(sizeof(char)
			* (ft_strlen(var_name) + ft_strlen(value) + 2));
	j = -1;
	while (var_name[++j])
		e_data->env[i][j] = var_name[j];
	var_name[j] = '=';
	k = -1;
	while (value[++k])
		e_data->env[i][++j] = value[k];
	e_data->env[i][j] = '0';
}
